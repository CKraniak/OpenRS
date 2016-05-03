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
#include <memory>
#include <type_traits>

#include "dispatcher.h"

const std::string STATE_EVENT_TYPE_PREFIX = "state_";

class StatefulDispatcher
{
    // I can't quite remember the train of thought I had to suggest it, but
    // I believe I will need information about the previous state in some
    // handlers.
    //
    // These DO NOT have the prefix on them
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
    using state_to_event_map = std::map<std::string, eid_t>;
    std::map<eid_t, state_to_event_map> e_2_state_e_map_;

    // Events without state information in them
    std::map<eid_t, std::shared_ptr<EventBase>> stateless_event_list_;

    // Events that do have state information in them
    std::map<eid_t, std::shared_ptr<EventBase>> stateful_event_list_;

    template <class T> eid_t getCurrentStateEventID(eid_t e);
    template <class T> eid_t makeCurrentStateEvent(eid_t e);

    template <class T> eid_t getStateEvent(eid_t e, std::string state);
    template <class T> eid_t makeStateEvent(eid_t e, std::string state);

public:

    int setState(std::string new_state);
    std::string getCurrentState() { return state_; }
    std::string getCurrentStatePrefix() { return STATE_EVENT_TYPE_PREFIX +
                                                 state_;                    }

    // ***********************************
    //
    // Have to redefine the dispatcher calls, since I'm not subclassing due to
    // the template garbage not letting me virtualize.

    template <class T, class A> ehid_t registerHandler(A& hnd) {
        return dispatcher_.registerHandler<T, A>(hnd);
    }
    int unregisterHandler(ehid_t hnd_id) {
        return dispatcher_.unregisterHandler(hnd_id);
    }

    template <class T> eid_t registerEvent(GameEvent<T>& e, bool override);
                       int   unregisterEvent(eid_t e_id);

    template <class data_type, class bool_type = bool>
                               int emitEvent(GameEvent<data_type>& e,
                                             bool_type register_if_not_present = true);
    template <class data_type> int emitEvent(eid_t e_id,
                                             typename EventSignal<data_type>::force_type data);
    template <class data_type> int emitEvent(eid_t e_id);

    template <class T> void setData(eid_t e_id, T data) {
        // This should not trigger a stateful event creation; all event data
        // should be stored in stateless until an event is fired, at which point
        // the data will be copied over
        dispatcher_.setData<T>(e_id, data);
    }

    template <class T> T getData(eid_t e_id) {
       return dispatcher_.getData<T>(e_id);
    }

    std::string getStateString() { return STATE_EVENT_TYPE_PREFIX + state_; }

    // ***********************************

    StatefulDispatcher();
    virtual ~StatefulDispatcher() {

    }

    static void test();
};

// Man, these templates are brutal.
//
// BROOTUL!!


// Returns FIRST_EVENT_ID - 1 if the (stateless) event was not registered
// before calling.
template <class data_type>
eid_t StatefulDispatcher::getCurrentStateEventID(eid_t e)
{
    return getStateEvent<data_type>(e, this->state_);
}

// e = stateless event ID
template <class T>
eid_t StatefulDispatcher::getStateEvent(eid_t e, std::string state)
{
    // Check that the event is registered as a stateless event
    if(stateless_event_list_.find(e) == stateless_event_list_.end()) {
        ERR_MSGOUT("unregistered event");
        return FIRST_EVENT_ID - 1;
    }

    if(state_.size() == 0) {
        return e;
    }

    auto vec = e_2_state_e_map_.find(e);
    if(vec == e_2_state_e_map_.end()) {
        // TODO: Finish making the event / adding it to the map properly.
        eid_t new_e = makeStateEvent<T>(e, state);
        vec = e_2_state_e_map_.find(new_e);
        if(vec == e_2_state_e_map_.end()) {
            ERR_MSGOUT("failed to register stateful event version of event");
            return FIRST_EVENT_ID - 1;
        }
    }

    std::map<std::string, eid_t> statemap = vec->second;
    auto state_it = statemap.find(state);
    if(state_it == statemap.end()) {
        // TODO: Finish making the event / adding it to the map properly.
        return makeStateEvent<T>(e, state);
    }
    // Only gets down here if the state existed before.
    return state_it->second;
}

