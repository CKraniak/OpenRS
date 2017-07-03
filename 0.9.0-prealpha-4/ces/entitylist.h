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

// Need an entity collection of some kind. It may not be "pure", but I will need
// to manage groups of entities *somehow*.

#ifndef ENTITYLIST_H
#define ENTITYLIST_H

#include <map>
#include <string>
#include "entity.h"

class EntityList
{
    friend class ECList;

private:
    EntityList();

    entity_id_t addNewEntity(std::string entity_type_name = "");
    bool existsEntityWithId(entity_id_t);
    bool deleteEntityWithId(entity_id_t);

    std::map<entity_id_t, Entity> entity_map_;
};

#endif // ENTITYLIST_H
