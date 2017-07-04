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
#include "entitylist.h"

entity_id_t EntityList::addNewEntity(Entity e)
{
    auto e_id = e.getId();
    if (existsEntityWithId(e_id)) {
        return FIRST_ENTITY_ID - 1;
    }
    entity_map_[e_id] = e;
    return e_id;
}

bool EntityList::existsEntityWithId(entity_id_t e_id)
{
    return entity_map_.find(e_id) != entity_map_.end();
}

bool EntityList::deleteEntityWithId(entity_id_t e_id)
{
    if (! existsEntityWithId(e_id)) {
        return false;
    }
    entity_map_.erase(e_id);
    return true;
}

Entity EntityList::getEntity(entity_id_t e_id)
{
    if(! existsEntityWithId(e_id)) {
        return Entity(); // invalid entity
    }
    return entity_map_[e_id];
}
