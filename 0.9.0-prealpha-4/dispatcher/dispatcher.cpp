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
#include <typeinfo>

#include "dispatcher.h"

ehid_t Dispatcher::getFirstUnusedHandlerId()
{
    ehid_t new_id = FIRST_HANDLER_ID;
    while(handler_list.find(new_id) != handler_list.end()) {
        ++new_id;
    }
    return new_id;
}

eid_t Dispatcher::getFirstUnusedEventId()
{
    eid_t new_id = FIRST_EVENT_ID;
    while(event_list.find(new_id) != event_list.end()) {
        ++new_id;
    }
    return new_id;
}

// virtual
Dispatcher::~Dispatcher()
{
    // Delete all of the second elements of each map
    // This C++11 stuff is noice
    for(auto pair : sig_map) {
        delete std::get<1>(pair);
    }
    for(auto pair : event_list) {
        delete std::get<1>(pair);
    }
    for(auto pair : handler_list) {
        delete std::get<1>(pair);
    }
}

// Returns 1 if handler was found and unregistered, 0 if not found
int Dispatcher::unregisterHandler(ehid_t hnd_id)
{
    auto it_h = handler_list.find(hnd_id);
    if(it_h != handler_list.end()) {
        // Remove any calls to hnd_id in the signals
        for(auto it = sig_map.begin(); it != sig_map.end(); ++it) {
            it->second->disconnect(hnd_id);
        }
        delete it_h->second; // the handler WAS new'd in registerHandler
        handler_list.erase(it_h); // BEWARE OF MEMORY LEAK
        return 1;
    }
    return 0;
}

//eid_t Dispatcher::registerEvent(std::vector<std::string> type_list,
//                                std::string name)
//{
//    ERR_MSGOUT("in registerEvent(vec<str>, str); "
//               "this function not implemented yet");
//    // Create a new event
//    // Pass it to registerEvent(GameEvent<T> &e) with T as void;
//}

int Dispatcher::unregisterEvent(eid_t e_id)
{
    // Find event's EventBase
    auto e_it = event_list.find(e_id);
    // Make sure event ID points to registered events
    if(e_it == event_list.end()) {
        return 0;
    }
    // Find event's SignalBase
    auto sig_it = sig_map.find(e_id);
    // Delete both
    delete (e_it->second);
    event_list.erase(e_it);
    if(sig_it != sig_map.end()) {
        delete (sig_it->second);
        sig_map.erase(sig_it);
    }
    return 1;
}

// Event name, data type, match string, and handler function.
GE_HND(test_handler_1, int, "test_1", {
           Dispatcher * parent_disp = static_cast<Dispatcher *>(parent);
           GameEvent<int> e("sub-event", {"sub_event"}, input);
           parent_disp->emitEvent(e, true);
           return input;
})

// So how would our numkey event propogation look between systems?
//
// Press numpad 7:
// Windows sends a keydown event to MainWndProc in MLInterface              -->
//
// The MLInterface would just kick out an event of types:
//     {"numkey", "numkey_7", "any_key"}, data: int = 7 (or char = '7')
// to the SystemGroup's StatefulDispatcher (in state "game")                -->
//
// StatefulDispatcher appends event type "state_game" and re-emits          -->
//
// The InputSystem will have registered a handler to the match-string:
//     "state_game AND numkey"
// This would look up the rule for what to do (as it's controlled by the
// script in the entity) and do that. It would have a list of all the entities
// that define an "on_numpad" rule. There will be only one, for now. The rule
// is in the player entity, and it will be:
//     "run_system(MOVEMENT, movePlayer, __MYID__)"
// which would rethrow an event of type(s):
//     {"system_movement"}, data: char = '7', int = player entity's id      -->
//
// The player movement system will have registered a handler to match-string:
//     "system_movement"
// which, when given the arguments, will emit an event of type:
//     {"system_grid", "request_moveto"}, data: position = position to move to (x and y)
//                                              type = player type (i.e. not shelf-able, etc.)
//                                                                          -->
//
// The grid will have registered a handler with match-string:
//     "system_grid AND request_moveto"
// It will start by looking to see if there's a wall-type terrain there. If so,
// it returns a BLOCKED to the movement system. If not, it looks to see what
// entities are in that location. What is the blocking rule? There should be a
// blocking_rule entity that has components of type "rule" and data
// "type1:type2" (read: type1 blocks type2). Blocking types are determined by
// an entity's blocktype, right now. So a player has "blocktype=player", an item
// may have "blocktype=cannot_shelf", and a zombie would have "blocktype=zombie"
// or somesuch. The block rule here would be "rule=zombie:player" (zombie blocks
// player). So the grid will find all the items in the square and look up their
// blocktypes, find the player's blocktype, and see if any of the rules say to
// block the player. If there's no block rule, it updates the grid and returns
// an OK to the movement system (which would return all the way up the stack).
// Otherwise, it would again return a BLOCKED return value.                 <--
//
// The player movement system now attempts to fight whatever blocked it. It
// sends an event of type:
//     {"system_combat", "attack"}, data: alliance=player,
//                                        power=attackpower,
//                                        location=(x, y)                   -->
//
// The combat system will have registered a handler with match-string:
//     "system_combat AND attack"
// It will look at the location for a target, based on alliance rules. If it
// finds something that can be damaged by the attacker, it applies the damage
// to their health and checks to see if it's dead. If it did die, it fires an
// event for that; we won't consider that here. Else, it fires an event of type:
//     {"entity_damaged"}, data: entity=eid, damage=dmg_amount
// I will probably have some rendering stuff listening for that event, but not
// yet. Once the damage is applied, it returns OK to the movement system.   <--
//
// The player movement system gets the OK from the combat system, and returns
// OK.                                                                      <--
//
// The input system gets the OK, then looks to see if anyone else accepts the
// on_numpad event. When it finds nobody, it returns OK.                    <--
//
// StatefulDispatcher returns to MLInterface / main loop.                   <--
//
// We get back to the Windows message pump.

