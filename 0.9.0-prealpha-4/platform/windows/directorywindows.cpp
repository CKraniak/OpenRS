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
DirectoryWindows::getFilenamesInDirectory(std::string dir, std::string ext)
{
    auto filenames = this->getTargetsInDirectory(dir, ~DWORD(0), FILE_ATTRIBUTE_DIRECTORY);
    std::vector<std::string> retfns;
    for(auto fn : filenames) {
        if (ext.length() >  fn.length()) {
            continue;
        }
        auto ss = fn.substr(fn.length() - ext.length());
        if (! ss.compare(ext)) {
            retfns.push_back(fn);
        }
    }
    return retfns;
}

// I want to point out a few things here.
//
// When I wrote this, it was originally in CesIo. Once it got out to like
// 50 lines I knew it needed to be its own function. It's complicated and
// uncommented and there may be some bugs in it still. That's mostly because I
// banged this out in like an hour without thinking too much about how I was
// going about it, and for some reason it magically worked out after a little
// while. Good documentation would be a massive improvement here.
//
// Furthermore I believe this is sufficiently independent of the platform that
// it could go in Directory instead of DirectoryWindows. Since I'm on a Windows
// machine I don't care, but whenever you need to port this try to put this in
// Directory.cpp first.
//
// There is a line for haters and you have to go to the back.
//
// For some reason walking C:\ won't show Users or Windows or tmp. Don't know
// why that is.
std::vector<std::string> DirectoryWindows::walk(std::string base_dir,
                                                std::string ext)
{
    auto directory_ = base_dir;

    std::vector<std::vector<std::string>> directory_state_;

    auto curdir = directory_ + "/*";
    auto curdir_dirs  = getSubdirectoriesInDirectory(curdir);
    auto curdir_files = getFilenamesInDirectory(curdir, ext);

    std::vector<std::string> files;
    for(auto fn : curdir_files) {
        files.push_back(directory_
                        + "/" + fn);
    }

    std::vector<std::string> subdirs_in;
    std::string sdis = "";
    do {
        if(curdir_dirs.size() > 0) {
            auto back = curdir_dirs.back();
            auto fnbase = sdis + "/" + back;
            curdir = directory_ + sdis + "/" + back + "/*";
            curdir_dirs.pop_back();
            auto new_dirs = getSubdirectoriesInDirectory(curdir);
            if (new_dirs.size() > 0) {
                directory_state_.push_back(curdir_dirs);
                subdirs_in.push_back(back);
                sdis = "";
                for (auto s: subdirs_in) {
                    sdis += "/" + s;
                }
                curdir_dirs = new_dirs;
            }
            auto new_files = getFilenamesInDirectory(curdir, ext);
            if (new_files.size() > 0) {
                for (auto file : new_files) {
                    files.push_back(directory_ + fnbase + "/" + file);
                }
            }
        }
        else if (directory_state_.size() > 0) {
            curdir_dirs = directory_state_.back();
            directory_state_.pop_back();
            subdirs_in.pop_back();
            sdis = "";
            for (auto s: subdirs_in) {
                sdis += "/" + s;
            }
        }
    } while (directory_state_.size() > 0 || curdir_dirs.size() > 0);

    return files;
}

// Gets all directories in directory.
std::vector<std::string>
DirectoryWindows::getSubdirectoriesInDirectory(std::string dir)
{
    return this->getTargetsInDirectory(dir, DWORD(FILE_ATTRIBUTE_DIRECTORY));
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
                                        DWORD attribute,
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
        if (!(fd.dwFileAttributes & attribute) ||
             (fd.dwFileAttributes & not_attribute)) {
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
