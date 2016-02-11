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

#ifndef CESYSTEM_H
#define CESYSTEM_H

#include <vector>
#include <string>
#include <map>
#include <algorithm>

#include "entity.h"
#include "component.h"

class EntitySignature {
public:
    std::vector<Component> components_in_signature;
    bool entityMatchesSignature(Entity & e);
};

// :TODO: move most of these specific system definitions into better places.

class CESystem
{
    // List of entity objects cached for quick operation
    // Avoids having to iterate over a giant list from somewhere, but would
    // need to be kept current ... maybe with some events?
    std::vector<Entity> entity_cache_;
    // Components an entity needs for the entitiy to be handled by this system
    // A system should be able to define multiple things it handles, e.g.
    // the input system can handle an "on_numkey" component OR an "on_esc"
    // component. Thus, we can use multiple signatures.
    std::vector<EntitySignature> signatures_handled_;

    // Maybe have an applyToEntities that runs a lambda or somesuch over all of
    // the entities?

protected:
    void pushNeededSignature(EntitySignature & sig) {
        signatures_handled_.push_back(sig);
    }

public:
    CESystem();
    bool entityHasNeededComponents(Entity & e);
    virtual int run(std::vector<Entity> & entity_list) {}
};

class ScriptedCESystem : public CESystem {
    using scriptable_function = int (*)(std::vector<std::string>);
    std::map<std::string, scriptable_function> script_function_map;

public:
    virtual int callScriptableFunction(std::string  function_name,
                                       std::vector<std::string> argv) {}
};

const int DEFAULT_GROUND_ASCII_CHAR = 176;

// The first system should be an AsciiDisplaySystem. It will take entities with
// components "locationx", "locationy", and "asciidisplaychar". Right now, I
// will not be handling the case of multiple things in the same location.
class AsciiDisplayCESystem : public CESystem {
    std::vector<char> grid_;
    int width_;
    int height_;

    // updateGrid will iterate over the entities and push their chars to the
    // locations specified. Any
    void updateGrid(std::vector<Entity> & adces_entities);

public:
    AsciiDisplayCESystem(int width, int height) :
            width_(width),
            height_(height),
            grid_(width * height, DEFAULT_GROUND_ASCII_CHAR) {
        grid_[width * height / 2] = 'P';
    }
    std::vector<char> getRenderData();
    bool setGridChar(char c, int x, int y);
};

// Input should operate on things that define some kind of on_<input> behavior
// e.g. "on_numpad" for a component name.
// Full list of supported components:
//   - on_numpad
//   - on_esc
// Behaviors called are shown in the value of the on_<input> component.
//
// on_esc should hook into the input handler for now, to get the main loop to
// break out.
//
// on_numpad will probably only be run by the player entity for now. It's
// behavior will probably be something to point to a routine internal to the
// MovementSystem. Perhaps:
//     "on_numpad=run_system(MOVEMENT, <func_name>, entity_id)"
// I'll need something that can resolve this to the right thing. Maybe a script
// system that, in a barebones state, only supports the "run_system" call.
struct KeyInData;

class InputCESystem : public CESystem {

public:
    // Push needed components
    int onInput(KeyInData in);
};

class PlayerMovementCESystem : public ScriptedCESystem {
    int pos_x;
    int pos_y;
public:
    PlayerMovementCESystem(int x, int y) : pos_x(x), pos_y(y) {}
    int onNumpad(char , AsciiDisplayCESystem &); // Apply a transform to the
                                               // player object

};

#endif // CESYSTEM_H
