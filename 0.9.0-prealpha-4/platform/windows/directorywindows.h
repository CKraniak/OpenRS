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
    std::vector<std::string> getFilenamesInDirectory(std::string dir);

private:
    // Runs the correct file attribute constant and only returns matching
    // values.
    std::vector<std::string> getTargetsInDirectory(std::string dir = "",
                                                   DWORD not_attribute = ~DWORD(0),
                                                   bool remove_dot_dirs = true);
};

#endif // DIRECTORYWINDOWS_H
