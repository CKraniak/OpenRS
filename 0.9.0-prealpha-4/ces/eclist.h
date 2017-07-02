#ifndef ECLIST_H
#define ECLIST_H

#include <vector>
#include <map>

#include "entitylist.h"
#include "componentlist.h"
#include "entitysignature.h"

// Methods and such for relations between EntityLists and ComponentLists.
// Mostly just maps between the two types of lists. But also this is where
// component lookup optimizations can occur.

// main() will make one, fill it with entities and components and whatnot
// form files.

class ECList
{
    std::vector<EntitySignature> singnatures_;
    EntityList entities_;
    ComponentList components_;

public:
    ECList();
};

#endif // ECLIST_H
