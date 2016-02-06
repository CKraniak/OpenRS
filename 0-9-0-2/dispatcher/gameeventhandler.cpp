#include <vector>
#include <typeinfo>
#include "gameeventhandler.h"

//template <class T> GameEventHandler<T>::GameEventHandler()
//{
//}


//bool GameEventHandler::shouldRunOnEventId(int e_id, Dispatcher parent)
//{

//}

bool HandlerBase::shouldRunOnEvent(EventBase& e)
{
    // Be sure the handler and event share data types.
    if (data_type_name.compare(e.getDataTypeName()) != 0)
        return false;
    std::vector<std::string> e_types = e.getTypes();
    return bool_tree.evaluate(e_types);
}

bool HandlerBase::operator==(const HandlerBase &that) const
{
    if(typeid(*this) == typeid(that)) {
        return true;
    }
    return false;
}
