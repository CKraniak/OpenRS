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

#ifndef GAMEGRIDCESYSTEM_H
#define GAMEGRIDCESYSTEM_H

#include "../cesystem.h"
#include "../entity.h"

// Note that in the long run the actual grid needs to be an entity.
// Also, other classes don't need to know shit, so the implementation is in
// the cpp file.
struct GameGrid;

class GameGridCESystem : public CESystem
{

public:
    std::shared_ptr<GameGrid> grid_;
    GameGridCESystem();
    void onDispatcherAvailable();
    void onECManagerAvailable();
};

#endif // GAMEGRIDCESYSTEM_H
