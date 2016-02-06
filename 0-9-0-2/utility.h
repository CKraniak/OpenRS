#ifndef UTILITY_H
#define UTILITY_H

#include <sstream>
#include <string>

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
#define ERR_MSGOUT_GLE(msg)                                         \
    do {                                                            \
        char buffer[2048];                                          \
        std::stringstream output;                                   \
        output << msg;                                              \
        output << ", Error ";                                       \
        output << GetLastError();                                   \
        output << ": ";                                             \
        output << FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,         \
                                NULL,                               \
                                GetLastError(),                     \
                                0,                                  \
                                buffer,                             \
                                2048,                               \
                                NULL);                              \
        MessageBox(NULL, output.str().c_str(),                      \
                            "Error", MB_OK | MB_ICONERROR);         \
    } while (0)

#endif // UTILITY_H
