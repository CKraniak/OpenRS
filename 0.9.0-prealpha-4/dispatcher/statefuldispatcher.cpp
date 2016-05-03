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

int StatefulDispatcher::setState(std::string new_state)
{
    std::string on_leave = "on_leave_" + STATE_EVENT_TYPE_PREFIX + state_,
                on_enter = "on_enter_" + STATE_EVENT_TYPE_PREFIX + new_state;

    GameEvent<int> leaving(on_leave.c_str(), {"on_leave"}, 0);
    emitEvent<int>(leaving, true);

    prev_state_ = state_;
    state_ = new_state;

    GameEvent<int> entering(on_enter.c_str(), {"on_enter"}, 0);
    emitEvent<int>(entering, true);

    return 1; // Not currently meaningful.
}

int StatefulDispatcher::unregisterEvent(eid_t e_id)
{

}

StatefulDispatcher::StatefulDispatcher() : state_(""), prev_state_("")
{
}

GE_HND(stateful_dispatcher_test_event_A____, int, "s1 AND state_1", {
           ERR_MSGOUT("s1, s1");
           return 0;
       })
GE_HND(stateful_dispatcher_test_event_B____, int, "s1 AND state_2", {
           ERR_MSGOUT("s1, s2");
           return 1;
       })
GE_HND(stateful_dispatcher_test_event_C____, int, "s2 AND state_1", {
           ERR_MSGOUT("s2, s1");
           return 2;
       })
GE_HND(stateful_dispatcher_test_event_D____, int, "s2 AND state_2", {
           ERR_MSGOUT("s2, s2");
           return 3;
       })
GE_HND(stateful_dispatcher_test_event_E____, int, "s3 AND state_1", {
           ERR_MSGOUT("s3, s1");
           return 4;
       })
GE_HND(stateful_dispatcher_test_event_F____, int, "s3 AND state_2", {
           ERR_MSGOUT("s3, s2");
           return 5;
       })
// The following state can run any event that's not of type "none".
GE_HND(stateful_dispatcher_test_event_ANY____, int, "NOT none", {
           ERR_MSGOUT("sANY");
           // ERR_MSGOUT(reinterpret_cast<StatefulDispatcher*>(parent)->getStateString().c_str());
           return 99;
       })

void StatefulDispatcher::test()
{
    GameEvent<int> e1 = GameEvent<int>("e1", {"s1"}, 0);
    GameEvent<int> e2 = GameEvent<int>("e2", {"s2"}, 0);
    GameEvent<int> e3 = GameEvent<int>("e3", {"s3"}, 0);
    StatefulDispatcher sd;
    eid_t e1t = sd.registerEvent(e1);
    ERR_MSGOUT(e1.getDebugPrintString().c_str()); // == "e1;s1"
    eid_t e2t = sd.registerEvent(e2);
    eid_t e3t = sd.registerEvent(e3);
    ehid_t h1 = sd.registerHandler<int>(stateful_dispatcher_test_event_A____);
    ehid_t h2 = sd.registerHandler<int>(stateful_dispatcher_test_event_B____);
    ehid_t h3 = sd.registerHandler<int>(stateful_dispatcher_test_event_C____);
    ehid_t h4 = sd.registerHandler<int>(stateful_dispatcher_test_event_D____);
    ehid_t h5 = sd.registerHandler<int>(stateful_dispatcher_test_event_E____);
    ehid_t h6 = sd.registerHandler<int>(stateful_dispatcher_test_event_F____);
    ehid_t h7 = sd.registerHandler<int>(stateful_dispatcher_test_event_ANY____);
    sd.setState("1"); // 2x sANY
    ERR_MSGOUT(e1.getDebugPrintString().c_str()); // == "e1;s1"
    sd.emitEvent<int>(e1t); // "s1, s1" and sANY
    sd.emitEvent<int>(e2t); // "s2, s1" and sANY
    sd.setState("2"); // 2x sANY
    sd.emitEvent<int>(e1t); // "s1, s2" and sANY
    sd.emitEvent<int>(e2t); // "s2, s2" and sANY
    sd.emitEvent<int>(e1t); // "s1, s2" and sANY
    sd.emitEvent<int>(e3t); // "s3, s2" and sANY
}

