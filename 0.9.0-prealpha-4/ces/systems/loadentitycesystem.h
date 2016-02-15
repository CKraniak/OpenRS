#ifndef LOADENTITYCESYSTEM_H
#define LOADENTITYCESYSTEM_H

// I will load entities / keep a master list of entities and components here.
// I can't think of any good places to put a master list, so here we are.

#include <string>

#include "../cesystem.h"
#include "../entitylist.h"
#include "../componentlist.h"
#include "../../fileio/directory.h"

class LoadEntityCESystem
{
    EntityList entity_list_;
    ComponentList component_list_;

public:
    LoadEntityCESystem();
    int loadFromDirectory(std::string directory);
};

#endif // LOADENTITYCESYSTEM_H
