#ifndef COMMANDTARGET_H
#define COMMANDTARGET_H

/** CommandTarget is the interface that is pushed to
  *
  */

#include <list>
#include <string>

class CommandTarget
{
public:
    virtual int operator()(std::list<std::string>) = 0;
};

#endif // COMMANDTARGET_H
