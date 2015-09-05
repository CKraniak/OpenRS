/** Represents a whole district, as in Rogue Survivor
  */

#include <iostream>

#include "district.h"
#include "terrain.h"
#include "item.h"
#include "playercharacter.h"

District::District(GameOptionSet *options, Dispatcher * dispatcher) {
    _options = options;
    width = 15;
    height = 15;
    is_generated = false;
    mainloop_dispatcher = dispatcher;
}

int District::generate() {
    if (is_generated) {
        return 0;
    }
    is_generated = true;
    // Manually add several Terrain tiles and items
    Terrain * t;
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (i == 7 && j == 7) {
                t = new Terrain(TT_WALL);
                terrain_array.push_back(t);
                t->getAsciiChar();
            }
            else {
                t = new Terrain(TT_GROUND);
                terrain_array.push_back(t);
                t->getAsciiChar();
            }
        }
    }

    // Add player character item
    item_array.push_back(new PlayerCharacter(8,
                                             8,
                                             IT_PLAYER_CHARACTER,
                                             mainloop_dispatcher,
                                             this));

    return 0;
}

char District::getAsciiCharAt(int x, int y) {
    if (x < 0 || x > width || y < 0 || y > height) {
        return 'O';
    }
    std::vector<Item *> items;
    if (this->getItemsAt(x, y, &items) != 0) {
        Item * item = items[items.size() - 1];
        return item->getAsciiChar();
    }
    else {
        Terrain * t = this->getTerrainAt(x, y);
        if (t != NULL) {
            return t->getAsciiChar();
        }
    }
    return 'X';
}

int District::getItemsAt(int x, int y, std::vector<Item *> * output) {
    int _x = -1;
    int _y = -1;
    for (   std::vector<Item *>::iterator it = item_array.begin();
            it != item_array.end();
            it++) {
        (*it)->getPosition(&_x, &_y);
        if (_x == x && _y == y) {
            output->push_back( (*it) );
        }
    }
    return output->size() > 0 ? 1 : 0;
}

Terrain * District::getTerrainAt(int x, int y) {
    if (!is_generated) {
        return NULL;
    }
    int index = y * width + x;
    //return terrain_array[index]->getAsciiChar();
    return terrain_array[index];
}

bool District::moveItemRelativeByNumpad(Item *item, char direction) {
    int x = -1;
    int new_x = -1;
    int y = -1;
    int new_y = -1;
    item->getPosition(&x, &y);
    switch (direction) {
    case '1': // SW
        new_x = x - 1; // Left one
        new_y = y + 1; // Down one
        break;

    case '2': // S
        new_x = x;
        new_y = y + 1; // Down one
        break;

    case '3': // SE
        new_x = x + 1; // Right one
        new_y = y + 1; // Down one
        break;

    case '4': // W
        new_x = x - 1; // Left one
        new_y = y;
        break;

    case '6': // E
        new_x = x + 1; // Right one
        new_y = y;
        break;

    case '7': // NW
        new_x = x - 1; // Left one
        new_y = y - 1; // Up one
        break;

    case '8': // N
        new_x = x;
        new_y = y - 1; // Up one
        break;

    case '9': // NE
        new_x = x + 1; // Right one
        new_y = y - 1; // Up one
        break;

    default:
        break;
    }
    if (new_x < 0 || new_x > (width - 1) || new_y < 0 || new_y > (height - 1)) {
        return false;
    }
    Terrain * t = this->getTerrainAt(new_x, new_y);
    if (t->isBlocking()) {
        return false; // can't move onto blocking terrain;
    }
    item->setPosition(new_x, new_y);
    return true;
}
