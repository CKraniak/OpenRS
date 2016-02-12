#ifndef MLINTERFACE_H
#define MLINTERFACE_H

// "Main Loop Interface" class
//
// Does more or less what it says, and provides hooks into functionality that
// openrs_main needs. It also does a huge chunk of interfacing to OS-specific
// calls in subclasses, which are automatically linked in (in the .cpp) as
// appropriate.

#include <memory>

// Declaration of the entry point to OpenRS.
//
// The subclasses of MLInterface are where the true entry points are, but those
// are expected to only do OS-specific thing to set up for the intended entry
// point, and then call it.
int openrs_main(int argc, char** argv);

class MLInterface {
    bool quit_;

protected:
    virtual void init_() {}

public:
    MLInterface() : quit_(false) {}
    virtual ~MLInterface() = default;
    // This is the OS-specific selector
    static std::unique_ptr<MLInterface> getInterface();

    void setQuit(bool q = true) { quit_ = q; }
    bool shouldQuit() { return quit_; }

    virtual void emptyMessagePump() {}
    virtual bool createMainWindow() {}
};

#endif // MLINTERFACE_H
