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
#ifndef SYSTEMGROUP_H
#define SYSTEMGROUP_H

#include <vector>
#include <memory>

#include "cesystem.h"
#include "../dispatcher/statefuldispatcher.h"

// So how would systems be hooked in?

// Clearly, any system that needs to be hooked in ought to be hooked in by
// the SystemGroup itself.
//
// NO, it will be the caller's responsibility. It should be possible to have
// more than one system group, and that only makes sense if the SystemGroup
// works more like a dumb container.
//
// main will simply have to do the necessary hooking in and option
// interpretation. Or some delegate of main. Either way, upstream of this class.

class SystemGroup
{
    std::vector<std::shared_ptr<CESystem>> systems_;
    std::shared_ptr<StatefulDispatcher> disp_;

public:
    SystemGroup(StatefulDispatcher * disp) :
        disp_(disp) {}
    bool connectSystem(CESystem * system);
    // Initialize will cause SystemGroup to send an "init" event via the
    // dispatcher. All connected systems should run their initialization at
    // that time.
    int initialize();
};

#endif // SYSTEMGROUP_H
