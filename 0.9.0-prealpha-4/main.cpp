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

#include "mlinterface/mlinterface.h"

int openrs_main(int argc, char** argv) {
    std::unique_ptr<MLInterface> mli = MLInterface::getInterface();
    mli.get()->createMainWindow();
    while ( ! mli.get()->shouldQuit() ) {
        // main loop:
        //  - look for input, process if available
        mli.get()->emptyMessagePump();
        //  - update
        //  - render
    }
    return 0;
}

