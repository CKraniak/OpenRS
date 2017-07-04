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

#ifndef ECLIST_H
#define ECLIST_H

#include <vector>
#include <map>

#include "entitylist.h"
#include "componentlist.h"
#include "entitysignature.h"

// Methods and such for relations between EntityLists and ComponentLists.
// Mostly just maps between the two types of lists. But also this is where
// component lookup optimizations can occur.

// main() will make one, fill it with entities and components and whatnot
// from files. --> changed, ECManager makes one itself and loads it up based on
//                 some input from main() or whoever.

class ECList
{
    std::vector<EntitySignature> singnatures_;
    EntityList entities_;
    ComponentList components_;
    std::map<entity_id_t, std::vector<component_id_t>>  ec_map_;
    std::map<component_id_t, int> component_ref_count;

    // Called at the end of removeComponentFromEntity() and unregisterEntity()
    // for now.
    bool isComponentOrphaned(component_id_t c);
    void removeOrphanedComponents();
    bool unregisterComponent(component_id_t c, bool skip_check_entitys_and_map);

public:
    entity_id_t    registerEntity(Entity e);
    bool           unregisterEntity(entity_id_t e);
    component_id_t registerComponent(Component c);
    bool           unregisterComponent(component_id_t c);

    bool giveComponentToEntity (component_id_t c, entity_id_t e);
    bool removeComponentFromEntity(component_id_t c, entity_id_t e);
    bool entityHasComponent(component_id_t c, entity_id_t e);

    Entity                   getEntity(entity_id_t e_id);
    std::vector<entity_id_t> getEntitiesMatchingSignature(EntitySignature sig);
    bool                     hasEntity(entity_id_t e_id);

    std::vector<component_id_t> getComponentsOfEntity(entity_id_t e_id);
    Component                   getComponent(component_id_t c_id);
    bool                        setComponentValue(component_id_t c_id,
                                                  std::string new_value);
    bool                        hasComponent(component_id_t c);

};

#endif // ECLIST_H
