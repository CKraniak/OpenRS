#ifndef OBSERVER_H
#define OBSERVER_H

//#include <vector>
#include <list>
#include <string>

//#include "commanddescriptor.h"
//#include "commandtarget.h"

#include "boost/signals2.hpp"
#include "boost/smart_ptr.hpp"

class CommandDescriptor;
class CommandTarget;

class Observer
{
    typedef int slot_type;

public:
    Observer();
    // \/ Not a void type; will update with the proper signals2 ret_type later
    void registerSlot(const CommandDescriptor&, CommandTarget&);
    void unregisterSlot( /* Will take the signals2 connection from above */ );
    int  emitSignal(const CommandDescriptor&);

    typedef std::list<std::string> OBS_ARGV_TYPE;

private:
    // Maps CommandDescriptors to the list of commands to run.
    // Commands stored as pointers, for now. Not sure if I need to
    // be storing as pointers or not.
#ifdef CPP_DEBUG_OBS_BOOST
public:
#endif // CPP_DEBUG_OBS_BOOST
    typedef boost::signals2::signal<int (OBS_ARGV_TYPE)> OBS_SIGNAL_TYPE;
#ifdef CPP_DEBUG_OBS_BOOST
private:
#endif // CPP_DEBUG_OBS_BOOST
   typedef boost::scoped_ptr<CommandDescriptor *>      OBS_MAP_KEY_TYPE;
   typedef std::map<OBS_MAP_KEY_TYPE, OBS_SIGNAL_TYPE> OBS_TARGET_MAP_TYPE;
   typedef OBS_TARGET_MAP_TYPE::iterator               OBS_TARGET_MAP_ITER_TYPE;
   OBS_TARGET_MAP_TYPE target_map_;
};

#endif // OBSERVER_H
