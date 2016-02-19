/* Copyright (C) 2016, Chesley Kraniak
 *
 * This code is distributed under the terms of the GPLv3 license, which should
 * have been included with this file in the root directory as "LICENSE.txt". If
 * you do not have a copy of the license, see:
 *     http://www.gnu.org/licenses/gpl-3.0.txt
 * or:
 *     https://github.com/CKraniak/OpenRS
 * for the license.
 *
 * dispatcher.h: Defines the Dispatcher class.
 *
 * The Dispatcher is the core of the event system in OpenRS. A user will define
 * one or many events and one or many event handlers, and these will be
 * registered with the Dispatcher. When an event is fired (using emitEvent()),
 * the emitted event will call all of the matching event handlers. Since the
 * connections to the Dispatcher aren't strict, this means a MovementSystem
 * doesn't have to know about an AudioSystem in order to tell it to play a
 * 'footstep' sound; the MovementSystem essentially "yells" that a movement has
 * happened, and the AudioSystem will listen for such yells itself and fire the
 * correct sound if it hears the correct yell.
 *
 * One of the key components of the system is the ability to match an arbitrary
 * event to an arbitrary handler. There are no privileged or special events; the
 * heirarchy is defined by the user of the Dispatcher. An event has one or many
 * "types" associated with it (e.g. a numpad 7 event may be a "keypress_type", a
 * "numkeypress_type", and a "num7keypress_type" all at once), and the handlers
 * have a matching code they define that can match types arbitrarily (e.g.
 * "numkeypress_type AND NOT num5keypress_type"). This way, the events can have
 * an arbitrary number of meanings, and the handlers can handle an arbitrary
 * set of events, based on the events' declared types.
 *
 * The user defines both event types and handler match strings on their
 * instantiation. An event's types is a vector of strings; a handler's match
 * string is a single string with keywords AND, OR, NOT and key symbols '(' and
 * ')'. An empty handler string matches nothing and is never run.
 *
 * The system works on Event *objects*, but EventHandler *classes*. This is so
 * the handlers can abuse C++'s operator() capability. There are other ways of
 * implementing this functionality, but I felt this was less intrusive than
 * most; it's particularly easier than using function pointers. This means you
 * cannot register the same handler twice. You probably shouldn't need to, since
 * any time you need to execute the same code twice, you can just loop it, and
 * if you need a runtime-defined number of loops, the type system is flexible
 * enough for you to pass in an extra loop_count variable in as a field of a
 * struct.
 *
 * I've made a special macro for defining event handler objects. You can study
 * the macro to learn how to make your own handler objects (for, say, state
 * management), or just use the macro as-is. However, since it's a preprocessor
 * macro, care should be used in how you pass things to it; in particular,
 * statements with side effects should not be passed in, except in the DEF
 * argument that defines the handler's code (which is only pasted once). I don't
 * have a better way of doing this right now, but I will look into making it
 * better later on.
 */
#ifndef DISPATCHER_H
#define DISPATCHER_H

// NOTE: Dispatcher currently does not work with reference types!
// NOTE: Types must have a public zero-argument constructor to function with
//       the Dispatcher!

#include <string>
#include <queue>
#include <type_traits>

#include "../utility.h"
#include "gameevent.h"
#include "gameeventhandler.h"
#include "eventsignal.h"

const ehid_t FIRST_HANDLER_ID = 1;
const eid_t FIRST_EVENT_ID = 1;

class Dispatcher
{
    // The bases are used so I don't have to template the Dispatcher class.
    // The callee shouldn't need to care if Dispatcher has a template or not.

    // Map of event ids to boost::signals2() signals
    // The handlers are connected to signals of matching event ids when either
    // an event or a handler is registered.
    std::map<eid_t, SignalBase*> sig_map;
    // Map of event ids to events, also serves as a list of events
    std::map<eid_t, EventBase* > event_list;
    // Map of handler ids to handlers, also serves as a list of handlers
    std::map<ehid_t, HandlerBase* > handler_list;
    // Could also do an event queue
    // Would cast void* to type T, whatever that type is, to enable data passing
    // Well, the event already has data inside of it, so the void* isn't needed
    //
    // NEW DESIGN THING: make this a call stack, only use it to prevent
    // infinite recursion.
    //std::queue<int> event_queue; // Event ID queue

    // The caller is either this or a substitute form of this. It's meant to
    // give handlers a way to emit events themselves.
    void * caller_;

    ehid_t getFirstUnusedHandlerId();
    eid_t  getFirstUnusedEventId();

    // Both of these functions return an invalid handle if the passed-in objects
    // weren't registered first.
    template <class T> eid_t  getEventId(GameEvent<T> &e);
    template <class T> ehid_t getHandlerId(GameEventHandler<T> &h);

    bool isHandlerInList(); // I forgot what I was doing with this ...

    // If I need to debog events, a function like this may come in handy
    //void printEventsToFile();

public:
    Dispatcher() : caller_(this) {}
    // Must delete any pointers in the maps
    virtual ~Dispatcher();

    // Unlike events, the handlers must be directly registered. The operator()
    // can't be specified in a constructor without some twisted function
    // pointers. At least, I don't think they can be.
    template <class T, class A> ehid_t registerHandler(A& hnd);
                                int    unregisterHandler(ehid_t hnd_id);

                       // Register predefined event e
                       // If override is false, an event with a different
                       // name but the same type list will be added to the
                       // event list and signal map (this enables 2 of the
                       // "same" events to exist.
    template <class T> eid_t registerEvent(GameEvent<T>& e, bool override);
                       int   unregisterEvent(eid_t e_id);
    template <class T, class A> int emitEvent(GameEvent<T>& e,
                                              A register_if_not_present);
    template <class T> int emitEvent(eid_t e_id,
                                     typename EventSignal<T>::force_type data);
    template <class T> int emitEvent(eid_t e_id);
    template <class T> void setData(int e_id, T data);

