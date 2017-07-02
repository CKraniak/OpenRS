#ifndef PLAYERMOVEMENTCESYSTEM_H
#define PLAYERMOVEMENTCESYSTEM_H

#include "../cesystem.h"

const int DEFAULT_START_X = 7;
const int DEFAULT_START_Y = 7;

// THE SYSTEM CONNECTION IS TEMPORARY, DO NOT RELY ON IT
#include "asciidisplaycesystem.h"

class PlayerMovementCESystem : public ScriptedCESystem {
    int pos_x;
    int pos_y;
public:
    PlayerMovementCESystem(int x = DEFAULT_START_X,
                           int y = DEFAULT_START_Y) :
        pos_x(x),
        pos_y(y) {}
    int onNumpad(char , AsciiDisplayCESystem &); // Apply a transform to the
                         // player object
    void onDispatcherAvailable();
};

#endif // PLAYERMOVEMENTCESYSTEM_H
