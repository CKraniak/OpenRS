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
#include <typeinfo>
#include "gameeventhandler.h"

bool HandlerBase::shouldRunOnEvent(EventBase& e)
{
    // Be sure the handler and event share data types.
    if (data_type_name.compare(e.getDataTypeName()) != 0)
        return false;
    std::vector<std::string> e_types = e.getTypes();
    return bool_tree.evaluate(e_types);
}

bool HandlerBase::operator==(const HandlerBase &that) const
{
    if(typeid(*this) == typeid(that)) {
        return true;
    }
    return false;
}
