/* Copyright (C) 2016, Chesley Kraniak
 *
 * This code is distributed under the terms of the GPLv3 license, which should
 * have been included with this file in the root directory as "LICENSE.txt". If
 * you do not have a copy of the license, see:
 *     http://www.gnu.org/licenses/gpl-3.0.txt
 * or:
 *     https://github.com/CKraniak/OpenRS
 * for the license.
 */

#include "mlinterface/mlinterface.h"
#include "dispatcher/statefuldispatcher.h"
#include "ces/systemgroup.h"
#include "ces/systems/systems.h"

int openrs_main(int argc, char** argv) {
    // - Start event system. Since this is the big intra-communication device,
    //   it will generally come first.
    StatefulDispatcher sd = StatefulDispatcher();

    SystemGroup systems(&sd);

    // - TODO: Process command-line args and config file here

    // **************************

    // I can't think of anyone other than main that should be responsible for
    // saying what goes in the system group. Certainly not the systems
    // themselves, as that would be anarchy. Not the system group, since that
    // conceptually is more like a container and not a manager. So there is
    // only main, as no other layers exist in the CES.
    //
    // This means if I want multiple system groups, then not letting the group
    // control system connection is important. Or if I want to adjust the
    // composure of the systems in a config file, then main is the right place
    // to process the config file and divvy out the system composures.
    AsciiDisplayCESystem adces;
    systems.connectSystem(&adces);

    InputCESystem ices;
    systems.connectSystem(&ices);

    PlayerMovementCESystem pmces;
    systems.connectSystem(&pmces);

    // **************************

    // Start OS-specific code in the Main Loop Interface class.
    // Mostly should involve grabbing OS events.
    std::unique_ptr<MLInterface> mli = MLInterface::getInterface(&sd);

    // **************************
    //
    // Just before jumping into the game loop, send the "init" command
    //sd.setState("init");

    // **************************
    mli.get()->createMainWindow();
    while ( ! mli.get()->shouldQuit() ) {
        // main loop:
        //  - look for input, process if available
        mli.get()->emptyMessagePump();
        //  - update
        //  - render
    }
    return 0;
}

