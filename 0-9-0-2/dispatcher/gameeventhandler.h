#ifndef GAMEEVENTHANDLER_H
#define GAMEEVENTHANDLER_H

#include <map>
#include <typeinfo>

#include "gameevent.h"
#include "booleancombinationtree.h"
#include "booleancombinationtreegenerator.h"

#include "../utility.h"

class HandlerBase {
protected:
    BooleanCombinationTree bool_tree;
    std::string boolean_string_;
    std::string data_type_name;
    bool is_base;

public:
//    virtual bool shouldRunOnEvent (EventBase& e) = 0;
    virtual ~HandlerBase() {}
    HandlerBase(const HandlerBase& that) {
        bool_tree = that.bool_tree;
        boolean_string_ = that.boolean_string_;
        data_type_name = that.data_type_name;
    }
    HandlerBase() : is_base(true), data_type_name("") {}
    HandlerBase & operator=(const HandlerBase& that);
    virtual bool operator==(const HandlerBase& that) const; // enables boost::signals2 to disconnect
    virtual bool shouldRunOnEvent(EventBase &e);
};

// Remember to init the data_type_name in all constructors.
template <class T> class GameEventHandler : public HandlerBase
{
public:
    //typedef typeof(T) force_type;
    using force_type = T;
    GameEventHandler() {
        data_type_name = typeid(T).name();
        is_base = false;
    }
    GameEventHandler(std::string boolean_string, int (*p)(T)) {
        BooleanCombinationTreeGenerator gen;
        bool_tree = gen.compile(boolean_string);
        data_type_name = typeid(T).name();
        boolean_string_ = boolean_string;
        is_base = false;
        run_ = p;
    }
    GameEventHandler(const GameEventHandler & that) {
        this->data_type_name = that.data_type_name;
        this->is_base = false; // kind of automatic if you're calling this function
        this->boolean_string_ = that.boolean_string_;
        this->bool_tree = BooleanCombinationTreeGenerator().compile(this->boolean_string_);
        this->run_ = that.run_;
    }

    virtual ~GameEventHandler() {}
//    GameEventHandler(GameEventHandler&);
//    GameEventHandler & operator=(const GameEventHandler&);
    virtual int operator()(T data) { return run_(data); }
//    virtual bool operator==(const GameEventHandler<T>& that) const {
//        return (typeid(*this) == typeid(that));
//    }

    int (* run_)(T);
    // Check the event's vector of types and see if they match the handler's
    // conditions.
    // bool shouldRunOnEvent (EventBase& e);
//    bool shouldRunOnEventId (int e_id, Dispatcher parent);
    //std::string getBooleanString() { return boolean_string_; }

};

//HandlerBase * makeHandler(const std::type_info & type) {

//}

//
// *************************************************************************
//
// The convenience subclasses are probably worthless now, in light of the
// lambdas.
//

// Convenience subclass.
class GameEventHandler_String : public GameEventHandler<std::string>
{
public:
    GameEventHandler_String() : GameEventHandler<std::string>() {}
    GameEventHandler_String(std::string boolean_string) :
        GameEventHandler<std::string>(boolean_string,
                                      [](std::string s){ return 0; }) {}
    virtual int operator()(std::string) {}
};

// Convenience subclass.
class GameEventHandler_Int : public GameEventHandler<int>
{
public:
    GameEventHandler_Int() :
        GameEventHandler<int>()
      {}
    GameEventHandler_Int(std::string boolean_string) :
        GameEventHandler<int>(boolean_string,
                              [](int i){ return 0; })
      {}
    virtual int operator()(int) {}
};

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

// If the event doesn't exist or hasn't been registered, then only an "ANY" handler can be run.


#endif // GAMEEVENTHANDLER_H
