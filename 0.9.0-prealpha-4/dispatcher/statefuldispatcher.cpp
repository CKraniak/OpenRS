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
    GameEvent<int> leaving("on_leave", {"on_leave"}, 0);
    emitEvent<int>(leaving);

    prev_state_ = state_;
    state_ = STATE_EVENT_TYPE_PREFIX + new_state;

    GameEvent<int> entering("on_enter", {"on_enter"}, 0);
    emitEvent<int>(entering);

    return 1; // Not currently meaningful.
}

int StatefulDispatcher::unregisterEvent(eid_t e_id)
{

}

StatefulDispatcher::StatefulDispatcher()
{

}

GE_HND(stateful_dispatcher_test_event_A____, int, "s1 AND state_1", { return 0; })
GE_HND(stateful_dispatcher_test_event_B____, int, "s1 AND state_2", { return 1; })
GE_HND(stateful_dispatcher_test_event_C____, int, "s2 AND state_1", { return 2; })
GE_HND(stateful_dispatcher_test_event_D____, int, "s2 AND state_2", { return 3; })
GE_HND(stateful_dispatcher_test_event_E____, int, "s3 AND state_1", { return 4; })
GE_HND(stateful_dispatcher_test_event_F____, int, "s3 AND state_2", { return 5; })
// The following state can run any event that's not of type "none".
GE_HND(stateful_dispatcher_test_event_ANY____, int, "NOT none", { return 99; })

void StatefulDispatcher::test()
{
    GameEvent<int> e1 = GameEvent<int>("e1", {"s1"}, 0);
    GameEvent<int> e2 = GameEvent<int>("e2", {"s2"}, 0);
    GameEvent<int> e3 = GameEvent<int>("e3", {"s3"}, 0);
    StatefulDispatcher sd;
    eid_t e1t = sd.registerEvent(e1);
    eid_t e2t = sd.registerEvent(e2);
    eid_t e3t = sd.registerEvent(e3);
    ehid_t h1 = sd.registerHandler<int>(stateful_dispatcher_test_event_A____);
    ehid_t h2 = sd.registerHandler<int>(stateful_dispatcher_test_event_B____);
    ehid_t h3 = sd.registerHandler<int>(stateful_dispatcher_test_event_C____);
    ehid_t h4 = sd.registerHandler<int>(stateful_dispatcher_test_event_D____);
    ehid_t h5 = sd.registerHandler<int>(stateful_dispatcher_test_event_E____);
    ehid_t h6 = sd.registerHandler<int>(stateful_dispatcher_test_event_F____);
    ehid_t h7 = sd.registerHandler<int>(stateful_dispatcher_test_event_ANY____);
    sd.setState("1");
    sd.emitEvent<int>(e1t);
    sd.emitEvent<int>(e2t);
    sd.setState("2");
    sd.emitEvent<int>(e1t);
    sd.emitEvent<int>(e2t);
    sd.emitEvent<int>(e1t);
    sd.emitEvent<int>(e3t);
}

