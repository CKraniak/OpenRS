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
#include "../fileio/cesio.h"
#include "../utility.h"

ECManager::ECManager(ECList eclist, EntityFactory e_factory) :
    eclist_(eclist), factory_(e_factory)
{

}

// Add a new Entity of EntityType entity_type to the ECList and return the
// handle to it.
//
// In the case of failure, makeEntity returns an invalid Entity. This can be
// checked with the Entity.isValid() function.
Entity ECManager::makeEntity(std::string entity_typename)
{
    Entity e;
    if (! factory_.hasEntity(entity_typename)) {
        e.is_valid_ = false;
        return e;
    }
    e = factory_.makeEntity(eclist_, entity_typename);
    e.is_valid_ =  true;
    return e;
}

bool ECManager::deleteEntity(Entity &e)
{
    bool success = eclist_.unregisterEntity(e.getId());
    if (success) {
        e.is_valid_ = false;
    }
    return success;
}

std::vector<Component> ECManager::getComponents(Entity e,
                                                std::string component_name)
{
    auto e_id =  e.getId();
    auto c_id_vec = eclist_.getComponentsOfEntity(e_id);
    std::vector<Component> ret_vec;
    for (auto c_id : c_id_vec) {
        Component c = eclist_.getComponent(c_id);
        // Skip names not equal to component_name
        // Skip no names if input was ""
        if ((! c.getName().compare(component_name)) ||
            (! component_name.compare(""))) {
            ret_vec.push_back(c);
        }
    }
    return ret_vec;
}

Component ECManager::getComponent(Entity e,
                                  std::string component_name,
                                  int index)
{
    auto components = getComponents(e, component_name);
    if (components.size() == 0) {
        return Component::getInvalidComponent();
    }
    return components[index];
}

bool ECManager::setComponentValue(Component & c, std::string new_value)
{
    if (! eclist_.setComponentValue(c.getId(), new_value)) {
        return false;
    }
    c.setData(new_value);
    return true;
}

bool ECManager::pushComponentUpdate(Component c)
{
    return setComponentValue(c, c.getData());
}

bool ECManager::addComponentToEntity(Entity e, Component c)
{
    // I  think this is duplicating what the ECList already does.
//    if ((! eclist_.hasComponent(c))  ||
//        (! eclist_.hasEntity(e))     ||
//        (eclist_.entityHasComponent(c, e))) {
//        return false;
//    }
    if (! eclist_.hasComponent(c.getId())) {
        eclist_.registerComponent(c);
    }
    return eclist_.giveComponentToEntity(c.getId(), e.getId());
}

bool ECManager::removeComponentFromEntity(Entity e, Component c)
{
    return eclist_.removeComponentFromEntity(c.getId(), e.getId());
}

std::vector<Entity> ECManager::getEntitiesWithSignature(EntitySignature sig)
{
    auto e_id_vec = eclist_.getEntitiesMatchingSignature(sig);
    std::vector<Entity> rval;
    if (e_id_vec.size() > 0) {
        for (auto id : e_id_vec) {
            rval.push_back(eclist_.getEntity(id));
        }
    }
    return rval;
}

// Load all entity files from the given directory.
// Return the number of unique types found.
int ECManager::loadDirectory(std::string directory)
{
    CesIo cio(directory);
    cio.populateEntityFactory(factory_);
    return factory_.getNumTypes();
}

// Leave at bottom. ECManager isn't supposed to know about this, and it's only
// here for testing purposes.
#include "../fileio/configuration.h"

void ECManager::test()
{
    INFO_MSGOUT_DEBUG("In ECManager::test()");
    // What to do?
    //
    // NEW RULE: No IDs leave ECManager. ECManager ONLY deals with Entitys and
    //           Components.
    //
    // Need to test:
    // X load files in (done in CesIO, but recreate it here)
    // --- Note: failure to specify path makes Directory walk the whole drive
    // X make one entity that has some components
    // X read components with an entity id
    // X modify a component using an id (or the Component itself)
    // X add a new component, connect to previously made entity
    // X make several entities with varying components and select only a subset
    // --- i.e. only those with locx and locy components
    // X read all components with an EntitySignature
    // X delete an entity
    // --- will need to delete orphaned Components
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

    // This cruft here will be done by main() in the long term.
    auto cwd = CWD + "/";

    Configuration conf;
    auto entity_dir = conf.getConfigurationValue("core_resource_directory");

    {
        ECManager m;
        int types = m.loadDirectory(cwd + entity_dir);

        auto entity = m.makeEntity("district");
        auto component_vec = m.getComponents(entity);
        auto component_1 = m.getComponent(entity, "");
        auto component_2 = m.getComponent(entity, "", 1);

        m.setComponentValue(component_1, "new_value_1");
        component_2.setData("yet more new data #2");
        m.pushComponentUpdate(component_2);

        auto component_3 = m.getComponent(entity, "");
        auto component_4 = m.getComponent(entity, "", 1);
    // Should be able to see component_3 == component_1 in debugger
    // Same with component_2 and component_4

        Component new_component("sinewaves", "groovy");
        m.addComponentToEntity(entity, new_component);
    }
    // Change scope so I can reuse variables.
    {
        ECManager m;
        m.loadDirectory(cwd + entity_dir);

        auto entity_1 = m.makeEntity("player");
        auto entity_2 = m.makeEntity("medkit");
        auto entity_3 = m.makeEntity("district");

        std::vector<std::string> locvec = {"position_x", "position_y"};
        EntitySignature locsig(locvec);

        auto entities = m.getEntitiesWithSignature(locvec);

        m.deleteEntity(entity_1);

        auto entity_dne = m.makeEntity("anentitywhichdoesntexist");

        auto c = m.getComponent(entity_2, "health_given");
        m.removeComponentFromEntity(entity_2, c);

        int a = 1 + 2; // So I can see entities in the debugger before they
                       // are deallocated.
    }
    return;
}
