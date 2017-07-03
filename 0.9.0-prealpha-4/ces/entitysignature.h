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

#ifndef ENTITYSIGNATURE_H
#define ENTITYSIGNATURE_H

#include "entity.h"
#include "componentlist.h"

// Need to be able for a system to know if an entity matches an arbitrary
// "signature". Do this by seeing what components it has.
//

class EntitySignature {
    ComponentList list_;

public:
    EntitySignature(ComponentList & cl) : list_(cl) {}
	bool entityMatchesSignature(Entity & e);
};

#endif // ENTITYSIGNATURE_H
