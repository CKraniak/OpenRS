#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include "entitytype.h"

// Look into having the factory emit when it creates any entities.

class EntityFactory {
	std::map<std::string, EntityType> type_map; // typename, entity_type pairs

public:
	Entity makeEntity(std::string type);
	bool loadFiles(std::vector<std::string> filename_vec);
	void pushNewType(std::string typename_, EntityType & type_);
	void removeType(std::string typename_);
};

#endif // ENTITYFACTORY_H
