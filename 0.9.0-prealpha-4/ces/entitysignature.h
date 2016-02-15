#ifndef ENTITYSIGNATURE_H
#define ENTITYSIGNATURE_H

#include "entity.h"
#include "componentlist.h"

// Need to be able for a system to know if an entity matches an arbitrary
// "signature". Do this by seeing what components it has.
//

class EntitySignature {
    ComponentList list_;

public:
    EntitySignature(ComponentList & cl) : list_(cl) {}
	bool entityMatchesSignature(Entity & e);
};

#endif // ENTITYSIGNATURE_H
