#ifndef COMMANDDESCRIPTOR_H
#define COMMANDDESCRIPTOR_H

#include <string>

class CommandDescriptor
{
public:
    CommandDescriptor();
    virtual bool invokedBy(const CommandDescriptor&);
    virtual bool operator==(CommandDescriptor&);

private:
    std::string command_name;
};

#endif // COMMANDDESCRIPTOR_H

// Commands still to define:
/*
  KEY_NUM_0
  KEY_NUM_1
  KEY_NUM_2
  KEY_NUM_3
  KEY_NUM_4
  KEY_NUM_5
  KEY_NUM_6
  KEY_NUM_7
  KEY_NUM_8
  KEY_NUM_9
*/
