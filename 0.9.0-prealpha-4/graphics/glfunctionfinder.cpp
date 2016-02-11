/* Copyright (C) 2016, Chesley Kraniak
 *
 * This code is distributed under the terms of the GPLv3 license, which should
 * have been included with this file in the root directory as "LICENSE.txt". If
 * you do not have a copy of the license, see:
 *     http://www.gnu.org/licenses/gpl-3.0.txt
 * or:
 *     https://github.com/CKraniak/OpenRS
 * for the license.
 */
#include <algorithm>
#include <fstream>
#include <exception>
#include <stdexcept>

#include <sstream>

#include "glfunctionfinder.h"

// Note: a good number of the functions in here make a lot of assumptions about
// what has and hasn't been checked before they're been called. Many cannot
// be arbitrarily called, but must have things like a current GL context, or
// a aGL context with a high enough version, etc. known to exist before being
// called; they won't check, so likely if you don't have what they need done
// before calling them, the call will trash the program. So check the usage
// of a function before you try to use it.

// Some platform-dependent code is in here. Anything starting with "wgl" is
// Windows-only. PFDs, HDCs, and HWNDs are all Windows only, I believe.

PFNGLGETSTRINGIPROC                 glGetStringi;
PFNWGLGETEXTENSIONSSTRINGARBPROC    wglGetExtensionsStringARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC   wglCreateContextAttribsARB;
PFNWGLCHOOSEPIXELFORMATARBPROC      wglChoosePixelFormatARB;
PFNGLGENBUFFERSPROC                 glGenBuffers;
PFNGLBINDBUFFERPROC                 glBindBuffer;
PFNGLBUFFERDATAPROC                 glBufferData;
PFNGLGENVERTEXARRAYSPROC            glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC            glBindVertexArray;
PFNGLDELETEVERTEXARRAYSPROC         glDeleteVertexArrays;
PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC        glVertexAttribPointer;
PFNGLDISABLEVERTEXATTRIBARRAYPROC   glDisableVertexAttribArray;
PFNGLCREATESHADERPROC               glCreateShader;
PFNGLSHADERSOURCEPROC               glShaderSource;
PFNGLCOMPILESHADERPROC              glCompileShader;
PFNGLGETSHADERIVPROC                glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC           glGetShaderInfoLog;
PFNGLATTACHSHADERPROC               glAttachShader;
PFNGLLINKPROGRAMPROC                glLinkProgram;
PFNGLGETPROGRAMIVPROC               glGetProgramiv;
PFNGLDETACHSHADERPROC               glDetachShader;
PFNGLDELETESHADERPROC               glDeleteShader;
PFNGLUSEPROGRAMPROC                 glUseProgram;
PFNGLVERTEXATTRIB3FPROC             glVertexAttrib3f;
PFNGLDELETEBUFFERSPROC              glDeleteBuffers;

// File level global.
static void * GlobalAddr = NULL;

// ***********************************************************************

// File level global.
static void * getGlAddress(const char * name) {
    void * retval = reinterpret_cast<void *>(wglGetProcAddress(name));
    if(     retval == NULL ||
            retval == reinterpret_cast<void *>( 1) ||
            retval == reinterpret_cast<void *>( 2) ||
            retval == reinterpret_cast<void *>(-1)) {
        retval = NULL;
        static HMODULE gl_dll = LoadLibrary("opengl32.dll");
        if(gl_dll == NULL) {
            ERR_MSGOUT_GLE("getGlAddress opengl32.dll loading failure");
            return NULL;
        }
        retval = reinterpret_cast<void *>(GetProcAddress(gl_dll, name));
    }
    return retval;
}

// Setup and Remove DC / Context are done from a WndProc.
// Furthermore, setupDC comes before setupContext, while
// removeContext comes before removeDC.
int GLFunctionFinder::setupDC(HWND h_wnd)
{
    this->h_dc = GetDC(h_wnd);
    if (this->h_dc == NULL) {
        ERR_MSGOUT_GLE("InitWndProc GetDC failure");
        this->setQuitInitWindow();
        return(0);
    }
    return 1;
}

int GLFunctionFinder::setupContext()
{
    int px_format_default = ChoosePixelFormat(this->h_dc,
                                              &(this->pfdarr_));
    if (px_format_default == 0) {
        ERR_MSGOUT_GLE("InitWndProc ChoosePixelFormat failure");
        this->setQuitInitWindow();
        return(0);
    }
    if(!SetPixelFormat(this->h_dc, px_format_default, &(this->pfdarr_))) {
        ERR_MSGOUT_GLE("InitWndProc SetPixelFormat failure");
        this->setQuitInitWindow();
        return(0);
    }
    this->h_context = wglCreateContext(this->h_dc);
    if (this->h_context == NULL) {
        ERR_MSGOUT_GLE("InitWndProc wglCreateContext failure");
        this->setQuitInitWindow();
        return(0);
    }
    if(!wglMakeCurrent (this->h_dc, this->h_context)) {
        ERR_MSGOUT_GLE("InitWndProc wglMakeCurrent failure");
        this->setQuitInitWindow();
        return(0);
    }
    return 1;
}

