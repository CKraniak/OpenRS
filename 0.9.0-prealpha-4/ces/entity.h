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

class Entity
{
    int id_;
    std::vector<int> component_id_list_;
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

// A prototype for an entity.
// Should be able to ask an EntityType object for an Entity, and have the Type
// build the entity for you, I'd think.
//
// The entity type is what gets loaded out of files.
class EntityType {
    std::string typename_; // Not required to be the same as the header in
                           // an entity data file, particularly when the
                           // entity type is internally generated.
    std::map<std::string, std::vector<std::string>> components;
public:
    Entity makeEntity();
    bool loadEntityTypeFromFile(std::string filename);
};

class EntityFactory {
    std::map<std::string, EntityType> type_map; // typename, entity_type pairs

public:
    Entity makeEntity(std::string type);
    bool loadFiles(std::vector<std::string> filename_vec);
    void pushNewType(std::string typename_, EntityType & type_);
    void removeType(std::string typename_);
};

#endif // ENTITY_H
