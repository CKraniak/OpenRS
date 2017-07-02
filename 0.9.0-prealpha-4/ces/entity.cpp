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

#include <vector>
#include <string>
#include <map>

#include "entity.h"
#include "../fileio/cesio.h"

Entity::Entity()
{

}

int Entity::numComponentsWithName(std::string name) {
    if(component_nametable_.find(name) !=
            component_nametable_.end()) {
        return component_nametable_[name].getNumComponents();
    }
    return 0;
}

ComponentList Entity::getComponentListWithName(std::string name) {
    auto retval = component_nametable_.find(name);
    return (retval == component_nametable_.end()) ? ComponentList() :
                                                    (*retval).second;
}

int Entity::pushComponent(Component c) {
    std::string name = c.getName();
    auto clist_it = component_nametable_.find(name);
    if (clist_it == component_nametable_.end()) {
        component_nametable_[name] = ComponentList();
    }
    return component_nametable_[name].pushUniqueComponent(c);
}
