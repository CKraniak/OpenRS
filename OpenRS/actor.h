#ifndef ACTOR_H
#define ACTOR_H

//#include <windows.h>
//#include

class Actor
{
private:
    // Will make part of a superclass, "MappableObject" or "DisplayableObject"
    wchar_t display_symbol;
    // Will eventually need a way to track actions.

public:
    Actor();
};

#endif // ACTOR_H
