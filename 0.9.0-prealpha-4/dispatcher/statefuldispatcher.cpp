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
#include "statefuldispatcher.h"

// Returns FIRST_EVENT_ID - 1 if the event was not registered before calling.
eid_t StatefulDispatcher::getCurrentStateEvent(eid_t e)
{
    return getStateEvent(e, this->state_);
}

// Returns the eid_t of the event that makeCurrentStateEvent makes
eid_t StatefulDispatcher::makeCurrentStateEvent(eid_t e)
{
    return makeStateEvent(e, this->state_);
}

eid_t StatefulDispatcher::getStateEvent(eid_t e, std::string state)
{
    // Check that the event is registered as a stateless event
    if(stateless_event_list_.find(e) == stateless_event_list_.end()) {
        return FIRST_EVENT_ID - 1;
    }

    auto vec = e_2_state_e_map_.find(e);
    if(vec == e_2_state_e_map_.end()) {
        // TODO: Finish making the event / adding it to the map properly.
        eid_t new_e = makeStateEvent(e, state);
        vec = e_2_state_e_map_.find(new_e);
    }

    std::map<std::string, eid_t> statemap = vec->second;
    auto state_it = statemap.find(state);
    if(state_it == statemap.end()) {
        // TODO: Finish making the event / adding it to the map properly.
        eid_t new_e = makeStateEvent(e, state);
    }
    // Only gets down here if the state existed before.
    return state_it->second;
}

eid_t StatefulDispatcher::makeStateEvent(eid_t e, std::string state)
{

}

int StatefulDispatcher::setState(std::string new_state)
{

}

int StatefulDispatcher::unregisterEvent(eid_t e_id)
{

}

StatefulDispatcher::StatefulDispatcher()
{

}

