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

#include "entity.h"
#include "../utility.h"

int Entity::generateUniqueEntityId()
{
    static int i = FIRST_ENTITY_ID;
    i++;
    if (i >= (1 << 30)) {
        INFO_MSGOUT("Warning: Entity generateUniqueId() almost out."
                    " Tell developer to fix UUID generator.");
    }
    return i;
}
