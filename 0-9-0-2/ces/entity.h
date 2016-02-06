#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>

class Entity
{
    int id_;
    std::vector<int> component_id_list_;
    std::string entity_type_name_; // e.g. "district", "player", "monster"
                                   // entity_type_name is only for readability;
                                   // any use in game logic is an error, as
                                   // logic should a.) be in a CESystem and
                                   // b.) rely on the components the entity has,
                                   // to determine what it is, not the name.

public:
    Entity();
    bool loadEntityPrototypeFromFile(std::string filename);
    int numComponentsWithName(std::string name);
};

// Need an entity collection of some kind. It may not be "pure", but I will need
// to manage groups of entities *somehow*.

#endif // ENTITY_H
