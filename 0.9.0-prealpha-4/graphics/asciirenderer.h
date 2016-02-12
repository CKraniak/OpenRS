#ifndef ASCIIRENDERER_H
#define ASCIIRENDERER_H

#include <memory>

#include "drivers/graphicsdriver.h"

class AsciiRenderer
{
    std::unique_ptr<GraphicsDriver> driver_ptr_;

public:
    AsciiRenderer();
};

#endif // ASCIIRENDERER_H
