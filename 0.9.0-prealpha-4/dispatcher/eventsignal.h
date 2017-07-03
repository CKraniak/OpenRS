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

#ifndef EVENTSIGNAL_H
#define EVENTSIGNAL_H

typedef int ehid_t; // Handler ID type
typedef int eid_t;  // Event ID type

#include "gameeventhandler.h"

#include "boost/signals2.hpp"

// Interface to the signals. Serves to isolate the template to subclasses.
// This is so I can put a std::map<int, SignalBase*> in the Dispatcher
// without making Dispatcher itself a templated class.
class SignalBase
{
    using conn_type = boost::signals2::connection;

protected:
    std::map<ehid_t, conn_type> conn_map;
    virtual void isDerived() = 0; // forcing a pure virtual base class

public:
    SignalBase() {}
    virtual ~SignalBase() {}
    void disconnect(ehid_t h_id);
};

template <class T> class EventSignal : public SignalBase {
    boost::signals2::signal<int (void *, T)> sig;
    void isDerived() {} // concrete

public:
    int  execSignal(void * caller, T& data);
    void connect(GameEventHandler<T> hnd, ehid_t h_id);
    int  operator()(void * caller, T& data);

    using force_type = T;
};

// Note: the caller argument is the sending dispatcher
template <class T>
int EventSignal<T>::execSignal(void *caller, T &data)
{
    // Weird call
    return operator()(caller, data);
}

template <class T>
void EventSignal<T>::connect(GameEventHandler<T> hnd, ehid_t h_id)
{
    if(conn_map.find(h_id) == conn_map.end()) {
        conn_map[h_id] = sig.connect(hnd);
    }
}

template <class T>
int EventSignal<T>::operator()(void * caller, T& data)
{
    if (! sig.empty()) {
        return *sig(caller, data);
    }
    else {
        return 0;
    }
}

#endif // EVENTSIGNAL_H
