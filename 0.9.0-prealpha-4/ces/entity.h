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
#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <map>

#include "../fileio/cesio.h"
#include "componentlist.h"

class Entity
{
    int id_;
    ComponentList component_list_;
    std::string entity_type_name_; // e.g. "district", "player", "monster"
                                   // entity_type_name is only for readability;
                                   // any use in game logic is an error, as
                                   // logic should a.) be in a CESystem and
                                   // b.) rely on the components the entity has,
                                   // to determine what it is, not the name.
    int generateUniqueId();
public:
    Entity();
    int numComponentsWithName(std::string name);
    std::vector<int> getComponentsWithName();
    int removeComponentById(int);
};

#endif // ENTITY_H
