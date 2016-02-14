#ifndef PLAYERMOVEMENTCESYSTEM_H
#define PLAYERMOVEMENTCESYSTEM_H

#include "../cesystem.h"

// THE SYSTEM CONNECTION IS TEMPORARY, DO NOT RELY ON IT
#include "asciidisplaycesystem.h"

class PlayerMovementCESystem : public ScriptedCESystem {
    int pos_x;
    int pos_y;
public:
    PlayerMovementCESystem(int x, int y) : pos_x(x), pos_y(y) {}
    int onNumpad(char , AsciiDisplayCESystem &); // Apply a transform to the
                         // player object
};

#endif // PLAYERMOVEMENTCESYSTEM_H
