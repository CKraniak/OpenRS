#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "gamescreen.h"
#include "camera.h"
#include "localgrid.h"

class GameManager
{
private:
    GameScreen * screen;
    Camera * camera;
    LocalGrid * imm_map;
    bool * is_initialized;

public:
    GameManager();
    int gameInit(int func_mode, int argc, char** argv);
    int run();
};

#endif // GAMEMANAGER_H
