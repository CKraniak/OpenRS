#ifndef ENTITYSIGNATURE_H
#define ENTITYSIGNATURE_H

#include "entity.h"
#include "componentlist.h"

class EntitySignature {
public:
	EntitySignature(ComponentList &);
	ComponentList components_in_signature;
	bool entityMatchesSignature(Entity & e);
};

#endif // ENTITYSIGNATURE_H
