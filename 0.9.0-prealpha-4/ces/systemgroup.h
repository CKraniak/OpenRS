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

// #include "../dispatcher/dispatcher.h"

class SystemGroup
{
    // Will probably add AUDIO later
    typedef enum {
        INPUT,
        RENDER,
        GAME
    } SYSTEM_TYPE;

public:
    SystemGroup();
};

#endif // SYSTEMGROUP_H
