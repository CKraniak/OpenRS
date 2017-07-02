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

#include "component.h"
#include "../utility.h"

// Not implemented yet.

BarebonesScript::BarebonesScript(std::string script_text)
{

}

int Component::generateUniqueId()
{
    static int i = 1;
    i++;
    if (i >= (1 << 30)) {
        INFO_MSGOUT("Warning: generateUniqueId() almost out."
                    " Tell developer to fix UUID generator.");
    }
    return i;
}
