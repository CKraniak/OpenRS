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
// more general case of N Components, and queryign certain kinds of information
// on the N components.

#ifndef COMPONENTLIST_H
#define COMPONENTLIST_H

#include <vector>
#include <map>
#include <memory>

#include "component.h"

struct ComponentIdList {
    std::vector<int> id_list_;
};

class ComponentList
{
    ComponentIdList id_list_;
    std::map<int, std::shared_ptr<Component>>  coomponent_id_map_;

    int getFirstUnusedId();

public:
    ComponentList();
};

#endif // COMPONENTLIST_H
