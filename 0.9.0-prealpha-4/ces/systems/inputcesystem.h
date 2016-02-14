#ifndef INPUTCESYSTEM_H
#define INPUTCESYSTEM_H

#include "../cesystem.h"

// THE SYSTEM CONNECTION IS TEMPORARY, DO NOT RELY ON IT
#include "asciidisplaycesystem.h"

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

#endif // INPUTCESYSTEM_H
