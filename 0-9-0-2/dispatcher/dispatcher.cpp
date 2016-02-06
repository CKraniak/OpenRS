#include <typeinfo>

#include "dispatcher.h"

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

Dispatcher::Dispatcher()
{

}

// Returns 1 if handler was found and unregistered, 0 if not found
int Dispatcher::unregisterHandler(ehid_t hnd_id)
{
    auto it_h = handler_list.find(hnd_id);
    if(it_h != handler_list.end()) {
        // Remove any calls to hnd_id in the signals
        for(auto it = sig_map.begin(); it != sig_map.end(); ++it) {
            it->second->disconnect(it_h->second);
        }
        delete it_h->second;
        handler_list.erase(it_h); // BEWARE OF MEMORY LEAK
        return 1;
    }
    return 0;
}

eid_t Dispatcher::registerEvent(std::vector<std::string> type_list,
                                std::string name)
{
    std::cout << std::endl << "here" << std::endl;
    // Create a new event
    // Pass it to registerEvent(GameEvent<T> &e) with T as void;
}

int Dispatcher::unregisterEvent(eid_t e_id)
{

}

// Wow this shit's cray.
// Have to pass a damn funciton pointer for this to do what it's supposed to.
// God.
#define GE_HND(NAME, TYPE, DEF, MATCH)                                           \
class hc_##NAME : public GameEventHandler<TYPE> {                              \
public:                                                                   \
    hc_##NAME(std::string s) : GameEventHandler<TYPE>(s,                       \
                                                     [](TYPE input) DEF ) { \
        /* data_type_name = typeid(hc_##NAME).name();      */                       \
}  \
}; hc_##NAME NAME(MATCH);

GE_HND( EHandTest, int, {
    ERR_MSGOUT("Handler EHandTest called");
    return input;
}, "event_any")

GE_HND(EHandTest_2, int, {
    ERR_MSGOUT("Handler EHandTest_2 called");
    return input;
}, "event_any")

GE_HND(EHandTest_3, char, {
    ERR_MSGOUT("Handler EHandTest_3 called");
    return static_cast<int>(input);
}, "event_any")

void Dispatcher::test()
{
    Dispatcher d;
    ehid_t hid = d.registerHandler<int>(EHandTest);
    ehid_t hid_double = d.registerHandler<int>(EHandTest);
    ehid_t hid_2 = d.registerHandler<int>(EHandTest_2);
    ehid_t hid_3 = d.registerHandler<char>(EHandTest_3);
    std::vector<std::string> types;
    types.push_back("event_any");
    GameEvent<int> e("e1", types, 0);
    GameEvent<char> e2("e2", types, 0);
    e2.setData(12);
    eid_t eid = d.registerEvent(e);
    eid_t eid2 = d.registerEvent(e2);
    int q = d.emitEvent<int>(eid, 35);
    int r = d.emitEvent<char>(eid2, 10);
    int u = d.emitEvent<char>(eid2); // uses setData data
    //    d.unregisterHandler(hid_2);
    int v = d.emitEvent<int>(eid, 25);
    // d.unregisterHandler(hid);
    std::stringstream ss;
    ss << "Int was: " << q << " and " << r << ", second was " << u << " and " << v;
    ERR_MSGOUT(ss.str().c_str());
}
