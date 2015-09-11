#include "game.h"

#include <cstdlib>

#ifdef CPP_DEBUG
#include <iostream>
#endif // CPP_DEBUG

Game::Game() // :
//    observer(),
//    conf(),
//    input(IH_IT_ASCII_CIN, observer),
//    timer()
{

}

// main() should return this function's exit code.
int Game::execute()
{
#ifdef CPP_DEBUG
    std::cout << "Game::execute()" << std::endl;
#endif // CPP_DEBUG

    return EXIT_SUCCESS;
}
/*

    // Start with the Observer, as other components will connect to it
    // and will thus need it to function properly.
    Observer dispatcher;
    // The configuration is loaded second, as any configurable thing will
    // need the information from it.
    ConfigurationManager conf;

    Timer timer;



    */
