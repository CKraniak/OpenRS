#include "observer.h"
#include "commanddescriptor.h"
#include "commandtarget.h"

Observer::Observer()
{
}

int Observer::emitSignal(const CommandDescriptor & signal)
{
    int retval = 0;
    OBS_ARGV_TYPE arglist;
    arglist.push_front("TEST");
    for(    OBS_TARGET_MAP_ITER_TYPE it = target_map_.begin();
            it != target_map_.end();
            it++) {
        if ((*(it->first))->invokedBy(signal)) {
            it->second(arglist); // Call the slots connected to the signal
        }
    }
    return retval;
}
