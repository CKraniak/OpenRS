#ifndef IMMEDIATEMAP_H
#define IMMEDIATEMAP_H

#include <string>

class LocalGrid;

#include "item.h"
#include "terrain.h"
#include "district.h"

class LocalGrid
{
private:
//    std::vector<Terrain> TerrainArray;
//    std::vector<Item> ItemArray;
    class District *current_district;
    int x_index;
    int y_index;
    int width;
    int height;

public:
    LocalGrid(int x, int y, District * district);
    int getAsciiString(std::string * output);
};

#endif // IMMEDIATEMAP_H
