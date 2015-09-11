#ifndef GAME_H
#define GAME_H

#include "configurationmanager.h"
#include "core.h"
#include "display.h"
#include "inputhandler.h"
#include "observer.h"
#include "timer.h"

class Game
{
public:
    Game();
    int execute();

private:
    void winMainLoopCallback(); // may or may not be what's needed

    ConfigurationManager conf;
    Core core;
    Display display;
    InputHandler input;
    Observer observer;
    Timer timer;
};

#endif // GAME_H
