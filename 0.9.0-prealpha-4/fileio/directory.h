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

// Notes: if you get an empty value form getFilenamesInDirectory or
// getSubdirectoriesInDirectory, either check the validity of your dir input
// or check the error value with getError. If you know your dir was ok, it
// could be the dir was empty.
//
// Or you could check for existence before you try to read the directory.

#include <vector>
#include <string>
#include <memory>

class Directory
{
public:
    typedef enum { NONE = 0, BAD_FILEHANDLE, UNKNOWN_ERROR } DIRECTORY_ERROR;

    Directory() {}
    virtual ~Directory() {}

    virtual std::vector<std::string> getFilenamesInDirectory(std::string dir = "") = 0;
    virtual std::vector<std::string> getSubdirectoriesInDirectory(std::string dir = "") = 0;
    virtual std::string getCwd() = 0;
    DIRECTORY_ERROR getError() { return this->error_; }
    virtual bool existsFile(std::string filename, std::string path = "") = 0;
    virtual bool existsDirectory(std::string path) = 0;

    static void test();
    static std::unique_ptr<Directory> getDirectory();

protected:
    DIRECTORY_ERROR error_;
};

#endif // DIRECTORY_H
