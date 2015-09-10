#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <map>

class Item;

// Note that Player Character is not an Actor
// An actor will have an AI; a Player Character should not need an AI
// That would be wierd. Except when hunger / sleep behaviors take over, so
// maybe they will have some AI behaviors then. Hmm.
enum ItemType {
    IT_ACTOR,
    IT_PLAYER_CHARACTER,
    IT_FURNITURE,
    //IT_INVENTORABLE,
    IT_NONE
};

// localgrid.h introduces a nasty circular dependency.
// Leave it out if at all possible.
//#include "localgrid.h"
#include "dispatcher.h"
//#include "district.h"

class District;

// The behavior_dispatcher is basically a runtime function binding
// mechanism for subclasses which can be queried by a superclass method.
class Item
{
private:
    int _x;
    int _y;
    ItemType itype;
    Dispatcher * mainloop_dispatcher;
    Dispatcher behavior_dispatcher;
    std::map<std::string *, int> behavior_registry;

protected:
    char display_character;

protected:
    District * _parent;
    void pushBehavior(CALLBACK_PTR func,
                      CommandDescriptor * descriptor,
                      void * _this = NULL);

public:
    Item(int x, int y,
         ItemType type,
         Dispatcher * dispatcher,
         District * parent);
    //class LocalGrid * getLocalGrid();
    inline char getAsciiChar() { return display_character; }
    void getPosition(int *x, int *y) { *x = _x; *y = _y; }
    void setPosition(int x, int y) { _x = x; _y = y; }
    bool hasBehavior(std::string behavior_name);
};

#endif // ITEM_H
