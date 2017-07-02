#ifndef ASCIIRENDERER_H
#define ASCIIRENDERER_H

#include <memory>

#include "drivers/graphicsdriver.h"
#include "asciicore.h"

// Thinking of this as a window manager class.

// *cores are for low level drawing stuff. A renderer would be assigned to a
// window and keep track of *core objects that are supposed to be rendered in
// a particular viewport. I think.

// So *cores are what holds renderable data and some render state information.
// And the renderer itself interfaces to the OS / window system.

// In the threaded version, you'd have the renderer outputting to a screen as
// the *core is updated in a different thread (3-core update buffering system).

class AsciiRenderer
{
    std::unique_ptr<GraphicsDriver> driver_ptr_;
    std::shared_ptr<AsciiCore> core_data_;

public:
    AsciiRenderer();
    setAsciiCore(std::shared_ptr<AsciiCore> core);

    void render();
};

#endif // ASCIIRENDERER_H
