/** Top level management.
  * Not sure if I'll be doing an event system or what.
  * Definitely handles any main loop, process spawning, etc.
  *
  * Actually, I might make a "MenuManager" and have both
  * get upper-leveled by main.
  */

#include "gamemanager.h"

GameManager::GameManager()
{
    // Control should come straight here from main, more or less.
    // For the starter program, I'll want to:
    //  - initialize the game
    //  - drop into an update loop of some sort
    //  - probably be ready to allow player movement using the numpad

    // Initialize will: build a game screen, which
    //                  needs a camera, which
    //                  needs an immediate map to show, which
    //                  will need a PC in it
    // The rules for building the game will eventually be part of a bunch
    // of world generators and such. For now, I'll put them in their own
    // function in this file, "GameInit" (as opposed to "MenuInit" or
    // somesuch).
    this->gameInit(1, 0, 0);
}

// This is the proto for what becomes the game world initializer.
// It will eventually have a bunch of options, I think.
int GameManager::gameInit(int func_mode, int argc, char **argv)
{
    int retval = 0;
    switch(func_mode) {
    case 1:
    {
        // This is where the very first init will run from.
//        screen = new GameScreen();
//        imm_map = new ImmediateMap();
//        imm_map.generate();
//        camera = new Camera();
//        camera.attachImmediateMap(imm_map);
//        screen.addCamera(camera);
//        is_initialized = true;
//        retval = 1;
        break;
    }

    default:
        break;
    }

//endinit:
    return retval;
}

int GameManager::run()
{
    return 0;
}
