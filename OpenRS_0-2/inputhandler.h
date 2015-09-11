#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "observer.h"

enum InputType {
    IH_IT_ASCII_CIN = 1,
    IH_IT_GLFW3,
    IH_IT_WIN32
};

class InputHandler
{
public:
    InputHandler();
    InputHandler(InputType, Observer&);

private:
    Observer observer_;
};

#endif // INPUTHANDLER_H
