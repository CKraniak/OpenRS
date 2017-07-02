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
