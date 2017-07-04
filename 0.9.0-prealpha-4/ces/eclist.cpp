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

#include <algorithm>

#include "eclist.h"
#include "../utility.h"

bool ECList::unregisterComponent(component_id_t c,
                                 bool skip_check_entitys_and_map)
{
    if (! skip_check_entitys_and_map) {
        //  Be sure it isn't in the ec_map_ anywhere
        for (auto ecpair  : ec_map_) {
            auto it = std::find(ecpair.second.begin(), ecpair.second.end(), c);
            if(it !=  ecpair.second.end()) {
                return false;
            }
        }
    }
    // From here, assume it isn't in ec_map_ anywhere.
    components_.removeComponent(c);
    auto crcnt = component_ref_count;
    crcnt.erase(c);
    component_ref_count = crcnt;
    return true;
}

entity_id_t ECList::registerEntity(Entity e)
{
    if (entities_.existsEntityWithId(e.getId())) {
        return FIRST_ENTITY_ID - 1;
    }
    auto e_id = entities_.addNewEntity(e);
    auto e_iter = ec_map_.find(e_id);
    if (e_iter != ec_map_.end()) {
        ERR_MSGOUT("Unexpected premapped entity in ECList::registerEntity,"
                   " possible failure of UUID or a copy constructor"
                   " somewhere.");
        return FIRST_ENTITY_ID - 1;
    }
    std::vector<component_id_t> empty_component_vector;
    ec_map_[e_id] = empty_component_vector;
    return e_id;
}

bool ECList::unregisterEntity(entity_id_t e)
{
    if (! hasEntity(e)) {
        return false;
    }
    auto components = getComponentsOfEntity(e);
    for (auto c : components) {
        removeComponentFromEntity(c, e);
    }
    ec_map_.erase(e);
    return entities_.deleteEntityWithId(e);
}

component_id_t ECList::registerComponent(Component c)
{
    // Unlike with the EntityList (which was coded later and differently), the
    // ComponentList will do the work for us. It's essentially the same as
    // with the EntityList, just somewhere else and not in ECList.
    //
    // TODO: Make registerComponent and registerEntity behave alike / not need
    //       different techniques to operate.
    return components_.pushUniqueIdComponent(c);
}

bool ECList::unregisterComponent(component_id_t c)
{
    return unregisterComponent(c, false);
}

// Returns true if successfully added, false otherwise.
// Could fail due to: unregistered entity, unregistered component, or
//                    component already given. You should know which when
//                    calling.
bool ECList::giveComponentToEntity(component_id_t c, entity_id_t e)
{
    auto e_it = ec_map_.find(e);
    if (! entities_.existsEntityWithId(e)) {
        return false;
    }

    if (e_it == ec_map_.end()) {
        ERR_MSGOUT("Unexpected unmapped entity in ECList::giveComponentToEntity.");
        return false;
    }
    auto c_vec = ec_map_[e];
    auto c_it = std::find(c_vec.begin(), c_vec.end(), c);
    if (c_it != c_vec.end()) {
        return false;
    }
    ec_map_[e].push_back(c);
    if (component_ref_count.find(c) == component_ref_count.end()) {
        component_ref_count[c] = 1;
    }
    else  {
        component_ref_count[c] += 1;
    }
    return true;
}

bool ECList::removeComponentFromEntity(component_id_t c, entity_id_t e)
{
    if (! entityHasComponent(c, e)) {
        return false;
    }
    std::vector<component_id_t> c_vec = ec_map_[e];
    c_vec.erase(std::remove(c_vec.begin(), c_vec.end(), c), c_vec.end());
    ec_map_[e] = c_vec;
    if (component_ref_count.find(c) == component_ref_count.end()) {
        ERR_MSGOUT("Unexpected  missing reference count in "
                   "ECList::removeComponentFromEntity");
        unregisterComponent(c, true);
    }
    component_ref_count[c] -= 1;
    if (component_ref_count[c] <= 0) {
        unregisterComponent(c, true);
    }
    return true;
}

bool ECList::entityHasComponent(component_id_t c, entity_id_t e)
{
    auto components = ec_map_.find(e);
    if (components == ec_map_.end()) {
        return false;
    }
    auto c_vec = components->second;
    auto c_it =  std::find(c_vec.begin(), c_vec.end(), c);
    return c_it != c_vec.end();
}

Entity ECList::getEntity(entity_id_t e_id)
{
    return entities_.getEntity(e_id);
}

std::vector<entity_id_t>
ECList::getEntitiesMatchingSignature(EntitySignature sig)
{
    std::vector<entity_id_t> entities_matched;
    for(auto entity_components_pair : ec_map_) {
        std::vector<Component> c;
        for (auto c_id : entity_components_pair.second) {
            c.push_back(components_.getComponentById(c_id));
        }
        if (sig.matchesSignature(c)) {
            entities_matched.push_back(entity_components_pair.first);
        }
    }
    return entities_matched;
}

bool ECList::hasEntity(entity_id_t e_id)
{
    return ec_map_.find(e_id) !=  ec_map_.end();
}

std::vector<component_id_t> ECList::getComponentsOfEntity(entity_id_t e_id)
{
    auto it = ec_map_.find(e_id);
    if (it == ec_map_.end()) {
        std::vector<component_id_t> r;
        r.push_back(FIRST_COMPONENT_ID  - 1);
        return r;
    }
    return ec_map_[e_id];
}

Component ECList::getComponent(component_id_t c_id)
{
    return components_.getComponentById(c_id);
}

bool ECList::setComponentValue(component_id_t c_id, std::string new_value)
{
    return components_.setComponentValue(c_id, new_value);
}

bool ECList::hasComponent(component_id_t c)
{
    return components_.hasComponent(c);
}
