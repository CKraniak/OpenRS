#include <iostream>

#include <windows.h>

#include "utility.h"

bool quit;

//#include "dispatcher/dispatcher.h"

// Class to give main the interface it needs to other pieces.
//struct InputData;

//class MLInterface {
//    InputData data;

//public:
//    typedef enum {
//        RECEIVED,
//        EXIT = 0,
//        NONE
//    } INPUT_STATUS;

//    void update();
//    void render();
//    INPUT_STATUS checkForInput();
//    void dispatchInput(InputData data);
//};

LRESULT CALLBACK MainWndProc(HWND h_wnd,
                             UINT msg,
                             WPARAM w_param,
                             LPARAM l_param) {
    switch (msg) {
    case WM_CREATE:
        return 0;

    case WM_KEYDOWN:
        switch (w_param) {
        case VK_NUMPAD1:
        case VK_NUMPAD2:
        case VK_NUMPAD3:
        case VK_NUMPAD4:
        case VK_NUMPAD5:
        case VK_NUMPAD6:
        case VK_NUMPAD7:
        case VK_NUMPAD8:
        case VK_NUMPAD9:
//            std::cout << "Numpad " <<
//                         w_param - VK_NUMPAD0 <<
//                         " pressed" <<
//                         std::endl;
            break;

        default:
            break;
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(h_wnd, &ps);
        FillRect(dc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(h_wnd, &ps);
        ReleaseDC(h_wnd, dc);
        return 0;
    }


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
        quit = true;
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
    WNDCLASSEX wc;
    wc.cbSize           = sizeof(WNDCLASSEX);
    wc.style            = 0;
    wc.lpfnWndProc      = MainWndProc;
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
    HWND h_wnd = CreateWindowEx(0,
                                "ORS_MAIN_WINDOW_CLASS",
                                "Test",
                                WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                NULL, NULL,
                                h_inst,
                                NULL);

    if(h_wnd == NULL) {
        ERR_MSGOUT("HWND is null");
    }

    ERR_MSGOUT("HWND is OK");

    ShowWindow(h_wnd, cmd_show_opt);
    UpdateWindow(h_wnd);

    // main pseudocode:
    // Configuration obj. should be loaded
    // SystemGroup should be made
    quit = false;
    MSG msg;
    while ( ! quit ) {
        if(PeekMessage(&msg, h_wnd, 0, 0, true)) {
            if(msg.message == WM_DESTROY) {
                quit = true;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        // main loop:
        //  - look for input, process if available
        //  - update
        //  - render
    }
    return 0;
}

