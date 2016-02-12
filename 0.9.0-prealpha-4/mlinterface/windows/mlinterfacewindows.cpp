#include "mlinterfacewindows.h"
#include "../../utility.h"

static void * global_addr = nullptr;

static HINSTANCE win_h_inst; // The HINST passed in via WinMain
static int win_cmd_show_opt; // The cmd_show_opt passed in via WinMain

const int GAME_GRID_HEIGHT = 15;
const int GAME_GRID_WIDTH  = 15;

const int FIXED_WINDOW_HEIGHT = 600;
const int FIXED_WINDOW_WIDTH  = 800;

void MLInterfaceWindows::emptyMessagePump() {
    MSG msg;
    while(PeekMessage(&msg, h_wnd_, 0, 0, PM_REMOVE)) {
        if(msg.message == WM_DESTROY) {
            setQuit();
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

bool MLInterfaceWindows::winCreateMainWindow(HINSTANCE h_inst,
                                   int cmd_show_opt) {
    WNDCLASSEX wc;
    wc.cbSize           = sizeof(WNDCLASSEX);
    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = MLInterfaceWindows::MainWndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = h_inst;
    wc.hIcon            = NULL;
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
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
                                WS_OVERLAPPED | WS_CAPTION, // no resize
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                FIXED_WINDOW_WIDTH, FIXED_WINDOW_HEIGHT,
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

void MLInterfaceWindows::asciiDraw(HDC dc) {
    RECT r;
    clearWindow(dc, r);
    setFont(dc);
    std::vector<char> new_text = adces.getRenderData();
    std::string new_text_str(new_text.begin(), new_text.end());
    display_text = new_text_str;
    DrawText(dc, display_text.c_str(), display_text.size(), &r, DT_NOCLIP);
    unsetFont(dc);
}

void MLInterfaceWindows::clearWindow(HDC dc, RECT &r) {
    GetClientRect(WindowFromDC(dc), &r);
    FillRect(dc, &r, (HBRUSH)(COLOR_WINDOW + 1));
}

void MLInterfaceWindows::setFont(HDC dc) {
    // The font only needs to be found / selected once, not every frame.
    if(font == NULL) {
        LOGFONT lf;
        memset(&lf, 0, sizeof(lf));
        strcpy_s(lf.lfFaceName, "Courier New");
        lf.lfHeight = -24;
        lf.lfWeight = FW_HEAVY;
        font = CreateFontIndirect(&lf);
        if(font == NULL) {
            // If this doesn't work, it *could* need to be HFONT* instead.
            font = static_cast<HFONT>(GetStockObject(ANSI_FIXED_FONT));
        }
    }
    // Again, should only need to get old_font once
    // Also, to get it, you have to set the font. If we've already gotten
    // old_font, skip setting old_font and just select the monospace font.
    if(old_font == NULL) {
        old_font = static_cast<HFONT>(SelectObject(dc, font));
        if (old_font == NULL) {
            ERR_MSGOUT("Failed to select font");
        }
    }
    else {
        SelectObject(dc, font);
    }
}

void MLInterfaceWindows::unsetFont(HDC dc) {
    if(old_font != NULL) {
        SelectObject(dc, old_font);
    }
}

MLInterfaceWindows::MLInterfaceWindows() :
        MLInterface(),
        display_text("INIT"),
        font(NULL),
        adces(GAME_GRID_WIDTH, GAME_GRID_HEIGHT),
        pmces(GAME_GRID_WIDTH / 2, GAME_GRID_HEIGHT / 2),
        old_font(NULL){
    h_inst = win_h_inst;
    cmd_show_opt = win_cmd_show_opt;
}

void MLInterfaceWindows::setThisAsGlobalAddr()
{
    global_addr = this;
}

LRESULT CALLBACK MLInterfaceWindows::MainWndProc(HWND h_wnd,
                                                 UINT msg,
                                                 WPARAM w_param,
                                                 LPARAM l_param) {
    MLInterfaceWindows * this_ = static_cast<MLInterfaceWindows *>(global_addr);

    switch (msg) {
    case WM_CREATE:
        // Lol
        return 0;

    case WM_KEYDOWN:
        switch (w_param) {
        // InvalidateRect moved up so the window won't flash when I push a
        // key that isn't numpad 1-9.
        case VK_NUMPAD1:
            this_->pmces.onNumpad('1', this_->adces);
            InvalidateRect(h_wnd, NULL, TRUE);
            break;
        case VK_NUMPAD2:
            this_->pmces.onNumpad('2', this_->adces);
            InvalidateRect(h_wnd, NULL, TRUE);
            break;
        case VK_NUMPAD3:
            this_->pmces.onNumpad('3', this_->adces);
            InvalidateRect(h_wnd, NULL, TRUE);
            break;
        case VK_NUMPAD4:
            this_->pmces.onNumpad('4', this_->adces);
            InvalidateRect(h_wnd, NULL, TRUE);
            break;
        case VK_NUMPAD5:
            this_->pmces.onNumpad('5', this_->adces);
            InvalidateRect(h_wnd, NULL, TRUE);
            break;
        case VK_NUMPAD6:
            this_->pmces.onNumpad('6', this_->adces);
            InvalidateRect(h_wnd, NULL, TRUE);
            break;
        case VK_NUMPAD7:
            this_->pmces.onNumpad('7', this_->adces);
            InvalidateRect(h_wnd, NULL, TRUE);
            break;
        case VK_NUMPAD8:
            this_->pmces.onNumpad('8', this_->adces);
            InvalidateRect(h_wnd, NULL, TRUE);
            break;
        case VK_NUMPAD9:
            this_->pmces.onNumpad('9', this_->adces);
            InvalidateRect(h_wnd, NULL, TRUE);
            break;

        default:
            break;
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(h_wnd, &ps);
        if(dc == NULL) {
            ERR_MSGOUT("Unable to get device context");
            return -1;
        }
        this_->asciiDraw(dc);
        EndPaint(h_wnd, &ps);
        return 0;
    }

    case WM_ERASEBKGND:
        return 1;

    case WM_PRINTCLIENT: {
        this_->asciiDraw(reinterpret_cast<HDC>(w_param));
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
        this_->setQuit();
        break;

    default:
        return DefWindowProc(h_wnd, msg, w_param, l_param);
    }

    return (0);
}

int MLInterfaceWindows::WinMain_(HINSTANCE hi, HINSTANCE phi, LPSTR argv, int cso){
    win_h_inst = hi;
    win_cmd_show_opt = cso;
    openrs_main(0, nullptr);
}

int WINAPI WinMain(HINSTANCE hi, HINSTANCE phi, LPSTR argv, int cso) {
    MLInterfaceWindows::WinMain_(hi, phi, argv, cso);
}
