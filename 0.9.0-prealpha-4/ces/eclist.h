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
// from files.

class ECList
{
    std::vector<EntitySignature> singnatures_;
    EntityList entities_;
    ComponentList components_;
    std::map<entity_id_t, component_id_t>  ec_map_;

public:
};

#endif // ECLIST_H
