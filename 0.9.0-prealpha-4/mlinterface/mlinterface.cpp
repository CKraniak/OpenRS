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

#include "mlinterface.h"

#if defined(_WIN32)
    #include "./windows/mlinterfacewindows.h"
#else
#error "Non-Windows systems not implemented; please write a MLInterface for " \
    "your OS"
#endif


// This is the OS-specific selector
std::unique_ptr<MLInterface>
MLInterface::getInterface(std::shared_ptr<StatefulDispatcher> disp) {
#if defined(_WIN32)
    std::unique_ptr<MLInterface> ptr(new MLInterfaceWindows(disp));
    ptr.get()->init_();
    return ptr;
#else
#error "Non-Windows systems not implemented; please write a MLInterface for " \
    "your OS"
#endif
}
