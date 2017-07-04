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

#include <string>
#include <map>

#include "component.h"

// Need to be able for a system to know if an entity matches an arbitrary
// "signature". Do this by seeing what components it has.
//

class EntitySignature {
    std::map<std::string, int> list_;

public:
    EntitySignature();
    EntitySignature(std::string component_name);
    EntitySignature(std::vector<std::string> component_names);

    int  setComponentName(std::string name, int count = 1);
    void unsetComponentName(std::string name);

    // If strict, the count must match exactly. If not, then the vector must
    // have at least as many of the component as expected.
    //
    // I.e. it's EQUAL TO vs. GREATER THAN OR EQUAL TO
    bool matchesSignature(std::vector<Component> components,
                          bool strict = true);
};

#endif // ENTITYSIGNATURE_H
