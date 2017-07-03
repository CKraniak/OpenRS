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

    static void test();

private:
    ECList eclist_;
    EntityFactory factory_;
};

#endif // ECMANAGER_H
