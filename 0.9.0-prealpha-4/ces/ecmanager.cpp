/* Copyright (C) 2017, Chesley Kraniak
 *
 * This code is distributed under the terms of the GPLv3 license, which should
 * have been included with this file in the root directory as "LICENSE.txt". If
 * you do not have a copy of the license, see:
 *     http://www.gnu.org/licenses/gpl-3.0.txt
 * or:
 *     https://github.com/CKraniak/OpenRS
 * for the license.
 */

#include "ecmanager.h"

#include "../utility.h"

ECManager::ECManager(ECList eclist, EntityFactory e_factory) :
    eclist_(eclist), factory_(e_factory)
{

}

// Leave at bottom. ECManager isn't supposed to know about this, and it's only
// here for testing purposes.
#include "../fileio/cesio.h"
#include "../fileio/configuration.h"

void ECManager::test()
{
    INFO_MSGOUT_DEBUG("In ECManager::test()");
    // What to do?
    // Need to test:
    // - load files in (done in CesIO, but recreate it here)
    // - make one entity that has some components
    // - modify a component
    // - add a new component
    // - make several entities with varying components and select only a subset
    // --- i.e. only those with locx and locy components
    // - delete an entity
    // - delete a component from an entity
    //
    // The ECManager will be in the SystemsGroup and will be passed in to each
    // system for local comms.
    //
    // What I may do to optimize is hold list caches. So the GameGrid will want
    // both all the terrain entities and all the actor/locx&locy entities;
    // I can just keep a list of those on the side and send those lists to the
    // GameGrid when it asks instead of recomputing each time. But for now that
    // will be put off.
    ECManager m;
    std::unique_ptr<Directory> d = Directory::getDirectory();
    Configuration conf;
    auto entity_dir = conf.getConfigurationValue("core_resource_directory");
    INFO_MSGOUT_DEBUG((std::string("CRD: ") + entity_dir).c_str());
    EntityFactory efac;
    auto cwd = d->getCwd() + "/";
    CesIo ioman(cwd + entity_dir);]
    ioman.populateEntityFactory(m.factory_);
    return;
}

