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

#ifndef MLINTERFACE_H
#define MLINTERFACE_H

// "Main Loop Interface" class
//
// Does more or less what it says, and provides hooks into functionality that
// openrs_main needs. It also does a huge chunk of interfacing to OS-specific
// calls in subclasses, which are automatically linked in (in the .cpp) as
// appropriate.
//
// JOBS:
//  - Interface to window creation routines
//  - Interface to OS events, which in Windows is tied to the window creation
//    routine to a large degree
//      - Possible TODO: put OS event handling in the dispatcher / event
//        handler's directory?
//  - Interface to rendering / drawing routines, which is supposed to be
//    implemented in the graphics directory but currently isn't going there
//      - TODO: move all drawing to the graphics directory, link into here
//        with a pointer or somesuch

#include <memory>

// Declaration of the entry point to OpenRS.
//
// The subclasses of MLInterface are where the true entry points are, but those
// are expected to only do OS-specific thing to set up for the intended entry
// point, and then call it.
int openrs_main(int argc, char** argv);

class StatefulDispatcher;

class MLInterface {
    bool quit_;

protected:
    // There needs to be a way to propogate OS events to the game's event
    // system.
    //
    // Note: since it's not new'd, it doesn't need a smart pointer.
    StatefulDispatcher * disp_;
    virtual void init_() {}

public:
    MLInterface(StatefulDispatcher * disp) :
        quit_(false),
        disp_(disp) {}
    virtual ~MLInterface() = default;
    // This is the OS-specific selector
    static std::unique_ptr<MLInterface> getInterface(StatefulDispatcher *);

    void setQuit(bool q = true) { this->quit_ = q; }
    bool shouldQuit() { return this->quit_; }

    virtual void emptyMessagePump() {}
    virtual bool createMainWindow() {}
};

#endif // MLINTERFACE_H
