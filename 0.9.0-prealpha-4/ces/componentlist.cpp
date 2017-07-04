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
#include "componentlist.h"

#include  <algorithm>

Component ComponentList::getComponentById(component_id_t id) {
    auto c_it = component_id_map_.find(id);
    if(c_it == component_id_map_.end()) {
        return Component();
    }
    return component_id_map_[id];
}

Component ComponentList::getComponentByIndex(int i) {
    return component_id_map_[id_list_[i]];
}

component_id_t ComponentList::pushUniqueIdComponent(Component c) {
    auto component_id = c.getId();
    if(component_id_map_.find(component_id) == component_id_map_.end()) {
        component_id_map_[component_id] =  c;
        id_list_.push_back(component_id);
        return component_id;
    }
    return FIRST_COMPONENT_ID - 1;
}

bool ComponentList::setComponentValue(component_id_t id, std::string new_value)
{
    auto component = getComponentById(id);
    if (! component.isValid()) {
        return false;
    }
    component.setData(new_value);
    component_id_map_[id] = component;
    return true;
}

bool ComponentList::hasComponent(component_id_t c)
{
    return component_id_map_.find(c) != component_id_map_.end();
}

bool ComponentList::removeComponent(component_id_t c)
{
    if (! hasComponent(c)) {
        return false;
    }
    component_id_map_.erase(c);
    id_list_.erase(std::remove(id_list_.begin(), id_list_.end(), c),
                   id_list_.end());
    return true;
}
