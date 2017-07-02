#include "collisioncesystem.h"

GE_HND(collision_on_collide, char, "on_collide", {

})

GE_HND(collision_on_found_collision_entity, Entity, "on_found_collision_entity", {
    auto this_ = reinterpret_cast<CollisionCESystem *>(that);
    this_->setCollisionTarget(input);
    return 0;
})

// Note that the move request properly belongs in GameGrid's control.
// A collision handler isn't meant to find collisions, only process them.
//
//GE_HND(collision_on_move_request, char, "on_request_move", {
//    INFO_MSGOUT("In Collision System");
//    return 0; // No collision detector implemented yet
//    /* How it will eventually work:
//     * - Check for presence of movable tile (call to the map / gamegrid)
//     * -
//     */
//})

void CollisionCESystem::onDispatcherAvailable() {
    // disp_->registerHandler<char>(collision_on_move_request);
}

CollisionCESystem::CollisionCESystem()
{

}
