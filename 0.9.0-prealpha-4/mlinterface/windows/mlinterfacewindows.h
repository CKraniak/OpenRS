#ifndef MLINTERFACEWINDOWS_H
#define MLINTERFACEWINDOWS_H

#include <windows.h>

#include "../mlinterface.h"
#include "../../ces/ces.h"

// Class to give main the interface it needs to other pieces.
class MLInterfaceWindows : public MLInterface {
    HWND h_wnd_;
    ENUMLOGFONTEXDV * ptr_elf;
    std::string display_text;
    HFONT font, old_font;
    AsciiDisplayCESystem adces;
    PlayerMovementCESystem pmces;
    HINSTANCE h_inst;
    int cmd_show_opt;

    static LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

    static int WINAPI WinMain_(HINSTANCE hi, HINSTANCE phi, LPSTR argv, int cso);
    friend int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

    bool winCreateMainWindow(HINSTANCE, int);

    void asciiDraw(HDC);
    void clearWindow(HDC, RECT &);
    void setFont(HDC);
    void unsetFont(HDC);

    void setThisAsGlobalAddr();
    void init_() {
        setThisAsGlobalAddr();
    }

public:
    MLInterfaceWindows(StatefulDispatcher *);
    void update();
    void render();
    void emptyMessagePump();
    bool createMainWindow() {
        winCreateMainWindow(h_inst, cmd_show_opt);
    }
};

#endif // MLINTERFACEWINDOWS_H
