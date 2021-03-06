/* Copyright (C) 2017, Chesley Kraniak
 *
 * This code is distributed under the terms of the GPLv3 license, which should
 * have been included with this file in the root directory as "LICENSE.txt". If
 * you do not have a copy of the license, see:
 *     http://www.gnu.org/licenses/gpl-3.0.txt
 * or:
 *     https://github.com/CKraniak/OpenRS
 * for the license.
 */

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

// *******************************

// As of 5-26-16, the above described behavior will probably not be pursued.
// The systems will each handle their own crap through the StatefulDispatcher,
// setting up handlers themselves. So the InputCESystem is now deprecated /
// useless.
struct KeyInData;

class InputCESystem : public CESystem {

public:
    // Push needed components
    int onInput(KeyInData in);
};

#endif // INPUTCESYSTEM_H
