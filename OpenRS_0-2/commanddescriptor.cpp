#include "commanddescriptor.h"


#ifdef CPP_DEBUG_OBS_BOOST
#include <iostream>
#endif // CPP_DEBUG_OBS_BOOST

CommandDescriptor::CommandDescriptor()
{
}

bool CommandDescriptor::invokedBy(const CommandDescriptor &)
{
#ifdef CPP_DEBUG_OBS_BOOST
    std::cout << "CommandDescriptor::invokedBy()" << std::cout;
#endif // CPP_DEBUG_OBS_BOOST
    return true;
}

bool CommandDescriptor::operator ==(CommandDescriptor &)
{
    return false;
}
