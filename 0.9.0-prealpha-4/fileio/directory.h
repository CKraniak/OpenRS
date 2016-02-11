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
#ifndef DIRECTORY_H
#define DIRECTORY_H

// Do platform-dependent directory scanning.

// To be used to find all filenames in a given directory. Particularly needed
// for loading all of the entity definitions in ./resources/data at runtime.

// On Windows:
// - FindFirstFile
// - FindNextFile
// - FindClose
//
// Need to figure out how to reference based on where the .exe is.

class Directory
{
public:
    Directory();
};

#endif // DIRECTORY_H
