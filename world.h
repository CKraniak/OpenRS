#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "district.h"
#include "gameoptionset.h"
#include "dispatcher.h"

class World
{
private:
    std::vector<District *> district_array;
    District * start_district;
    GameOptionSet * _options;
    int start_district_row;
    int start_district_column;
    int width;
    int height;
    bool is_generated;
    Dispatcher * mainloop_dispatcher;

public:
    World(GameOptionSet * options, Dispatcher * dispatcher);
    int generate();
//    int getWorldDistrictMapInfo();
    District * getStartDistrict();
};

#endif // WORLD_H
