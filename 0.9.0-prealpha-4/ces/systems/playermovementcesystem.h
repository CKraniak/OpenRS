/* Copyright (C) 2017, Chesley Kraniak
 *
 * This code is distributed under the terms of the GPLv3 license, which should
 * have been included with this file in the root directory as "LICENSE.txt". If
 * you do not have a copy of the license, see:
 *     http://www.gnu.org/licenses/gpl-3.0.txt
 * or:
 *     https://github.com/CKraniak/OpenRS
 * for the license.
 */

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
