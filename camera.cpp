/** The camera is responsible for managing the display of
  * the main game screen. Generally, this will be the area
  * around the PC, but since I'm divorcing it from the PC
  * it can move / display other immediate maps.
  */

#include <iostream>
#include "camera.h"

//Camera::Camera() {

//}

//int Camera::attachToItem(Item item) {
//    return 0;
//}

//int Camera::attachToPosInDistrict(District *district, int x, int y) {
//    return 0;
//}

Camera::Camera(int width, int height, LocalGrid *map, Dispatcher * dispatcher) {
     _width = width;
     _height = height;
     _map = map;
     AsciiArray = "";
     to_redraw = false;
     dispatcher->registerCommand(CALLBACK_FIX(this->onMarkForRedrawEvent),
                                 &CDESC_DISPLAY_MARKFORREDRAW,
                                 this);
     dispatcher->registerCommand(CALLBACK_FIX(this->onRedrawEvent),
                                 &CDESC_DISPLAY_REDRAW,
                                 this);
}

void Camera::update() {
    this->updateAsciiScene();
    this->printAsciiScene();
}

int Camera::updateAsciiScene() {
    AsciiArray.clear();
    _map->getAsciiString(&AsciiArray);
    return 1;
}

void Camera::printAsciiScene() {
    std::cout << AsciiArray << std::endl;
}