// WHAT DOES THIS MEAN?

// It means I need to implement a Dispatcher that doesn't block until an event
// is complete, since it will rethrow a bunch. So the queue is not needed yet,
// if ever.


GE_HND(dispatcher_test_event_A____, int, "s1", {
//           ERR_MSGOUT(std::string("s1, s1: " + std::to_string(input)).c_str());
           return 0;
       })
GE_HND(dispatcher_test_event_B____, int, "s1", {
           ERR_MSGOUT("s1, s2");
           return 1;
       })
GE_HND(dispatcher_test_event_C____, int, "s2", {
           ERR_MSGOUT("s2, s1");
           return 2;
       })
GE_HND(dispatcher_test_event_D____, int, "s2", {
           ERR_MSGOUT("s2, s2");
           return 3;
       })
GE_HND(dispatcher_test_event_E____, int, "s3", {
           ERR_MSGOUT("s3, s1");
           return 4;
       })
GE_HND(dispatcher_test_event_F____, int, "s3", {
           ERR_MSGOUT("s3, s2");
           return 5;
       })
// The following state can run any event that's not of type "none".
GE_HND(dispatcher_test_event_ANY____, int, "NOT none", {
           ERR_MSGOUT("sANY");
           return 99;
       })

void Dispatcher::test()
{
    // Test 1.1: no data, no registration of event, calls handler ok
    do {
        Dispatcher sd;
        GameEvent<int> e1("e1", {"s1"});
        sd.registerHandler<int>(dispatcher_test_event_A____);
        sd.emitEvent(e1, true);
    } while (0);
    // Test 1.2: same as 1.1, except register event and emit with no data
    do {
        Dispatcher sd;
        GameEvent<int> e1("e1", {"s1"});
        eid_t eid = sd.registerEvent(e1);
        sd.registerHandler<int>(dispatcher_test_event_A____);
        sd.emitEvent<int>(eid);
    } while (0);
    // Test 1.3: same as 1.2, except emit with data (int)
    do {
        Dispatcher sd;
        GameEvent<int> e1("e1", {"s1"});
        eid_t eid = sd.registerEvent(e1);
        sd.registerHandler<int>(dispatcher_test_event_A____);
        sd.emitEvent<int>(eid);
    } while (0);
    return;
}
/* void Dispatcher::test()
{
    // TEST CASES:
    // 1 single dispatcher, single event, single handler
    //   1.1
    //   1.2
    //   1.3
    //   1.4  1.3 with char
    //   1.5  1.3 with void *
    //   1.6  1.3 with class &
    //   1.7  1.6 modifying class member and getting it to stick
    //   1.8  delete handler, emit event, no segfaults
    //   1.9  re-register event, test with data
    //   1.10 double register event, test that registration only happens once
    //   1.11 1.10 after the handler was deleted once
    //   1.12 loop a handler register/delete 1000x+, see if it becomes unstable
    //   1.13 1.12, but register/delete an event instead
    //   1.14 test with multiple data types (int, char, and class)
    // 2 single dispatcher, 3 events, 7 handlers, map:
    //        E1 -> (H1, H2, H3), E2->(H4, H5, H6), E3->(H1, H4, H5, H7)
    //   2.1  register all, different data in E1-3, make sure basic emit works
    //   2.2  delete H4, see it deleted in E2 and E3
    //   2.3  remap H3 from E1 to E3 (exclusive, for all future cases), test
    //   2.4  delete E2, test to make sure H4 and H5 still ok in E3
    // 3 3 dispatchers, 3 events, 5 handlers, map:
    //        D1->(E1, E2), D2->(E1, E2, E3), D3->(E1)
    //        E1->(H1, H2, H3), E2->(H1, H4), E3->(H1, H2, H4, H5)
    //   3.1  runs correctly from all dispatchers
    //   3.2  delete E1 from D1, all still runs as expected
    //   3.3  add H5 to E1, run from all dispatchers
    //   3.4  delete H4 from E2, run from all dispatchers
    //   3.5  delete D2, check all run as expected
}
*/
