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

// As the components are listed by ID in the entities, there needs to be a way
// to grab the components from somewhere.
//
// There will actually be a "main list" somewhere, but this class handles the
// more general case of N Components, and querying certain kinds of information
// on the N components.

#ifndef COMPONENTLIST_H
#define COMPONENTLIST_H

#include <vector>
#include <unordered_map>
#include <memory>

#include "component.h"

class ComponentList
{
    std::vector<int> id_list_;
    std::unordered_map<int, std::shared_ptr<Component>>  component_id_map_;
    int count_;

    int getFirstUnusedId();

public:
    ComponentList();
    // Currently does not check for existence
    std::shared_ptr<Component> getComponentById(int id) {
        return component_id_map_[id];
    }
    std::shared_ptr<Component> getComponentByIndex(int i) {
        return component_id_map_[id_list_[i]];
    }
    int pushUniqueComponent(Component c) {
        int component_id = c.getId();
        if(component_id_map_.find(component_id) == component_id_map_.end()) {
            component_id_map_[component_id] =
                    std::shared_ptr<Component>(new Component(c));
            id_list_.push_back(component_id);
        }
    }

    int getNumComponents() { return component_id_map_.size(); }
};

#endif // COMPONENTLIST_H
