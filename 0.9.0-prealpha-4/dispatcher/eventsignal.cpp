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

#include "eventsignal.h"


void SignalBase::disconnect(ehid_t h_id)
{
    auto it = conn_map.find(h_id);
    if(it != conn_map.end()) {
        conn_map[h_id].disconnect();
        conn_map.erase(it);
    }
}

