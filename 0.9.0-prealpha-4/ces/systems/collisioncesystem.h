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

#ifndef COLLISIONCESYSTEM_H
#define COLLISIONCESYSTEM_H

#include "../cesystem.h"
#include "../entity.h"

class CollisionCESystem : public CESystem {
    Entity collision_target_;

public:
    CollisionCESystem();
    void setCollisionTarget(Entity e) { collision_target_ = e; }
    void onDispatcherAvailable();
};

#endif // COLLISIONCESYSTEM_H
