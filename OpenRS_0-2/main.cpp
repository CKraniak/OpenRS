/** Copyright (C) 2015, by Chesley Kraniak
    License is currently indeterminate, but will be opensource (at least
    GPL levels of freedom, I think).

    See the README or MANUAL for more information.
  */

#include "game.h"

#ifdef CPP_DEBUG
#define COUT(x) std::cout << x << std::endl
#endif // CPP_DEBUG

#ifdef CPP_DEBUG_BOOST_SIG
#include "boost/signals2.hpp"
#include <iostream>
#endif // CPP_DEBUG_BOOST_SIG

#ifdef CPP_DEBUG_OBS_BOOST
#include <list>
#include <string>
#include "observer.h"
#endif // CPP_DEBUG_OBS_BOOST

#ifdef CPP_DEBUG_BOOST_SIG
struct HelloWorld {
    void operator()() const {
        std::cout << "Hello World!" << std::endl;
    }
};
#endif // CPP_DEBUG_BOOST_SIG

#ifdef CPP_DEBUG_OBS_BOOST
struct ObsBoostTest {
    int operator()(Observer::OBS_ARGV_TYPE argv) const {
        COUT("ObsBoostTest()");
        COUT(argv.front());
        return EXIT_SUCCESS;
    }
};
#endif // CPP_DEBUG_OBS_BOOST

int main(int argc, char** argv)
{
    Game game_obj;
#ifdef CPP_DEBUG_BOOST_SIG
    COUT("debug boost signals");
    boost::signals2::signal<void ()> sig;
    HelloWorld h;
    sig.connect(h);
    sig();
#endif // CPP_DEBUG_BOOST_SIG
#ifdef CPP_DEBUG_OBS_BOOST
    COUT("debug observer boost");
    Observer::OBS_SIGNAL_TYPE obs_sig;
    ObsBoostTest obt;
    obs_sig.connect(obt);
    Observer::OBS_ARGV_TYPE arg;
    arg.push_front("TestArg");
    obs_sig(arg);
#endif // CPP_DEBUG_OBS_BOOST
    // Any command line args would be handled here.
    return game_obj.execute();
}
