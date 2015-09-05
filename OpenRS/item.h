#ifndef ITEM_H
#define ITEM_H

#include <iostream>

#include "localgrid.h"
#include "dispatcher.h"
#include "district.h"

enum ItemType {
    IT_CREATURE,
    IT_PLAYER_CHARACTER,
    IT_FURNITURE
};

class Item
{
private:
    int _x;
    int _y;
    char display_character;
    ItemType itype;
    Dispatcher * mainloop_dispatcher;

protected:
    District * _parent;

public:
    Item(int x, int y,
         ItemType type,
         Dispatcher * dispatcher,
         District * parent);
    //class LocalGrid * getLocalGrid();
    inline char getAsciiChar() { return display_character; }
    void getPosition(int *x, int *y) { *x = _x; *y = _y; }
    void setPosition(int x, int y) { _x = x; _y = y; }
};

#endif // ITEM_H
