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

#ifndef DIRECTORYWINDOWS_H
#define DIRECTORYWINDOWS_H

#include <windows.h>

#include "../../fileio/directory.h"

class DirectoryWindows : public Directory
{
public:
    DirectoryWindows();

    bool existsDirectory(std::string path);
    bool existsFile(std::string filename, std::string path);
    std::string getCwd();
    std::vector<std::string> getSubdirectoriesInDirectory(std::string dir);
    std::vector<std::string> getFilenamesInDirectory(std::string dir = "",
                                                     std::string ext = "");
    std::vector<std::string> walk(std::string base_dir = "",
                                  std::string ext = "");

private:
    // Runs the correct file attribute constant and only returns matching
    // values.
    std::vector<std::string> getTargetsInDirectory(std::string dir = "",
                                                   DWORD attribute = ~DWORD(0),
                                                   DWORD not_attribute = DWORD(0),
                                                   bool remove_dot_dirs = true);
};

#endif // DIRECTORYWINDOWS_H
