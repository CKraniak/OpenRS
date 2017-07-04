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

#include "cesystem.h"

CESystem::CESystem()
{
}

void CESystem::connectDispatcher(std::shared_ptr<StatefulDispatcher> disp)
{
    disp_ = disp;
    onDispatcherAvailable();
}

void CESystem::connectECManager(std::shared_ptr<ECManager> ec_data_sys)
{
    ec_data_sys_ = ec_data_sys;
    onECManagerAvailable();
}
