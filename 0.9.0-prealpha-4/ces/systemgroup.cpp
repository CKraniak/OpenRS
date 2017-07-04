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
#include "systemgroup.h"

bool SystemGroup::connectSystem(std::shared_ptr<CESystem> system) {
    system->connectDispatcher(disp_);
    system->connectECManager(ec_data_sys_);
    systems_.push_back(system);
    return true;
}

int SystemGroup::initialize()
{

}
