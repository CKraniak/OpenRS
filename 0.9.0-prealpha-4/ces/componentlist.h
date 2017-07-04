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

#include "component.h"

class ComponentList
{
    friend class ECList;

private:
    ComponentList() {}

    std::vector<component_id_t> id_list_;
    std::unordered_map<component_id_t, Component>  component_id_map_;
    int count_;

    // Currently does not check for existence
    Component getComponentById(component_id_t id);
    Component getComponentByIndex(int i);
    component_id_t pushUniqueIdComponent(Component c);

    bool setComponentValue(component_id_t id, std::string new_value);

    bool hasComponent(component_id_t c);

    bool removeComponent(component_id_t c);

    int getNumComponents() { return component_id_map_.size(); }
};

#endif // COMPONENTLIST_H
