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
 * gameeventhandler.h: Defines the GameEventHandler<T> class.
 *
 * The game event handler does two major things:
 *   - it defines which events it responds to
 *   - it defines what the response is
 *
 * The events each describe a list of "event types", which are arbitrary
 * strings. The handlers can "match" the list of strings in an equally
 * arbitrary manner, using a "matching string". An event handler with a matching
 * string of "case_1 AND (case_2 OR case_3)" will match any event that has the
 * types {"case_1", "case_2"}, {"case_1", "case_3"}, or
 * {"case_1", "case_2", "case_3"}.
 *
 * The matching string is mostly unlimited. Keywords are "AND", "OR", "NOT",
 * "(", ")"; whitespace separates tokens. Outside of these rules, anything goes.
 * A matching string of "JUNK!!@1.name AND and AND (_fakedata)" will match
 * events which have types {"JUNK!!@1.name", "and", "_fakedata"}. Note the
 * keywords are case-sensitive. (Or so I think. Haven't tested that.) One other
 * restriction is the matching string has to parse properly. Failure to have
 * the proper parentheses, putting AND next to another AND or an OR (e.g.
 * "one AND OR two"), putting event types next to each other (e.g.
 * "one two AND three") do not make sense, and the parser will detect a syntax
 * error and yield an event handler that matches nothing at all.
 *
 * The other important point is how the event handler is expected to define a
 * response to the event. You must define a function which takes a single
 * argument (with type == the handler's template parameter) and returns an int.
 * Right now, the return isn't used for anything important, but it does need to
 * be int anyway. If you need to do fancy things or return fancy things, define
 * a handler with a struct template parameter and use the facilities you provide
 * in the struct itself to return fancy things or communicate state between
 * handlers. (Inter-handler communication as described here has not been tested
 * yet, so you might not be able to rely on being able to do that.)
 *
 * The GE_HND macro is the current shortcut way to make a handler. It's rather
 * kludgy, so there is a VERY good chance it gets replaced later on.
 */
#ifndef GAMEEVENTHANDLER_H
#define GAMEEVENTHANDLER_H

#include <map>
#include <typeinfo>
#include <type_traits>

#include "gameevent.h"
#include "booleancombinationtree.h"
#include "booleancombinationtreegenerator.h"

#include "../utility.h"

// This is the current shortcut way to make a handler that can immediately be
// passed to a Dispatcher object as-is. This macro would be put near the top of
// a .cpp file (not a .h) in a non-function (global-ish) namespace, and the
// call would be something like:
//    dispatcher.registerHandler(NAME);
// where NAME was what was used to define the handler's name in this macro.
//
// Arguments:
//  - NAME: the name of the handler. Should be bare (i.e. no quotation marks).
//  - TYPE: the data type passed to your handler's function, also bare.
//  - DEF:  your handler's code. DO include braces around it (i.e. "{ <code> }")
//  - MATCH: your handler's match string, as discussed else where. NOT bare, DO
//           use quotation marks.
//
// The user
#define GE_HND(NAME, TYPE, MATCH, DEF)                                           \
class hc_##NAME : public GameEventHandler<TYPE> {                              \
public:                                                                   \
    hc_##NAME(std::string s) : GameEventHandler<TYPE>(s,                       \
                                                      [](void * parent, \
                                                         TYPE input, \
                                                         void * that) -> int DEF ) {} \
}; hc_##NAME NAME(MATCH);

class Dispatcher;

// The HandlerBase class facilitates the de-templating of the Dispatcher class.
// It should not be used directly in user code.
//
// This class holds everythign that does not require a template to work. This
// includes the BCT (which implements the match string functionality) and
// the
class HandlerBase {
protected:
    BooleanCombinationTree bool_tree;
    std::string boolean_string_;
    std::string data_type_name;
    bool is_base; // <-- might switch this to a pure virtual function, like in
                  //     SignalBase.

public:
    virtual ~HandlerBase() {}
    HandlerBase(const HandlerBase& that) {
        bool_tree = that.bool_tree;
        boolean_string_ = that.boolean_string_;
        data_type_name = that.data_type_name;
    }
    HandlerBase() : is_base(true), data_type_name("") {}
    HandlerBase & operator=(const HandlerBase& that);
    virtual bool operator==(const HandlerBase& that) const;
    virtual bool shouldRunOnEvent(EventBase &e);
};

