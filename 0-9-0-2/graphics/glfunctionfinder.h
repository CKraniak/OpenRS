#ifndef GLFUNCTIONFINDER_H
#define GLFUNCTIONFINDER_H

#include <vector>
#include <string>

#include <windows.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>

#include "../utility.h"

#include <fstream>

// ***********************************************************************

extern PFNGLGETSTRINGIPROC                 glGetStringi;
extern PFNWGLGETEXTENSIONSSTRINGARBPROC    wglGetExtensionsStringARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC   wglCreateContextAttribsARB;
extern PFNWGLCHOOSEPIXELFORMATARBPROC      wglChoosePixelFormatARB;
extern PFNGLGENBUFFERSPROC                 glGenBuffers;
extern PFNGLBINDBUFFERPROC                 glBindBuffer;
extern PFNGLBUFFERDATAPROC                 glBufferData;
extern PFNGLGENVERTEXARRAYSPROC            glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC            glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC         glDeleteVertexArrays;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC        glVertexAttribPointer;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC   glDisableVertexAttribArray;
extern PFNGLCREATESHADERPROC               glCreateShader;
extern PFNGLSHADERSOURCEPROC               glShaderSource;
extern PFNGLCOMPILESHADERPROC              glCompileShader;
extern PFNGLGETSHADERIVPROC                glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC           glGetShaderInfoLog;
extern PFNGLATTACHSHADERPROC               glAttachShader;
extern PFNGLLINKPROGRAMPROC                glLinkProgram;
extern PFNGLGETPROGRAMIVPROC               glGetProgramiv;
extern PFNGLDETACHSHADERPROC               glDetachShader;
extern PFNGLDELETESHADERPROC               glDeleteShader;
extern PFNGLUSEPROGRAMPROC                 glUseProgram;
extern PFNGLVERTEXATTRIB3FPROC             glVertexAttrib3f;
extern PFNGLDELETEBUFFERSPROC              glDeleteBuffers;

// ***********************************************************************

// Does something like GLEW in that it sets all the function pointers up.
class GLFunctionFinder {
private:
    HINSTANCE h_inst_;
    int cmd_show_opt_;
    WNDCLASSEX ff_wnd_class;
    ATOM window_;
    HWND win_hnd;
    bool quitWindow;
    std::vector<std::string> extension_list;
    int v_maj, v_min;
    HDC h_dc;
    HGLRC h_context;
    int pfd_int_;
    int * pf_attribs_;
    PIXELFORMATDESCRIPTOR pfdarr_;
    typedef enum {
        INIT,
        GET_PFD
    } IW_ACTION;
    IW_ACTION initwnd_action;
    bool is_interrogator_registered;
    bool is_gl_inited;

    int setupDC(HWND h_wnd);
    int setupContext();
    int removeDC(HWND h_wnd);
    int removeContext();

    int initGL();
    int getPfdFromAttrib__();

    // Create the GL window used to query the driver
    // Query type is controlled by initwnd_action enum value
    void initGLInterrogationWindow();

    // Window main "loop".
    // Should not need to run very long.
    void runGLInterrogationWindow();

    bool setupExtensionQueryFunctions();

    // This should only be called after the extension vector is set.
    bool setupCoreGlDisplayFunctions();

    // Use more basic OGL functions to get initial version information.
    // This happens before any function addresses are retrieved, so there's
    // not a good way of doing this besides what's here (that I know of).
    bool setupVersionInts();

    std::vector<std::string> splitStringOnNewline(std::string extension_str);

    // This function must be run AFTER setupExtensionQueryFunctions(), since it
    // uses the extension query functions.
    std::string retrieveExtensionString(HDC context);

    bool saveExtensionListToFile(std::string filename);

    // This is only public because I have to run this in InitWndProc.
    // It should never be called by anyone besides the class or InitWndProc.
    bool setupExtensionStringVector(HDC context);

    HWND getWinHnd() { return win_hnd; }
    int getCmdShowOpt() { return cmd_show_opt_; }

    // Needs to be static so the implicit "this" isn't in the signature.
    static LRESULT CALLBACK InitWndProc(HWND, UINT, WPARAM, LPARAM);

public:
    GLFunctionFinder(HINSTANCE h_inst, int cmd_show_opt);
    ~GLFunctionFinder();

    // Perform the interrogation of the OS for extensions, version, etc.
    void interrogateWindows();
    // Query the list of extensions to see if the given extension is supported.
    bool hasExtension(std::string extension);
    std::vector<std::string> getExtensionList() { return extension_list; }
    int majorVersion() { return v_maj; }
    int minorVersion() { return v_min; }
    // Function that can be used to break out of the window's main loop.
    void setQuitInitWindow(bool val = true) { quitWindow = val; }
    int getPFDFromAttrib(int * pf_attribs);
};

// ***********************************************************************

// static


#endif // GLFUNCTIONFINDER_H
