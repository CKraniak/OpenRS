/* Copyright (C) 2016, Chesley Kraniak
 *
 * This code is distributed under the terms of the GPLv3 license, which should
 * have been included with this file in the root directory as "LICENSE.txt". If
 * you do not have a copy of the license, see:
 *     http://www.gnu.org/licenses/gpl-3.0.txt
 * or:
 *     https://github.com/CKraniak/OpenRS
 * for the license.
 */
#ifndef ENTITY_H
#define ENTITY_H

#include <string>

// Almost nothing but a unique number for an ID.

typedef int entity_id_t;

const entity_id_t FIRST_ENTITY_ID = 1;

class Entity
{
    friend class ECManager;
    friend class EntityType;

    entity_id_t id_;
    std::string entity_type_name_; // e.g. "district", "player", "monster"
                                   // entity_type_name is only for readability;
                                   // any use in game logic is an error, as
                                   // logic should a.) be in a CESystem and
                                   // b.) rely on the components the entity has,
                                   // to determine what it is, not the name.

    bool is_valid_; // Can only be set / unset by the ECManager.

    int generateUniqueEntityId();
public:
    Entity(std::string entity_type_name = "") :
        id_(generateUniqueEntityId()),
        entity_type_name_(entity_type_name),
        is_valid_(false) {}

    std::string getTypeName() { return entity_type_name_; }
    void setTypeName(std::string type_name) {
        entity_type_name_ = type_name;
    }

    entity_id_t getId() { return id_; }
    bool        isValid() { return is_valid_; }
};

#endif // ENTITY_H
