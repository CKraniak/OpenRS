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

#include "entityfactory.h"

#include  <cassert>

Entity EntityFactory::makeEntity(ECList ec_list, std::string type)
{
    assert(hasEntity(type));
    return type_map[type].makeEntity(ec_list);
}

bool EntityFactory::pushNewType(std::string typename_, EntityType type_)
{
    auto tit = type_map.find(typename_);
    if(tit != type_map.end()) {
        return 0;
    }
    type_map[typename_] = type_;
}

bool EntityFactory::hasEntity(std::string typename_)
{

}