// This is the base class for user-defined event handlers. User code will use
// this by subclassing it with a specific type, and in the user's constructor,
// the GameEventHandler constructor will be called with a pointer to the user's
// handler implementation function as the second argument. See how GE_HND is
// implemented for reference.
template <class T> class GameEventHandler : public HandlerBase
{
public:
    GameEventHandler() {
        static_assert(std::is_constructible<T>::value, "GameEventHandler type must be constructible");
        data_type_name = typeid(T).name();
        is_base = false;
    }
    GameEventHandler(std::string boolean_string, int (*f)(void *, T, void *), void * that = nullptr) {
        static_assert(std::is_constructible<T>::value, "GameEventHandler type must be constructible");
        BooleanCombinationTreeGenerator gen;
        bool_tree = gen.compile(boolean_string);
        data_type_name = typeid(T).name();
        boolean_string_ = boolean_string;
        is_base = false;
        run_ = f;//orrest, run!
        that_ = that;
    }
    GameEventHandler(const GameEventHandler & that) {
        static_assert(std::is_constructible<T>::value, "GameEventHandler type must be constructible");
        this->data_type_name = that.data_type_name;
        this->is_base = false; // kind of automatic if you're in this function
        this->boolean_string_ = that.boolean_string_;
        // :TODO: This could be made better:
        this->bool_tree = BooleanCombinationTreeGenerator().compile(this->boolean_string_);
        this->run_ = that.run_;
        this->that_ = that.that_;
    }

    void setThat(void * that) { that_ = that; }

    virtual ~GameEventHandler() {}
    //virtual int operator()(T data) { return run_(data); }
    virtual int operator()(void * parent_, T data) {
        return run_(parent_, data, that_);
    }

    // run_ is a function pointer to a (user-supplied) function which is run
    // upon invoking operator(). It's basically me trying to get around the
    // subclass / signals2 crap that makes it hard to just implement operator()
    // in a subclass like I would otherwise do. I'm pretty sure this has
    // something to do with limitations in the way boost does things with
    // polymorphic base classes in signals2.
    int (* run_)(void *, T, void *);
    void * that_;
};

namespace comment_collapser__ { // Some editors can collapse / hide this block.

// *******************************************************************
//
// What follows is my initial design train-of-thought for the Dispatcher. It
// was all written before I rewrote the majority of the Dispatcher system,
// and for the most part is what constitutes my reasoning for the design I
// chose. The first block is the most interesting part here, because it's where
// I came up with the match string concept.
//
// Note that the "ANY" handler functionality was not included; you can get an
// "ANY"-like functionality by using a matching string to the effect of:
//     "NOT <something-never-used-as-an-event-type>"
// This matches any event, so long as none of your events are actually of the
// type "<something-never-used-as-an-event-type>". (Things happen sometimes ...)
//
// *******************************************************************

// I want to be able to pass in something like:
// "STATE_GAME_RUNNING AND (KEY_ESC OR KEY_ARROW)"
// and have it "compile" into the correct tree structure.

// So you'd make a GameEventHandler like:

// class MyHandler : class GameEventHandler<my_data_type>
// {
// public:
//   MyHandler() : GameEventHandler("my_compilable_string") { }
//   int operator()(my_data_type& data) {
//     // Code goes here
//   }
// }

// The list of events would populate based on the compiled string.
// You'd then register with the dispatcher:

// Dispatcher d;
// int handler_id = d.registerHandler(MyHandler());

// The event needs to be registered:

// int event_id = d.registerEvent<my_data_type>("event_name",
//                                              "my_event_type
//                                               my_2nd_event_type
//                                               my_3rd_event_type");

// Multiple types are used to represent multiple categorizations. If I want
// a "KEY_ARROW_DOWNARROW_RELEASED" event, it should also be listed as a
// generic "KEY_RELEASED" event if I choose. I can also decide events should
// only run in certain "states" by adding an "event" to this list named
// "STATE_MENU_MAIN" or similar.

// How this matches handlers is determined by the handlers themselves.
// See the compiling of matching strings above.

// Then emit the event:

// d.emitEvent<my_data_type>(eventId, data);

// If the event doesn't exist or hasn't been registered, then only an "ANY"
// handler can be run.
}

#endif // GAMEEVENTHANDLER_H
