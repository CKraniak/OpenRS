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
#include <memory>

#include "entity.h"
#include "component.h"
#include "entitysignature.h"
#include "../dispatcher/statefuldispatcher.h"

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
    //
    // Well, no. That's the whole point of subclassing this: to provide
    // the specific required functionality in the subclass.

    // Dispatcher the entity gets events from
    std::shared_ptr<StatefulDispatcher> disp_;

protected:
    void pushNeededSignature(EntitySignature & sig) {
        signatures_handled_.push_back(sig);
    }

public:
    CESystem();
    bool entityHasNeededComponents(Entity & e);
    virtual int run(std::vector<Entity> & entity_list) {}
    bool connectDispatcher(StatefulDispatcher *);
};

// Subclasses of a ScriptedCESystem are meant to fill the script function map
// themselves. I want to be able to put:
//     "run_system(MOVEMENT, playerMove, __MYID__)"
// in a data file and it be able to ask the Movement system for the playerMove
// function (from the map here), and pass it an argument of "__MYID__".
class ScriptedCESystem : public CESystem {
    using scriptable_function = int (*)(std::vector<std::string>);
    std::map<std::string, scriptable_function> script_function_map;

public:
    virtual int callScriptableFunction(std::string  function_name,
                                       std::vector<std::string> argv) {}
};

#endif // CESYSTEM_H
