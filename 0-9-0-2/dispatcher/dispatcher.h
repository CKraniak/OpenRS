#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <string>
#include <queue>
#include "gameevent.h"
#include "gameeventhandler.h"

#include "boost/signals2.hpp"

#include "../utility.h"

typedef int ehid_t; // Handler ID type
typedef int eid_t;  // Event ID type

// Interface to the signals. Serves to isolate the template to subclasses.
class SignalBase
{


protected:
    virtual void isDerived() = 0; // forcing a pure virtual base class

public:
    SignalBase() {}
//    template <class T> int  execSignal(T& data);
//    template <class T> void connect(GameEventHandler<T>& hnd);
    virtual void disconnect(HandlerBase* hnd) = 0;
//    template <class T> int  operator()(T data);
};

template <class T> class EventSignal : public SignalBase {
    boost::signals2::signal<int (T)> sig;
    void isDerived() {} // concrete
    std::map<ehid_t, boost::signals2::connection> conn_map;

public:
    int  execSignal(T& data);
    void connect(GameEventHandler<T> hnd, ehid_t h_id);
    void disconnect(HandlerBase* hnd);
    int  operator()(T& data);

    //typedef typeof(T) force_type; // used to force a non-deduced context in
                                  // emitEvent() later on.
    using force_type = T;
};

template <class T>
int EventSignal<T>::execSignal(T &data)
{
    // Weird call
    return operator()(data);
}

template <class T>
void EventSignal<T>::connect(GameEventHandler<T> hnd, ehid_t h_id)
{
    sig.connect(hnd);
}

template <class T>
void EventSignal<T>::disconnect(HandlerBase *hnd)
{
    GameEventHandler<T> * p = dynamic_cast<GameEventHandler<T>*>(hnd);
    sig.disconnect(*p); // This relies on operator== to function.
                          // operator== currently checks the typeids.
                          // Handlers need different classes to be useful,
                          // so this shouldn't be too big of a deal ... yet.
}

template <class T>
int EventSignal<T>::operator()(T& data)
{
    return *sig(data);
}

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
    std::queue<int> event_queue; // Event ID queue

    ehid_t getFirstUnusedHandlerId();
    eid_t  getFirstUnusedEventId();

    template <class T> eid_t  getEventId(GameEvent<T> &e);
    template <class T> ehid_t getHandlerId(GameEventHandler<T> &h);

    bool isHandlerInList();

public:
    Dispatcher();

    // Unlike events, the handlers must be directly registered. The operator()
    // can't be specified in a constructor without some twisted function
    // pointers. At least, I don't think they can be.
                       template <class T, class A>
                       ehid_t registerHandler(A& hnd);
                       int    unregisterHandler(ehid_t hnd_id);

                       // Register predefined event e
                       // If override is false, an event with a different
                       // name but the same type list will be added to the
                       // event list and signal map (this enables 2 of the
                       // "same" events to exist.
    template <class T> eid_t registerEvent(GameEvent<T>& e, bool override);

                       eid_t registerEvent(std::vector<std::string> type_list,
                                           std::string name = "");
                       int   unregisterEvent(eid_t e_id);
    template <class T> int emitEvent(GameEvent<T>& e, bool);
    template <class T> int emitEvent(eid_t e_id,
                                     typename EventSignal<T>::force_type data);
    template <class T> int emitEvent(eid_t e_id);
    template <class T> void setData(int e_id, T data);

    int         getEventIdFromName(std::string name);
    std::string getEventTypenameFromId(int e_id);

    static void test();
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
            static_cast<EventSignal<T> *>(sig_map[it->first])->connect(hnd, new_id);
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
eid_t Dispatcher::registerEvent(GameEvent<T> &e, bool override = true)
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

template <class T>
int Dispatcher::emitEvent(GameEvent<T>& e,
                          bool register_if_not_present = true) {
    eid_t eid = getEventId(e);
    bool do_register = false;
    int retval;
    if(eid == -1) {
        if(! register_if_not_present) {
            return 0;
        }
        do_register = true;
        eid = registerEvent(e);
    }
    if (e.hasData()) {
        retval = emitEvent<T>(eid, e.getData());
    }
    else {
        retval = emitEvent<T>(eid, T());
    }
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
template <class T>
int Dispatcher::emitEvent(eid_t e_id,
                          typename EventSignal<T>::force_type data)
{
    // dynamic_cast< GameEvent<T> * >(event_list.at(e_id))->setData(data);
    // throws if e_id isn't in the sig_map.
    // --> USED TO throw, switched from 'at' to 'find' so it shouldn't anymore
    if(sig_map.find(e_id) != sig_map.end()) {
        return
            dynamic_cast<EventSignal<T>*>(sig_map.at(e_id))->execSignal(data);
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
