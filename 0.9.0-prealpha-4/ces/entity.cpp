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

bool EntityType::loadEntityTypeFromFile(std::string filename)
{
    CesIo io_handler;
    int bytes_read = io_handler.loadFromFile(filename);
}
