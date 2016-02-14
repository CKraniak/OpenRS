#ifndef GRAPHICSDRIVER_H
#define GRAPHICSDRIVER_H

// Interface to system-specific graphics calls
//
// The implementers sub-class from this and implement the virtual functions

class GraphicsDriver
{
public:
    GraphicsDriver();

    // I don't know what I need yet.
    virtual drawGrid() {}
    virtual drawTextLine() {}
};

#endif // GRAPHICSDRIVER_H
