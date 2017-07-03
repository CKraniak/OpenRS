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

Entity EntityType::makeEntity(ECList ec_list)
{
    Entity ret_entity;
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
