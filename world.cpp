/** Holds the entire game world (all terrain and game grid items).
  */

#include "world.h"

World::World(GameOptionSet * options, Dispatcher * dispatcher) {
    _options = options;
    start_district = NULL;
    start_district_row = 0;
    start_district_column = 0;
    width = 0;
    height = 0;
    is_generated = false;
    mainloop_dispatcher = dispatcher;
}

int World::generate() {
    District * d = new District(_options, mainloop_dispatcher);
    district_array.push_back(d);
    d->generate();
    is_generated = true;
    return 1;
}

District *World::getStartDistrict() {
    if (!is_generated) {
        return NULL;
    }
    if (start_district == NULL) {
        start_district = district_array[start_district_row * width
                                + start_district_column];
    }
    return start_district;
}
