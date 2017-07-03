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

#ifndef ENTITYTYPE_H
#define ENTITYTYPE_H

#include <map>

#include "entity.h"
#include "eclist.h"

const std::string DEFAULT_TYPENAME_COMPONENT_NAME = "entity";

// A prototype for an entity.
// Should be able to ask an EntityType object for an Entity, and have the Type
// build the entity for you, I'd think.
//
// The entity type is what gets loaded out of files.
//
// The EntityFactory will hold a collection of these and use them to do
// the actual dirty work.
class EntityType {
	std::string typename_; // Not required to be the same as the header in
						   // an entity data file, particularly when the
						   // entity type is internally generated.
    // I think this data type was to allow multiple components with the same
    // name (by specifying multiple values). Empty components have "" values.
    std::map<std::string, std::vector<std::string>> components_;
public:
    Entity makeEntity(ECList ec_list);

    EntityType() {}
    EntityType(std::string name,
               std::map<std::string, std::vector<std::string>> components) :
        typename_(name), components_(components) {}

    int addComponent(std::string component_name,
                     std::string component_value);
    int removeMatchingComponent(std::string component_name,
                                std::string component_value);
    int removeAllComponentsWithName(std::string component_name);

    std::string getTypename() { return typename_; }
    std::string setTypename(std::string type_name) { typename_ = type_name; }
    // Search through previously added components for a entity=<str> type and
    // if  found make it the EntityType's typename.
    std::string findAndSetTypenameInComponents(std::string typename_component =
                                                   DEFAULT_TYPENAME_COMPONENT_NAME);
};

#endif // ENTITYTYPE_H
