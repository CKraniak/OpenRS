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

#ifndef GRAPHICSDRIVER_H
#define GRAPHICSDRIVER_H

// Interface to system-specific graphics calls
//
// The implementers sub-class from this and implement the virtual functions

class GraphicsDriver
{
public:
    GraphicsDriver();

    // I don't know what I need yet.
    virtual drawGrid() {}
    virtual drawTextLine() {}
};

#endif // GRAPHICSDRIVER_H
