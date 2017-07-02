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
#ifndef UTILITY_H
#define UTILITY_H

#ifdef _WIN32
#include "platform/windows/utilitywindows.h"
#else
#error "Non-Windows systems not implemented; please write a utility.h for " \
    "your OS"
#endif


#endif // UTILITY_H