    void * setCaller(void * caller = nullptr) {
        caller_ = (caller == nullptr ? this : caller);
        return caller_;
    }
    void * resetCaller() { return setCaller(); }

    static void test();

    // Might implement later if thought useful:
    // int         getEventIdFromName(std::string name);
    // std::string getEventTypenameFromId(int e_id);
};

template <class T>
ehid_t Dispatcher::getHandlerId(GameEventHandler<T> &hnd) {
    for(auto it = handler_list.begin(); it != handler_list.end(); ++it) {
        if (hnd == *(it->second)) {
            return it->first;
        }
    }
    return -1;
}

template <class T, class A>
ehid_t Dispatcher::registerHandler(A& hnd)
{
    // Check to see if the handler is already listed
    // Need to change this to use getHandlerId
    for(    auto it = handler_list.begin();
            it != handler_list.end();
            ++it) {
        if (hnd == *(it->second)) {
            return it->first;
        }
    }
    // Push handler to the handler list
    ehid_t new_id = getFirstUnusedHandlerId();
    handler_list[new_id] = new A(hnd);
    // For each event in the event list
    // If the handler runs on it, connect it to the signal
    // :TODO: split the Event and Handler matching pieces into their own
    //        functions.
    for(    auto it = event_list.begin();
            it != event_list.end();
            ++it) {
        if (hnd.shouldRunOnEvent(*(it->second))) {
            // Check to see if the signal exists, and if not make it
            if(sig_map.find(it->first) == sig_map.end()) {
                sig_map[it->first] = new EventSignal<T>();
            }
            static_cast<EventSignal<T>*>(sig_map[it->first])->connect(hnd,
                                                                      new_id);
        }
    }
    return new_id;
}

template <class T>
eid_t Dispatcher::getEventId(GameEvent<T> &e) {
    for(auto it = event_list.begin(); it != event_list.end(); ++it) {
        if(e == *(it->second)) {
            return it->first;
        }
    }
    return -1;
}

template <class T>
eid_t Dispatcher::registerEvent(GameEvent<T> &e, bool override = false)
{
    // Add event to event_list if not already on it
    eid_t e_id = getEventId(e);
    if(! override && e_id != -1) {
        return e_id;
    }
    // Create new signal and map it to the event ID
    eid_t new_id = getFirstUnusedEventId();
    event_list[new_id] = new GameEvent<T>(e);
    if(sig_map.find(new_id) == sig_map.end()) {
        sig_map[new_id] = new EventSignal<T>();
    }
    // Add any handlers that match the event to the signal
    for(auto it = handler_list.begin(); it != handler_list.end(); ++it) {
        // The cast is unnecessary, but gets my editor to autocomplete
        if(static_cast<HandlerBase*>(it->second)->shouldRunOnEvent(e)) {
            static_cast<EventSignal<T> *>(sig_map[new_id])->
                connect(* dynamic_cast<GameEventHandler<T>*>(it->second),
                        it->first);
        }
    }
    return new_id;
}

// Note: register_if_not_present both sregister AND de-registers
template <class T, class A>
int Dispatcher::emitEvent(GameEvent<T>& e,
                          A register_if_not_present = true) {
    static_assert(std::is_same<A, bool>::value,
                  "emitEvent(GameEvent<T>&, bool) called with non-bool");
    // Check registration status.
    eid_t eid = getEventId(e);
    bool do_register = false;
    int retval;
    if(eid == -1) { // If event not registered
        if(! register_if_not_present) { // return if the override isn't enabled
            return 0;
        }
        do_register = true;
        eid = registerEvent(e); // otherwise, go ahead and register
    }
    // Execute the event.
    if (e.hasData()) {
        retval = emitEvent<T>(eid, e.getData());
    }
    else {
        retval = emitEvent<T>(eid, T());
    }
    // Unregister after executing, as the function should leave the Dispatcher
    // in its previous state.
    if(do_register) {
        unregisterEvent(eid);
    }
    return retval;
}

// NOTE:
// I've found that if you call this without explicitly specifying T, the
// compiler will get T from the data argument. THIS CAN DEDUCE THE WRONG TYPE!
// Segfaults happen when this happens.
//
// Note: this typename nonsense exists solely to force the compiler to make
// this a non-deduced context, or in other words, the caller is FORCED to write
// something like "emitEvent<int>(args)" and not "emitEvent(args)", specifying
// the type of the template. Failure to specify can end up with a wrongly casted
// EventSignal, which leads to segfaulting.
//
// This function is the "core" emitEvent function, as the other two signatures
// call this one to do the actual emitting.
template <class T>
int Dispatcher::emitEvent(eid_t e_id,
                          typename EventSignal<T>::force_type data)
{
    // dynamic_cast< GameEvent<T> * >(event_list.at(e_id))->setData(data);
    // throws if e_id isn't in the sig_map.
    // --> USED TO throw, switched from 'at' to 'find' so it shouldn't anymore
    if(sig_map.find(e_id) != sig_map.end()) {
        return
            dynamic_cast<EventSignal<T>*>(sig_map.at(e_id))->execSignal(caller_,
                                                                        data);
    }
    return 0;
}

template <class T>
int Dispatcher::emitEvent(eid_t eid) {
    if(event_list.find(eid) != event_list.end()) {
        GameEvent<T> * ptr = dynamic_cast<GameEvent<T>*>(event_list[eid]);
        if (ptr->hasData()) {
            return emitEvent<T>(eid, ptr->getData());
        }
        else {
            return emitEvent<T>(eid, T());
        }
    }
    return 0;
}

#endif // DISPATCHER_H
