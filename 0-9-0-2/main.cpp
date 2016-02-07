#include <iostream>
#include <sstream>

#include <windows.h>
#include "strsafe.h"

#include "utility.h"
#include "dispatcher/dispatcher.h"

// Put this AFTER all includes
// Can't pollute other files if other files never see this crap.
static void * global_addr = nullptr;

// Class to give main the interface it needs to other pieces.
class MLInterface {
    bool quit_;
    HWND h_wnd_;
    ENUMLOGFONTEXDV * ptr_elf;
    std::string display_text;

public:
    MLInterface() : quit_(false), display_text("INIT") {}
    void update();
    void render();
    static LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
    void setQuit(bool q = true) { quit_ = q; }
    bool shouldQuit() { return quit_; }
    void emptyMessagePump();
    bool createMainWindow(HINSTANCE, int);
    void asciiDraw(HDC);
};

void MLInterface::emptyMessagePump() {
    MSG msg;
    while(PeekMessage(&msg, h_wnd_, 0, 0, PM_REMOVE)) {
        if(msg.message == WM_DESTROY) {
            setQuit();
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

bool MLInterface::createMainWindow(HINSTANCE h_inst,
                                   int cmd_show_opt) {
    WNDCLASSEX wc;
    wc.cbSize           = sizeof(WNDCLASSEX);
    wc.style            = 0;
    wc.lpfnWndProc      = MLInterface::MainWndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = h_inst;
    wc.hIcon            = NULL;
    wc.hCursor          = NULL;
    wc.hbrBackground    = NULL;
    wc.lpszClassName    = "ORS_MAIN_WINDOW_CLASS";
    wc.lpszMenuName     = NULL;
    wc.hIconSm          = NULL;
    ATOM a = RegisterClassEx(&wc);
    if(a == 0) {
        ERR_MSGOUT_GLE("MLInterface::createMainWindow RegisterClassEx failed");
        return false;
    }
    HWND h_wnd = CreateWindowEx(0,
                                "ORS_MAIN_WINDOW_CLASS",
                                "Test",
                                WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,// no resize
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                640, 480,
                                NULL, NULL,
                                h_inst,
                                NULL);
    if(h_wnd == NULL) {
        ERR_MSGOUT("MLInterface::createMainWindow CreateWindowEx failed");
        return false;
    }
    // The return value of ShowWindow does not have meaning here.
    ShowWindow(h_wnd, cmd_show_opt);
    if (! UpdateWindow(h_wnd)) {
        ERR_MSGOUT("MLInterface::createMainWindow UpdateWindow failed");
        return false;
    }
    h_wnd_ = h_wnd;
    return true;
}

void MLInterface::asciiDraw(HDC dc) {
    if(dc == NULL) {
        ERR_MSGOUT("Unable to get device context");
        return;
    }
    RECT r;
    GetClientRect(WindowFromDC(dc), &r);

    FillRect(dc, &r, (HBRUSH)(COLOR_WINDOW + 1));

    LOGFONT lf; // <-- parentheses means actually initialize, I believe.
    memset(&lf, 0, sizeof(lf)); // but set the bitch anyway.
//        lf.lfWeight = 900; // BOLD
    strcpy_s(lf.lfFaceName, "Courier New");
    lf.lfHeight = -24;
    lf.lfWeight = FW_HEAVY;
    HFONT test_font = CreateFontIndirect(&lf);
    HFONT font;
    if(test_font == NULL) {
        font = *static_cast<HFONT*>(GetStockObject(ANSI_FIXED_FONT));
    }
    else {
        font = test_font;
    }

    // Switch to Courier-style monospaced font
    HFONT old_font = static_cast<HFONT>(SelectObject(dc, font));
    if (old_font == NULL) {
        ERR_MSGOUT("Failed to select font");
    }
//    std::string s = "TESTING FONT, Courier New 012345 ilI1\niiiiiiiiiiiiii";
    std::string s = this->display_text;
    DrawText(dc, s.c_str(), s.size(), &r, DT_NOCLIP);
    SelectObject(dc, old_font);
}

LRESULT CALLBACK MLInterface::MainWndProc(HWND h_wnd,
                                          UINT msg,
                                          WPARAM w_param,
                                          LPARAM l_param) {
    MLInterface * this_ = static_cast<MLInterface *>(global_addr);

    switch (msg) {
    case WM_CREATE:
        return 0;

    case WM_KEYDOWN:
        switch (w_param) {
        case VK_NUMPAD1:
            this_->display_text = "1";
            break;
        case VK_NUMPAD2:
            this_->display_text = "2";
            break;
        case VK_NUMPAD3:
            this_->display_text = "3";
            break;
        case VK_NUMPAD4:
            this_->display_text = "4";
            break;
        case VK_NUMPAD5:
            this_->display_text = "5";
            break;
        case VK_NUMPAD6:
            this_->display_text = "6";
            break;
        case VK_NUMPAD7:
            this_->display_text = "7";
            break;
        case VK_NUMPAD8:
            this_->display_text = "8";
            break;
        case VK_NUMPAD9:
            this_->display_text = "9";
            break;

        default:
            break;
        }
        InvalidateRect(h_wnd, NULL, TRUE);
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(h_wnd, &ps);
        this_->asciiDraw(dc);
        EndPaint(h_wnd, &ps);
        return 0;
    }

    case WM_PRINTCLIENT: {
        this_->asciiDraw(reinterpret_cast<HDC>(w_param));
    }

//    case WM_ERASEBKGND: {
//        this_->asciiDraw(reinterpret_cast<HDC>(w_param));
//    }


    case WM_CHAR:
        if(w_param == VK_ESCAPE) {
            PostMessage(h_wnd, WM_CLOSE, 0, 0);
        }
        break;

    case WM_CLOSE:
        DestroyWindow(h_wnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(EXIT_SUCCESS);
        this_->setQuit();
        break;

    default:
        return DefWindowProc(h_wnd, msg, w_param, l_param);
    }

    return (0);
}

int WINAPI WinMain(HINSTANCE h_inst,
                   HINSTANCE unused_1,
                   LPSTR cmd_args,
                   int cmd_show_opt)
{
    MLInterface mli;
    global_addr = &mli;
    mli.createMainWindow(h_inst, cmd_show_opt);
    while ( ! mli.shouldQuit() ) {
        // main loop:
        //  - look for input, process if available
        mli.emptyMessagePump();
        //  - update
        //  - render
    }
    return 0;
}

