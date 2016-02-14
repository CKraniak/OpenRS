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
