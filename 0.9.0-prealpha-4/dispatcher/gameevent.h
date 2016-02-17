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
 * gameevent.h: Defines the GameEvent<T> class.
 *
 * A game event is an object that serves to pattern-match to some group of
 * handlers. Essentially, when an event is emmited from a Dispatcher, the
 * dispatcher will use the data type and the event types listed in the given
 * GameEvent to call the matching GameEventHandlers. The only thing needed
 * here is to have said data type, event types, and (optionally) data to send
 * to the handlers.
 */

#ifndef GAMEEVENT_H
#define GAMEEVENT_H

#include <string>
#include <vector>
#include <typeinfo>

// The EventBase class facilitates the de-templating of the Dispatcher class.
// It should not be used directly in user code.
//
// This class is where the operator== for the GameEvent is defined. Dispatcher
// uses this to reject registration of the "same" event in cases where rejection
// is not explicitly overridden. Currently, the GameEvent's "name" field is
// used to determine if events are the "same" or not.
class EventBase {
protected:
    std::string name_;
    std::vector<std::string> types_;
    bool has_data;

public:
    std::string              getName()  const { return name_; }
    std::vector<std::string> getTypes() const { return types_; }
    virtual std::string      getDataTypeName() const = 0;
    bool operator==(EventBase & that) {
        return (this->name_.compare(that.getName()) == 0);
    }

    int pushType(std::string type) {
        types_.push_back(type);
        return types_.size();
    }
    std::string popType() {
        std::string rval = *(types_.rbegin());
        types_.pop_back();
        return rval;
    }
    // w00p, golf that shit
    std::string getDebugPrintString() {
        return (name_ + [this]()
                        {
                            std::string r;
                            for(auto s: types_) {
                                r += ';';
                                r += s;};
                            return r;
                        } ());
    }

    virtual ~EventBase() {}

};

// The GameEvent class describes an "event" and its attributes, including:
//   - the "event types"
//   - the data to send (optional) and its data type (not optional)
//   - an event "name", which is used to see if events are "the same"
//
// If the user wants all of the events to be able to call all of the handlers,
// the user will need to assure that they all use the same type in their
// templates. This is arguably a good idea, unless the user is interested in
// sending multiple kinds of data. Things which need to respond to multiple
// kinds of data should define a struct to contain all of the possible data
// types that might be passed; the events and handlers can then use this struct
// to pass any kind of data defined in the struct. One could also use such a
// struct to facilitate inter-handler communication, but that may make a
// program's flow ridiculously difficult to follow, if not impossible.
template <class T> class GameEvent : public EventBase
{
    T data_;
    std::string data_type_name;

private:
    // void initDataTypeName() { data_type_name = typeid(T).name(); }

public:
    GameEvent():
        data_type_name(typeid(T).name()) {}
    GameEvent(std::string name,
              std::vector<std::string> types,
              T data):
          data_(data),
          data_type_name(typeid(T).name()) {
        name_ = name;
        types_ = types;
        has_data= true;
    }

    T                        getData()  const { return data_; }
    std::string              getDataTypeName() const { return data_type_name; }
    void setData(T data) { data_ = data; has_data = true; }
    T    getData() { return data_; }
    void clearData() { data_ = T(); has_data = false; }

    bool hasData() { return has_data; }
};

#endif // GAMEEVENT_H
