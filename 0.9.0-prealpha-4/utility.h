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

#include <sstream>

#include <windows.h>

#define ERR_MSGOUT(msg)                                         \
do {                                                            \
    MessageBox(NULL, msg, "Error", MB_OK | MB_ICONERROR);       \
} while (0)

#ifdef DEBUG
#define ERR_MSGOUT_DEBUG(x) ERR_MSGOUT(x)
#else
#define ERR_MSGOUT_DEBUG(x)
#endif

// The buffer is arbitrarily sized.
// Note: I haven't got this to work properly, yet.
// Update: might have fixed, untested as of 2-10-16.
#define ERR_MSGOUT_GLE(msg)                                         \
    do {                                                            \
        char * buffer;                                              \
        DWORD err_dword = GetLastError();                           \
        std::stringstream output;                                   \
        output << (msg);                                            \
        output << ", Error ";                                       \
        output << err_dword;                                        \
        output << ": ";                                             \
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |                  \
                      FORMAT_MESSAGE_ALLOCATE_BUFFER |              \
                      FORMAT_MESSAGE_IGNORE_INSERTS,                \
                      NULL,                                         \
                      err_dword,                                    \
                      0,                                            \
                      reinterpret_cast<LPTSTR>(&buffer),            \
                      2048,                                         \
                      NULL);                                        \
        output << buffer;                                           \
        MessageBox(NULL, output.str().c_str(),                      \
                            "Error", MB_OK | MB_ICONERROR);         \
    } while (0)

#endif // UTILITY_H
