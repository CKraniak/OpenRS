#ifndef DATABASECESYSTEM_H
#define DATABASECESYSTEM_H

// Okay, this one is REALLY, REALLY the master list. It should do nothing else
// besides manage the "application wide" set of entities.
//
// So, how will this work?
//
// The issue I'm trying to solve is I need something that's the authoritative
// entity / component repository, without being a global God object thing.

#include "../cesystem.h"

class DatabaseCESystem : public CESystem
{
public:
    DatabaseCESystem();
};

#endif // DATABASECESYSTEM_H
