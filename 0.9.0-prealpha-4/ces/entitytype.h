#ifndef ENTITYTYPE_H
#define ENTITYTYPE_H

#include "entity.h"

// A prototype for an entity.
// Should be able to ask an EntityType object for an Entity, and have the Type
// build the entity for you, I'd think.
//
// The entity type is what gets loaded out of files.
class EntityType {
	std::string typename_; // Not required to be the same as the header in
						   // an entity data file, particularly when the
						   // entity type is internally generated.
	std::map<std::string, std::vector<std::string>> components;
public:
	Entity makeEntity();
	bool loadEntityTypeFromFile(std::string filename);
};

#endif // ENTITYTYPE_H