// Returns the eid_t of the event that makeCurrentStateEvent makes
template <class T>
eid_t StatefulDispatcher::makeCurrentStateEvent(eid_t e)
{
    return makeStateEvent<T>(e, this->state_);
}

// The state parameter DOES NOT have the prefix on it,
template <class T>
eid_t StatefulDispatcher::makeStateEvent(eid_t e, std::string state)
{
    if(state.size() == 0) {
        return FIRST_EVENT_ID - 1;
    }
    auto elist_elem = stateless_event_list_.find(e);
    if(elist_elem == stateless_event_list_.end()) {
        ERR_MSGOUT("event not in list");
        return FIRST_EVENT_ID - 1;
    }
    GameEvent<T> e_st = GameEvent<T>(*dynamic_cast<GameEvent<T>*>(elist_elem->second.get()));
    e_st.pushType(std::string(STATE_EVENT_TYPE_PREFIX) + state);
    // TODO: better checking of end() iterators and return values
    eid_t state_eid = dispatcher_.registerEvent(e_st);
    e_2_state_e_map_[e][state] = state_eid;
    stateful_event_list_[state_eid] = std::shared_ptr<GameEvent<T>>(new GameEvent<T>(e_st));
    return dispatcher_.registerEvent(e_st);
}

// Add event to the stateless event list and give it an empty state event map
template <class data_type>
eid_t StatefulDispatcher::registerEvent(GameEvent<data_type> &e,
                                        bool override = true)
{
    // Make sure it's an unknown state; check against both stateful and
    // stateless lists
    for(auto event : stateless_event_list_) {
        if (e == *event.second.get()) {
            return event.first;
        }
    }
    for(auto event : stateful_event_list_) {
        if (e == *event.second.get()) {
            return event.first;
        }
    }
    eid_t e_id = dispatcher_.registerEvent(e, override);
    // Put unknown event in stateless event list
    stateless_event_list_[e_id] =
            std::shared_ptr<EventBase>(new GameEvent<data_type>(e));
    // Make a new s2e_map, since this is a new event and should not already
    // have one
    e_2_state_e_map_[e_id] = state_to_event_map();
    // return makeCurrentStateEvent<T>(e_id); // why did I do it this way?
    // The caller should never need to care about any state information, so
    // we return the stateless ID.
    return e_id;
}

template <class data_type, class bool_type>
int StatefulDispatcher::emitEvent(GameEvent<data_type> &e,
                                  bool_type register_if_not_present)
{
    static_assert(std::is_same<bool_type, bool>::value,
                  "emitEvent(GameEvent<data_type>&, bool) called with non-bool");
    if(state_.size() != 0) {
        e.pushType(std::string(STATE_EVENT_TYPE_PREFIX) + state_);
    }
    dispatcher_.setCaller(this);
    int rval = dispatcher_.emitEvent<data_type>(e, register_if_not_present);
    dispatcher_.resetCaller();
    if(state_.size() != 0) {
        e.popType();
    }
    return rval;
}

template <class data_type>
int StatefulDispatcher::emitEvent(eid_t e_id,
                                  typename EventSignal<data_type>::force_type data)
{
    setData(e_id, data);
    return emitEvent<data_type>(e_id);
}

template <class data_type>
int StatefulDispatcher::emitEvent(eid_t e_id)
{
    // Get stateful event, make if it doesn't exist
    eid_t stateful_eid = getCurrentStateEventID<data_type>(e_id);
    // Copy data
    dispatcher_.setData<data_type>(stateful_eid, dispatcher_.getData<data_type>(e_id));
    // Run event
    dispatcher_.setCaller(this);
    int rval = dispatcher_.emitEvent<data_type>(stateful_eid);
    dispatcher_.resetCaller();
    return rval;
}

#endif // STATEFULDISPATCHER_H
