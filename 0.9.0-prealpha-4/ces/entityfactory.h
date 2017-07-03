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

#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include <map>

#include "entitytype.h"
#include "eclist.h"

// Look into having the factory emit when it creates any entities.

class EntityFactory {
	std::map<std::string, EntityType> type_map; // typename, entity_type pairs

public:
    Entity makeEntity(ECList ec_list, std::string type);
    bool pushNewType(std::string typename_, EntityType type_);
	void removeType(std::string typename_);
    bool hasEntity(std::string typename_);
};

#endif // ENTITYFACTORY_H
