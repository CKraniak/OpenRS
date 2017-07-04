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

#ifndef ECMANAGER_H
#define ECMANAGER_H

#include <vector>

#include "eclist.h"
#include "entityfactory.h"

// Manage the interaction between an ECList and an EntityFactory.
//
// The current design says you send an ECList to the factory and the factory
// adds stuff to the ECList based on the EntityType we're making. Since you
// need both an ECList and an EntityFactory for such a basic operation, I
// thought it'd be simpler to have a bit of code handle the pair for you.
//
// I fucking hate "manager" classes, though. I so wanted to avoid this, but I
// can't think of anything better.


class ECManager
{
public:
    ECManager(ECList         eclist    = ECList(),
              EntityFactory  e_factory = EntityFactory());

    EntityFactory setFactory(EntityFactory factory);
    ECList        setEcList(ECList eclist);

    Entity makeEntity(std::string entity_typename);
    bool   deleteEntity(Entity &e);

    // Recall that there can be multiple components with the same name
    std::vector<Component> getComponents(Entity e,
                                         std::string component_name = "");
    Component              getComponent(Entity e,
                                        std::string component_name,
                                        int index = 0);

    // Using this is necessary to propogate the component value change into the
    // ECList. You can't just change the component's value in your function and
    // expect it to work, since you (should) only have a copy.
    bool setComponentValue(Component &c, std::string new_value);
    // This copies your changes to a component's value into the ECList, making
    // the changes available to other systems.
    bool pushComponentUpdate(Component c);

    // You can kind of muddle around with Components, but entity generation
    // is currently only something an EntityType can do.
    bool addComponentToEntity(Entity e, Component c);
    bool removeComponentFromEntity(Entity e, Component c);

    std::vector<Entity> getEntitiesWithSignature(EntitySignature sig);

    static void test();

    int loadDirectory(std::string directory);

private:
    ECList eclist_;
    EntityFactory factory_;
};

#endif // ECMANAGER_H
