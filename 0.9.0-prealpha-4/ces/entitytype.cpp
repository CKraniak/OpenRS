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

#include "entitytype.h"

#include  <cassert>

// Making an entity involves mostly just adding the correct entity (+ name)
// and components to the ECList and associating them.
Entity EntityType::makeEntity(ECList & ec_list)
{
    Entity ret_entity;
    ret_entity.setTypeName(typename_);
    ret_entity.is_valid_ = true;
    auto e_id = ec_list.registerEntity(ret_entity);
    for (auto components_with_name : components_)  {
        for (auto component_value : components_with_name.second) {
            // Skip the bit about making a component out of the entity type
            if (! component_value.compare(typename_)) {
                continue;
            }
            Component c(components_with_name.first, component_value);
            auto c_id = ec_list.registerComponent(c);
            ec_list.giveComponentToEntity(c_id, e_id);
        }
    }
    return ret_entity;
}

int EntityType::addComponent(std::string component_name, std::string component_value)
{
    auto cit = components_.find(component_name);
    if (cit == components_.end()) {
        std::vector<std::string> vs;
        vs.push_back(component_value);
        components_[component_name] = vs;
    }
    else {
        components_[component_name].push_back(component_value);
    }
    return 1; // What was I returning??
}

std::string EntityType::findAndSetTypenameInComponents(std::string typename_component)
{
    auto cit = components_.find(typename_component);
    if(cit != components_.end()) {
        if (cit->second.size() > 1) {
            return "";
        }
        typename_ = cit->second[0];
        return typename_;
    }
    return "";
}
