/** The immediate map is what represents the environment local to
  * an actor.
  * A camera will draw its info from the immediate map primarily.
  */

#include "localgrid.h"

//LocalGrid::LocalGrid() {
//    current_district = new District(NULL);
//}

LocalGrid::LocalGrid(int x, int y, District * district) {
        current_district = district;
        x_index = x;
        y_index = y;
        width = 15;
        height = 15;
}

int LocalGrid::getAsciiString(std::string *output) {
    int center_width = width / 2;
    int center_height = height / 2;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            (*output) +=
                current_district->getAsciiCharAt(x_index + j - center_width,
                                                 y_index + i - center_height);
        }
        (*output) += "\n";
    }
    return 1;
}
