/* Copyright (C) 2017 Chesley Kraniak
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
#include "fileio/configuration.h"
#include "fileio/cesio.h"
#include "ces/ecmanager.h"

#include  <cassert>

// Generate commonly used configuration information.
// Not to be confused with an application-wide singleton maker. Over my dead
// body.
//
// If it's not something related to what you would learn from configuration.txt,
// then it doesn't belong here / in the Configuration object.
Configuration setupConfig () {
    Configuration conf;
    std::unique_ptr<Directory> d = Directory::getDirectory();

    assert(conf.setConfiguration("exe_cwd", d->getCwd()));
    assert(conf.setConfiguration("full_core_resource_dir",
                                    conf.get("exe_cwd") +
                                    conf.get("core_resource_directory")));

    return conf;
}

int openrs_main(int argc, char** argv) {


    ECManager::test();
    return 0;




    // - Start event system. Since this is the big intra-communication device,
    //   it will generally come first.
    auto sd = std::shared_ptr<StatefulDispatcher>(new StatefulDispatcher());
    SystemGroup systems(sd);

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
    std::shared_ptr<AsciiDisplayCESystem> adces(new AsciiDisplayCESystem());
    systems.connectSystem(adces);

//    std::shared_ptr<InputCESystem> ices(new InputCESystem());
//    systems.connectSystem(ices);

    std::shared_ptr<PlayerMovementCESystem> pmces(new PlayerMovementCESystem());
    systems.connectSystem(pmces);

    std::shared_ptr<CollisionCESystem> cces(new CollisionCESystem());
    systems.connectSystem(cces);

    //std::shared_ptr<GameGridCESystem> ggces(new GameGridCESystem());
    //systems.connectSystem(ggces);

    // **************************

    // Start OS-specific code in the Main Loop Interface class.
    // Mostly should involve grabbing OS events.
    std::unique_ptr<MLInterface> mli = MLInterface::getInterface(sd);

    // Here is where renderer INIT will go
    // The display system will need its 3-state setup at some point.
    // Right now the focus is on completing the CES first, which means a
    // barebones rendering system right now.

    // **************************
    //
    // Just before jumping into the game loop, send the "init" command
    //sd.setState("init");

    // **************************
    mli->createMainWindow();
    while ( ! mli->shouldQuit() ) {
        // main loop:
        //  - look for input, process if available, reach and update game data
        //    as required.
        mli->emptyMessagePump();
        //  - update --> everything draws to dirty Layers down the display tree
        //               and back up again
        //  - render --> render gets a list of stuff updated and the new data
        //               and pushes it all to the GPU
    }
    return 0;
}

