#ifndef TERRAIN_H
#define TERRAIN_H

enum TerrainType {
    TT_WALL = 1,
    TT_STAIR,
    TT_GROUND,
    TT_EXIT
};

enum TerrainSubType {
    TST_WALL_NS = 1,
    TST_WALL_EW,
    TST_WALL_NWCORNER,
    TST_WALL_NECORNER,
    TST_WALL_SWCORNER,
    TST_WALL_SECORNER,

    TST_GROUND_GRASS,
    TST_GROUND_ROAD,
    TST_GROUND_SIDEWALK,
    TST_GROUND_FLOOR_WOOD,
    TST_GROUND_FLOOR_CONCRETE,
    TST_GROUND_FLOOR_TILE,

    TST_NONE
};
#include <iostream>
class Terrain
{
private:
    TerrainType ttype;
    // bool isIndoors;
    TerrainSubType tstype;
    char display_character;

public:
    Terrain(TerrainType type);
    inline char getAsciiChar() { return display_character; }
    bool isBlocking();
};

#endif // TERRAIN_H
