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

// The idea is to extend Dispatcher functionality by adding state information
// to events. Instead of an explicit state machine switch-case block, I'll
// have a class that has a STATE variable, and instead of hard-coding cases,
// the events themselves will be modified. Essentially, that means handlers
// will have to be written in a state-aware manner; the match string will have
// to account for state information.

// How it works is it's like the normal event system, except it pushes state
// information onto events as they are emmited. So, for example, let's say I
// have two states: menu and game. And let's say I have an event, on_numpad.
// I tell the StatefulDispatcher to put the system in the menu state, and it
// fires a special event with type "on_state_change" and string argument "menu".
// Thereafter, if an on_numpad event is emitted, then before it is sent to be
// dispatched, the StatefulDispatcher appends a special "state_menu" event type
// to the event.

// The StatefulDispatcher will be responsible for registering and de-registering
// its modified events, and keeping track of them.

#ifndef STATEFULDISPATCHER_H
#define STATEFULDISPATCHER_H

#include <string>
#include <map>

#include "dispatcher.h"

class StatefulDispatcher
{
    // I can't quite remember the train of thought I had to suggest it, but
    // I believe I will need information about the previous state in some
    // handlers.
    std::string state_, prev_state_;
    // This class essentially manages a dispatcher's IO, adding special things
    // to most events, and passing through most handlers.
    //
    // I would have subclassed, but since a lot of Dispatcher's member functions
    // are templated, I wouldn't be able to virtualize them, so it's pretty
    // useless. So I tack on my extra functionality by wrapping the class with
    // another one.
    Dispatcher dispatcher_;
    // A map from an event that has no state to one that does.
    //
    // This is so event emmiters (like an input system) don't need to care
    // about state, while we can continue to keep a registry of events anyway.
    // We could register a state-added event each time a stateless event is
    // emitted, but the Dispatcher was not made for that operating mode; it
    // would recompute the cache on each emit, and burn a shitload of time.
    // So we will keep a cache of eid_t's that we've already asked the
    // Dispatcher to cache. That way, we don't ask for the Dispatcher to re-do
    // the same cache construction every time.
    //
    // When someone asks us to emit an eid_t, we look up the eid_t in this map,
    // and if it's there we have a map of states to modified events' eid_ts.
    // So, given some event, we try to register and get its eid_t. Then we look
    // it up here with the current state to see if the state has been mapped.
    // If so, we just tell the Dispatcher to emit the state, and if not, we
    // modify the event, regiester it, store the eid_t here, then emit.
    std::map<eid_t, std::map<std::string, eid_t>> e_2_state_e_map_;

public:
    StatefulDispatcher();
};

#endif // STATEFULDISPATCHER_H
