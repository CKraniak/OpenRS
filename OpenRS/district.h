#ifndef DISTRICT_H
#define DISTRICT_H

#include <vector>

class District;

#include "gameoptionset.h"
#include "terrain.h"
#include "item.h"
#include "playercharacter.h"
#include "dispatcher.h"

class District
{
private:
    std::vector<Terrain *> terrain_array;
    std::vector<class Item *> item_array;
    GameOptionSet * _options;
    int width;
    int height;
    bool is_generated;
    Dispatcher * mainloop_dispatcher;
    static void onMoveItem(void * _this,
                           int argc,
                           std::vector<std::string *> argv) {
        (((District *)_this)->mainloop_dispatcher)
                            ->pushCommand(&CDESC_DISPLAY_MARKFORREDRAW);
//        (((District *)_this)->mainloop_dispatcher)
//                            ->execute();
#ifdef PRINTLINE
        std::cout << "district onMoveItem" << std::endl;
#endif
    }

public:
    District(GameOptionSet * options,
             Dispatcher * dispatcher);
    int generate();
    int getWidth() { return width; }
    int getHeight() { return height; }
    char getAsciiCharAt(int x, int y);
    int getItemsAt(int x, int y, std::vector<Item *> * output);
    Terrain * getTerrainAt(int x, int y);
    bool moveItemRelativeByNumpad(Item * item, char direction);
};

#endif // DISTRICT_H