int GLFunctionFinder::removeDC(HWND h_wnd)
{
    if(ReleaseDC(h_wnd, this->h_dc) == 0) {
        ERR_MSGOUT_GLE("InitWndProc ReleaseDC failure");
        this->setQuitInitWindow();
        return(0);
    }
    this->h_context = NULL;
    return 1;
}

int GLFunctionFinder::removeContext()
{
    if(!wglMakeCurrent(NULL, NULL)) {
        ERR_MSGOUT_GLE("InitWndProc wglMakeCurrent failure");
        this->setQuitInitWindow();
        return(0);
    }
    if(!wglDeleteContext(this->h_context)) {
        ERR_MSGOUT_GLE("InitWndProc wglDeleteContext failure");
        this->setQuitInitWindow();
        return(0);
    }
    return 1;
}

int GLFunctionFinder::initGL()
{
    // Check the OpenGL version number, start basic: look for a major
    // version less than 3.
    const unsigned char * version_string =
            static_cast<const unsigned char *>(glGetString(GL_VERSION));
    if(version_string[0] == '1' || version_string[0] == '2') {
        ERR_MSGOUT("OpenGL version is too low");
        std::stringstream ss;
        ss << version_string;
        ERR_MSGOUT(ss.str().c_str());
        this->setQuitInitWindow();
        return(0);
    }
    if (!this->setupExtensionStringVector(this->h_dc)) {
        this->setQuitInitWindow();
        return(0);
    }
    if ( !this->setupCoreGlDisplayFunctions() ) {
        this->setQuitInitWindow();
        return(0);
    }
    this->saveExtensionListToFile("extensions.txt");

    const unsigned char * vendor_string = glGetString(GL_VENDOR);
    const unsigned char * renderer_string = glGetString(GL_RENDERER);

    std::stringstream s;
    s << vendor_string << ", " << renderer_string;
    ERR_MSGOUT_DEBUG(s.str().c_str()); // Says "NVIDIA Corporation" on work PC
                                       // Quadro K2100m

    int n;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
    std::stringstream ss;
    ss << n;
    ERR_MSGOUT_DEBUG(ss.str().c_str()); // 16 (on Quadro K2100m)

    n = 0;
    ss.str(std::string());
    glGetIntegerv(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS, &n);
    ss << n;
    ERR_MSGOUT_DEBUG(ss.str().c_str()); // 32 (on Quadro K2100m)

    return 1;
}

// Internal PFD getter.
// Assumes pf_attribs_ is properly set.
int GLFunctionFinder::getPfdFromAttrib__()
{
    UINT num_formats;
    wglChoosePixelFormatARB(this->h_dc, this->pf_attribs_, NULL,
                                    1, &(this->pfd_int_), &num_formats);
    return this->pfd_int_;
}

void GLFunctionFinder::initGLInterrogationWindow() {
    if(!is_interrogator_registered) {
        window_ = RegisterClassEx(&ff_wnd_class);
        is_interrogator_registered = true;
    }
    if (window_ == 0) {
        ERR_MSGOUT("Failed to register interrogator window class");
        exit(EXIT_FAILURE);
    }
    GlobalAddr = reinterpret_cast<void *>(this);
    win_hnd = CreateWindowEx(0, // Extended options (none)
                             ff_wnd_class.lpszClassName,
                             "InterrogationWindow",
                             0,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             640,
                             480,
                             NULL,
                             NULL,
                             h_inst_,
                             NULL);
    if (!win_hnd) {
        ERR_MSGOUT_GLE("Failed create interrogator window");
        exit(EXIT_FAILURE);
    }
}

void GLFunctionFinder::runGLInterrogationWindow() {
    initGLInterrogationWindow();
    MSG msg;
    while (quitWindow != true)
    {
        PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
        // TranslateMessage(&msg); // <-- only useful if we need to process
                                   // keyboard input (protip: we don't).
        DispatchMessage(&msg);
        Sleep(0);
    }
    DestroyWindow(win_hnd);
    win_hnd = NULL;
}

bool GLFunctionFinder::setupExtensionQueryFunctions() {
    glGetStringi = reinterpret_cast<PFNGLGETSTRINGIPROC>
            (getGlAddress("glGetStringi"));
    if(glGetStringi == NULL) {
        ERR_MSGOUT_GLE("glGetStringi address retrieval failure");
        return(false);
    }
    wglGetExtensionsStringARB =
            reinterpret_cast<PFNWGLGETEXTENSIONSSTRINGARBPROC>
            (getGlAddress("wglGetExtensionsStringARB"));
    if(wglGetExtensionsStringARB == NULL) {
        ERR_MSGOUT_GLE("glGetStringi address retrieval failure");
        return(false);
    }
    if(!setupVersionInts()) {
        ERR_MSGOUT_GLE("glGetIntegerv version retrieval failure");
        return(false);
    }
    return true;
}

