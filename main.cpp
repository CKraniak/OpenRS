#include <iostream>
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#define DEBUG 1

#include "defines.h"
#include "gameoptionset.h"
#include "world.h"
#include "camera.h"
#include "dispatcher.h"
#include "command_aliases.h"

CALLBACK_FUNCTION(Camera, update, &CDESC_ANY)

// Old registration code, keep for documentation in case the macro starts
// to break on some future callback.
//void callbackCameraUpdate(void * _this,
//                          int argc,
//                          std::vector<std::string> argv) {
//    ((Camera *)_this)->update();
//}

int main(int argc, char** argv)
{
    GameOptionSet * options = new GameOptionSet();
    Dispatcher disp;
    World * world = new World(options, &disp);
    world->generate();
    int start_x = world->getStartDistrict()->getWidth() / 2;
    int start_y = world->getStartDistrict()->getHeight() / 2;
    LocalGrid * map = new LocalGrid(start_x,
                                    start_y,
                                    world->getStartDistrict());
    Camera * camera = new Camera(start_x, start_y, map, &disp);
    //bool needs_update = true;
    // Old registration code, keep for documentation in case the macro starts
    // to break on some future callback.
//    disp.registerCommand(CALLBACK_FIX(callbackCameraUpdate),
//                         &CDESC_ANY,
//                         camera);
    // HOLY SHIT
    // I CAN'T BELIEVE THIS MACRO WORKED FIRST TIME
    REGISTER_CALLBACK(Camera, update, &CDESC_ANY, disp, camera)
    std::string input;
    while (1) {
//        if (needs_update) {
//            camera->update();
//            needs_update = false;
//        }
        std::cin >> input;
        disp.parseCinInput(&input);
        //disp.execute(); // <-- This is so damn evil.
                        //     If it works, I'll be floored.
                        // UPDATE: HOLY FUCK, I'M FLOORED
        // needs_update = true;
    }
    return 0;
}

