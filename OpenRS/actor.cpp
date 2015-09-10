/** Actor class represents a thing that "acts" in any way
  * in the game world. This could be a player character,
  * NPC, enemy, animal, etc.
  */

#include "actor.h"

Actor::Actor(int x, int y,
             ActorType type,
             Dispatcher * disp,
             District * parent) :
    Item(x, y, IT_ACTOR, disp, parent) {

}
