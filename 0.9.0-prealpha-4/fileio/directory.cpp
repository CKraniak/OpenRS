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
#include "directory.h"
#include "../utility.h"

#if defined(_WIN32)
    #include "../platform/windows/directorywindows.h"
#else
#error "Non-Windows systems not implemented; please write a Directory for " \
    "your OS"
#endif


// This is the OS-specific selector
std::unique_ptr<Directory> Directory::getDirectory() {
#if defined(_WIN32)
    std::unique_ptr<Directory> ptr(new DirectoryWindows());
    return ptr;
#else
#error "Non-Windows systems not implemented; please write a Directory for " \
    "your OS"
#endif
}

void Directory::test()
{
    std::unique_ptr<Directory> d = Directory::getDirectory();
    auto cwd = d->getCwd();
    INFO_MSGOUT_DEBUG((std::string("CWD: ") + cwd).c_str());
    if (d->existsDirectory(cwd)) {
        INFO_MSGOUT_DEBUG("Detected CWD is valid");
    }
    else {
        INFO_MSGOUT_DEBUG("Detected CWD is NOT valid");
    }
    if (d->existsDirectory(cwd + "randomjunkthatisntadir")) {
        INFO_MSGOUT_DEBUG("existsDirectory is freaky");
    }
    else {
        INFO_MSGOUT_DEBUG("existsDirectory functioning");
    }
    auto fnamevec = d->getFilenamesInDirectory();
    for (auto filename : fnamevec) {
        INFO_MSGOUT_DEBUG((std::string("FILENAME IN DIR: ") + filename).c_str());
    }
    auto dirs = d->getSubdirectoriesInDirectory();
    for (auto dir : dirs) {
        INFO_MSGOUT_DEBUG((std::string("DIRS IN DIR: ") + dir).c_str());
    }
    if (d->existsFile("Makefile")) {
        INFO_MSGOUT_DEBUG("Makefile found in .exe directory");
    }
    else {
        INFO_MSGOUT_DEBUG("NO Makefile found in .exe directory");
    }
    if (d->existsFile("nonsensefilethatisnonsense")) {
        INFO_MSGOUT_DEBUG("existsFile is on acid");
    }
    else {
        INFO_MSGOUT_DEBUG("existsFile is functioning");
    }
}
