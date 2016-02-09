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

void SignalBase::disconnect(ehid_t h_id)
{
    auto it = conn_map.find(h_id);
    if(it != conn_map.end()) {
        conn_map[h_id].disconnect();
        conn_map.erase(it);
    }
}

ehid_t Dispatcher::getFirstUnusedHandlerId()
{
    ehid_t new_id = 1;
    while(handler_list.find(new_id) != handler_list.end()) {
        ++new_id;
    }
    return new_id;
}

eid_t Dispatcher::getFirstUnusedEventId()
{
    eid_t new_id = 1;
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

void Dispatcher::test()
{
    // TEST CASES:
    // 1 single dispatcher, single event, single handler
    //   1.1  no data, no registration of event, calls handler ok
    //   1.2  same as 1.1, except register event and emit with no data
    //   1.3  same as 1.2, except emit with data (int)
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
    //   1.14
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
