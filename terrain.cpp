/** Terrain is stuff on the game grid that's immutable.
  * This includes walls, ground, holes, stairs, etc.
  */

#include "terrain.h"

char getDisplayChar(TerrainType ttype, TerrainSubType tstype) {
    switch (ttype) {
    case TT_WALL:
        return 'W';
    case TT_GROUND:
        return '.';
    case TT_STAIR:
        return 'S';
    case TT_EXIT:
        return 'E';
    }
    return ' ';
}

Terrain::Terrain(TerrainType type) {
    ttype = type;
    tstype = TST_NONE;
    display_character = getDisplayChar(type, tstype);
}

bool Terrain::isBlocking() {
    if (ttype == TT_WALL) {
        return true;
    }
    return false;
}