// Special sauce to shorten the GL function pointer code by a LOT
// Prefix GLFF means intended for use in this class / funciton only
// Gets #undef'd right after all of the calls.
#define GLFF_SETUP_GL_FUNC_ADDR_NOEXT(ptr_name__, func_name__)              \
do {                                                                        \
func_name__ = reinterpret_cast< ptr_name__ >                                \
                        (getGlAddress(#func_name__));                       \
if(func_name__ == NULL) {                                                   \
ERR_MSGOUT( #func_name__ " address retrieval failure" );                    \
return false;                                                               \
}                                                                           \
} while (0)

bool GLFunctionFinder::setupCoreGlDisplayFunctions() {
    // wglCreateContextAttribsARB
    // This is currently the only extension-dependent function at this
    // point (2-10-16).
    if(this->hasExtension("WGL_ARB_create_context_profile")) {
        GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNWGLCREATECONTEXTATTRIBSARBPROC,
                                      wglCreateContextAttribsARB);
    }
    else {
        ERR_MSGOUT("WGL_ARB_create_context_profile NOT found");
        return false;
    }
    if(this->hasExtension("WGL_ARB_pixel_format")) {
        GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNWGLCHOOSEPIXELFORMATARBPROC,
                                      wglChoosePixelFormatARB);
    }
    else {
        ERR_MSGOUT("WGL_ARB_pixel_format NOT found");
        return false;
    }
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLGENBUFFERSPROC,
                                  glGenBuffers);
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLBINDBUFFERPROC,
                                  glBindBuffer);
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLBUFFERDATAPROC,
                                  glBufferData);
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLGENVERTEXARRAYSPROC,
                                  glGenVertexArrays);
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLBINDVERTEXARRAYPROC,
                                  glBindVertexArray);
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLDELETEVERTEXARRAYSPROC,
                                  glDeleteVertexArrays);
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLENABLEVERTEXATTRIBARRAYPROC,
                                  glEnableVertexAttribArray);
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLVERTEXATTRIBPOINTERPROC,
                                  glVertexAttribPointer);
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLDISABLEVERTEXATTRIBARRAYPROC,
                                  glDisableVertexAttribArray);
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLCREATESHADERPROC,
                                  glCreateShader);
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLSHADERSOURCEPROC,
                                  glShaderSource);
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLCOMPILESHADERPROC,
                                  glCompileShader);
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLGETSHADERIVPROC,
                                  glGetShaderiv);
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLGETSHADERINFOLOGPROC,
                                  glGetShaderInfoLog);
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLATTACHSHADERPROC,
                                  glAttachShader);
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLLINKPROGRAMPROC,
                                  glLinkProgram);
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLGETPROGRAMIVPROC,
                                  glGetProgramiv);
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLDETACHSHADERPROC,
                                  glDetachShader);
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLDELETESHADERPROC,
                                  glDeleteShader);
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLUSEPROGRAMPROC,
                                  glUseProgram);
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLVERTEXATTRIB3FPROC,
                                  glVertexAttrib3f);
    GLFF_SETUP_GL_FUNC_ADDR_NOEXT(PFNGLDELETEBUFFERSPROC,
                                  glDeleteBuffers);
    return true;
}

#undef GLFF_SETUP_GL_FUNC_ADDR_NOEXT

bool GLFunctionFinder::setupVersionInts() {
    glGetIntegerv(GL_MAJOR_VERSION, &v_maj);
    glGetIntegerv(GL_MINOR_VERSION, &v_min);
    if(!v_maj || !v_min) {
        return false;
    }
    return true;
}

std::vector<std::string> GLFunctionFinder::splitStringOnNewline(
                                                std::string extension_str) {
    std::vector<std::string> list_;
    std::string tmp = "";
    for(    std::string::iterator it = extension_str.begin();
            it != extension_str.end();
            ++it) {
        if(*it == '\n') {
            if(tmp.compare("\n") != 0 and tmp.size() > 0) {
                list_.push_back(tmp);
            }
            tmp = "";
        }
        else {
            tmp += *it;
        }
    }
    if(tmp.compare("\n") != 0 and tmp.size() > 0) {
        list_.push_back(tmp);
    }
    return list_;
}

