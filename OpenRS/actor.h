#ifndef ACTOR_H
#define ACTOR_H

enum ActorType {
    AT_PLAYER_CHARACTER,
    AT_NPC,
    AT_ENEMY,
    AT_NONE
};

//#include <windows.h>
#include "item.h"

class Actor : public Item
{
private:
    //char display_symbol;
    // Will eventually need a way to track actions.
    // Probably a behavior dispatcher, or maybe an AI object.
    static void onDistrictUpdate(void * _this,
                                 int argc,
                                 std::vector<std::string *> argv) {
        ((Actor *)_this)->update();
    }

protected:
    // Call to ask Actor to move / act in the district.
    // Virtual SHOULD make it bind to the child's reimplementation.
    virtual void update() = 0;

public:
    Actor(int x, int y,
          ActorType type,
          Dispatcher * dispatcher,
          District * parent);
};

#endif // ACTOR_H
