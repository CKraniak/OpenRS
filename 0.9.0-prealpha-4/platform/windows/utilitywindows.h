#ifndef UTILITYWINDOWS_H
#define UTILITYWINDOWS_H

#include <sstream>

#include <windows.h>

#define ERR_MSGOUT(msg)                                         \
do {                                                            \
    MessageBox(NULL, msg, "Error", MB_OK | MB_ICONERROR);       \
} while (0)

#define INFO_MSGOUT(msg)                                        \
do {                                                            \
    MessageBox(NULL, msg, "Info", MB_OK | MB_ICONINFORMATION);  \
} while (0)

#ifdef DEBUG
#define ERR_MSGOUT_DEBUG(x)  ERR_MSGOUT(x)
#define INFO_MSGOUT_DEBUG(x) INFO_MSGOUT(x)
#else
#define ERR_MSGOUT_DEBUG(x)
#define INFO_MSGOUT_DEBUG(x)
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


#endif // UTILITYWINDOWS_H

