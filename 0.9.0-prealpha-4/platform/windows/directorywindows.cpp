#include "directorywindows.h"

#include "../../utility.h"

DirectoryWindows::DirectoryWindows() : Directory()
{

}

// Gets all filenames in directory.
// May or may not have some unintended side effects, as I didn't exactly see
// an option for "FILES", only "NOT A  DIRECTORY". So this is currently just
// giving you not-directories rather than files. CK, 7-1-17
std::vector<std::string>
DirectoryWindows::getFilenamesInDirectory(std::string dir)
{
    return this->getTargetsInDirectory(dir, FILE_ATTRIBUTE_DIRECTORY);
}

// Gets all directories in directory.
std::vector<std::string>
DirectoryWindows::getSubdirectoriesInDirectory(std::string dir)
{
    return this->getTargetsInDirectory(dir, ~DWORD(FILE_ATTRIBUTE_DIRECTORY));
}

// Gets CWD. If directory hasn't changed, it should be the directory the
// executable is in.
//
// Strips off  trailing slash. If you need that you have to add it back on
// again.
std::string DirectoryWindows::getCwd()
{
    TCHAR filename[MAX_PATH];
    int rv = GetModuleFileName(NULL, filename, MAX_PATH);
    std::string retval(filename);
    std::size_t locn = retval.find_last_of("/\\");
    return retval.substr(0, locn);
}

// On the inefficient side, but I don't want to write any more WinAPI code.
//
// The right way to do this would probably be to  further generalize (+rename)
// the getTargetsInDirectory function and make this basically a passthrough
// to that. That's ALMOST what this is here, but instead of having
// the file looked for directly we get ALL of them and loop through the list.
//
// The easy WinAPI function will only go  up to MAX_PATH, so I''m not doing
// it here.
bool DirectoryWindows::existsFile(std::string filename, std::string path)
{
    bool exists = false;

    auto files_in_dir = this->getFilenamesInDirectory(path);
    for(auto file : files_in_dir) {
        if (filename.compare(file) == 0) {
            return true;
        }
    }
    return false;
}

// Tests say this works both with and without a trailing slash.
bool DirectoryWindows::existsDirectory(std::string path)
{
    path = std::string("\\\\?\\") + path;
    std::wstring wpath(path.begin(), path.end());
    DWORD attrs = GetFileAttributesW(wpath.c_str());
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        return false;
    }
    if (attrs & FILE_ATTRIBUTE_DIRECTORY) {
        return true;
    }
    return false;
}

// Handles long paths. Does a 64-bit filesystem switch. I don't know what that
// will do on 32-bit Windows.
std::vector<std::string>
DirectoryWindows::getTargetsInDirectory(std::string dir,
                                        DWORD not_attribute,
                                        bool remove_dot_dirs)
{
    std::string dir_ = dir;
    if (dir_.length() == 0) {
        dir_ = this->getCwd();
        dir_ += "/*";
    }

    if (dir_.length() > MAX_PATH) {
        dir_ = std::string("\\\\?\\") + dir_;
    }
    std::wstring wdir_(dir_.begin(), dir_.end());
    WIN32_FIND_DATAW fd;

    PVOID old_val;
    Wow64DisableWow64FsRedirection(&old_val);

    HANDLE search_handle = FindFirstFileW(wdir_.c_str(), &fd);
    std::vector<std::string> retvec;
    if (search_handle == INVALID_HANDLE_VALUE) {
        this->error_ = BAD_FILEHANDLE;
        return retvec;
    }

    std::wstring wstr_temp;
    std::string fd_filename;
    int code = 0;
    do {
        if (fd.dwFileAttributes & not_attribute) {
            continue;
        }
        wstr_temp = fd.cFileName;
        fd_filename = std::string(wstr_temp.begin(), wstr_temp.end());
        if (remove_dot_dirs && (! fd_filename.compare(".") ||
                                ! fd_filename.compare(".."))) {
            continue;
        }
        retvec.push_back(fd_filename);
    } while (code = FindNextFileW(search_handle, &fd));

    if (GetLastError() != ERROR_NO_MORE_FILES) {
        ERR_MSGOUT_DEBUG("Unknown error traversing files");
        retvec = std::vector<std::string>();
        this->error_ = UNKNOWN_ERROR;
        return retvec;
    }

    Wow64RevertWow64FsRedirection(old_val);

    FindClose(search_handle);

    return retvec;
}
