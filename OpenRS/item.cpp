/** Any non-terrain thing on the game grid.
  *
  */

#include "item.h"

char getDisplayChar(ItemType itype) {
    switch (itype) {
    case IT_ACTOR:
        return 'X';
    case IT_PLAYER_CHARACTER:
        return 'P';
    case IT_FURNITURE:
        return 'n';
    }
    return ' ';
}

Item::Item(int x, int y,
           ItemType type,
           Dispatcher * dispatcher,
           District * parent) {
    itype = type;
    _x = x;
    _y = y;
    display_character = getDisplayChar(type);
    mainloop_dispatcher = dispatcher;
    _parent = parent;
}