std::string GLFunctionFinder::retrieveExtensionString(HDC context) {
    int num_extentions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &num_extentions);
    std::string extension_str;
    for(int i = 0; i < num_extentions; i++) {
        extension_str += reinterpret_cast<const char *>
                (glGetStringi(GL_EXTENSIONS, i));
        extension_str += "\n";
    }
    extension_str += wglGetExtensionsStringARB(context);
    std::replace(extension_str.begin(), extension_str.end(), ' ', '\n');
    return extension_str;
}

bool GLFunctionFinder::saveExtensionListToFile(std::string filename) {
    // Send to a file for review.
    std::ofstream outfile(filename.c_str());
    if (! (outfile)) {
        ERR_MSGOUT("Unable to open file in saveExtensionListToFile");
        return false;
    }
    for(    std::vector<std::string>::iterator it = extension_list.begin();
            it != extension_list.end();
            ++it) {
        outfile << *it << std::endl;
    }
    outfile.close();
    return true;
}

bool GLFunctionFinder::setupExtensionStringVector(HDC context) {
    // Set up the extension string getter functions.
    if(!setupExtensionQueryFunctions()) {
        return(false);
    }
    // Get ARB and WGL extension list, \n-delimeted
    std::string extension_str = retrieveExtensionString(context);
    // Split the string
    extension_list = splitStringOnNewline(extension_str);

}

LRESULT GLFunctionFinder::InitWndProc(HWND h_wnd,
                                      UINT u_msg,
                                      WPARAM w_param,
                                      LPARAM l_param) {
    // Start by making a class-like "_this" pointer to use to call class
    // methods. Because I made this a static member function of
    // GLFunctionFinder, private methods and variables can be called.
    // Stupid, but it works.
    GLFunctionFinder * _this
            = reinterpret_cast<GLFunctionFinder *>(GlobalAddr);
    switch(u_msg) {
    case WM_CREATE: {
        _this->setupDC(h_wnd);
        _this->setupContext();
        switch(_this->initwnd_action) {
        case INIT:
            if(! _this->is_gl_inited) {
                _this->initGL();
                _this->is_gl_inited = true;
            }
            break;

        case GET_PFD:
            _this->getPfdFromAttrib__();
            break;
        }
        _this->removeContext();
        _this->removeDC(h_wnd);
        _this->setQuitInitWindow();
        break;
    }

    case WM_CLOSE:
        _this->setQuitInitWindow();
        break;

    case WM_DESTROY:


    default:
        return DefWindowProc(h_wnd, u_msg, w_param, l_param);
    }

    return(1);
}

GLFunctionFinder::GLFunctionFinder(HINSTANCE h_inst,
                                   int cmd_show_opt) :
        h_inst_(h_inst),
        cmd_show_opt_(cmd_show_opt),
        quitWindow(false),
        v_maj(0),
        v_min(0),
        is_interrogator_registered(false),
        is_gl_inited(false) {
    ff_wnd_class.cbSize         = sizeof(WNDCLASSEX);
    ff_wnd_class.style          = CS_OWNDC;
    ff_wnd_class.lpfnWndProc    = InitWndProc;
    ff_wnd_class.cbClsExtra     = 0;
    ff_wnd_class.cbWndExtra     = 0;
    ff_wnd_class.hInstance      = h_inst_;
    ff_wnd_class.hIcon          = NULL;
    ff_wnd_class.hCursor        = LoadCursor(NULL, IDC_ARROW);
    ff_wnd_class.hbrBackground  = (HBRUSH)(COLOR_BACKGROUND);
    ff_wnd_class.lpszMenuName   = NULL;
    ff_wnd_class.lpszClassName  = "INTERROGATOR_CLASS";
    ff_wnd_class.hIconSm        = NULL;
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        24,
        8,
        0,
        PFD_MAIN_PLANE,
        0,
        0,
        0,
        0
    };
    pfdarr_ = pfd; // <-- world's stupidest bug was forgetting this line
    if (GlobalAddr != NULL) {
        throw new std::logic_error("Double initialization of "
                                   "GLFunctionFinder");
    }
}

GLFunctionFinder::~GLFunctionFinder() {
    GlobalAddr = NULL;
}

void GLFunctionFinder::interrogateWindows() {
    initwnd_action = INIT;
    runGLInterrogationWindow();
}

bool GLFunctionFinder::hasExtension(std::string extension) {
    for(    std::vector<std::string>::iterator it = extension_list.begin();
            it != extension_list.end();
            ++it) {
        if(extension.compare(*it) == 0) {
            return true;
        }
    }
    return false;
}

int GLFunctionFinder::getPFDFromAttrib(int *pf_attribs)
{
    pfd_int_ = -1;
    pf_attribs_ = pf_attribs;
    initwnd_action = GET_PFD;
    runGLInterrogationWindow();
    pf_attribs_ = NULL;
    if(pfd_int_ == -1) {
        ERR_MSGOUT("GLFF didn't get pf int");
        pfd_int_ = 0;
    }
    return pfd_int_;
}
