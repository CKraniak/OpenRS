#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include "entitytype.h"

class EntityFactory {
	std::map<std::string, EntityType> type_map; // typename, entity_type pairs

public:
	Entity makeEntity(std::string type);
	bool loadFiles(std::vector<std::string> filename_vec);
	void pushNewType(std::string typename_, EntityType & type_);
	void removeType(std::string typename_);
};

#endif // ENTITYFACTORY_H
