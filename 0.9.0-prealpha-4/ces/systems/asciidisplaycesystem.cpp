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

#include "asciidisplaycesystem.h"

// So currently this system does the everything on its own.
//
// What I want in the long run is for the code to be arranged as follows:
// - DisplayCES: display things
//    - AsciiDisplayCES: I/O for the AsciiRenderer and AsciiCore, interface
//      to DisplayCES
//       - AsciiRenderer: Displays AsciiCore data
//          - GraphicsDriver: provides line and quad drawing ability which
//            renderer will use
//       - AsciiCore: Ascii window manager functionality
//
// Update cycle:
//    - main sends any input events out
//       - input events affect whatever they affect, state changes, player
//         movements, etc.
//    - main sends a tick event with a delta t argument
//    - DisplaySystem calls down to propogate tick
//    - AsciiDisplay propogates to AsciiCore
//    - AsciiCore grabs anything it needs from things that have marked
//      themselves dirty (an "update" event, with a list arg that things
//      push their info onto, I suppose)
//
// Render cycle:
//    - main loop sends render event, caught by DisplayCES
//    - DisplayCES gets the right sub-Display CES (Ascii) and rethrows event
//    - AsciiDisplayCES gets render data  out of AsciiCore and sends to
//      the AsciiRenderer
//    - AsciiRenderer uses GraphicsDriver to write to screen (interfaces
//      game render data to the driver's needs)
std::vector<char> AsciiDisplayCESystem::getRenderData()
{
    std::vector<char> out;
    int i = 0;
    for(auto it = grid_.begin(); it != grid_.end(); ++it, ++i) {
        if (i && !(i % width_)) {
            out.push_back('\n');
        }
        out.push_back(grid_[i]);
        out.push_back(' ');
    }
    return out;
}

bool AsciiDisplayCESystem::setGridChar(char c, int x, int y)
{
    if(x < 0 || x >= width_ || y < 0 || y >= height_) {
        return false;
    }
    grid_[y * width_ + x] = c;
    return true;
}

GE_HND(test_esc_handler, int, "on_key_esc", {
           reinterpret_cast<AsciiDisplayCESystem*>(that)->test_handler();
           return 0;
       })

GE_HND(retrieve_ascii_data_handler,
       std::shared_ptr<std::vector<char>>,
       "adces_retrieve_data_for_wndproc", {
           auto this_ = reinterpret_cast<AsciiDisplayCESystem*>(that);
           auto data = this_->getRenderData();
           for (auto c : data) {
               input->push_back(c);
           }
           return 0;
       })

// WAKE ME UP INSIDE
// RESCUE ME
//
// HOLD ME I'M SCARED
//
// This is one way to manually define a GameEventHandler:
template <class this_type>
class adces_on_esc : public GameEventHandler<int> {
public:
    adces_on_esc(this_type that) : GameEventHandler<int>("on_key_esc",
    []( void * parent, int input, void * that_) -> int {
        static_assert(std::is_pointer<this_type>::value,
                      "adces_on_esc() called with non-pointer");
        INFO_MSGOUT("Exiting now");
        reinterpret_cast<this_type>(that_)->test_handler();
        return 0;
    }, that) {}
};

void AsciiDisplayCESystem::onDispatcherAvailable()
{
//    adces_on_esc<decltype(this)> hnd(this);
//    test_esc_handler.setThat(this);
//    disp_->registerHandler<int>(test_esc_handler);
    //    disp_->registerHandler<int>(hnd);
    retrieve_ascii_data_handler.setThat(this);
    disp_->registerHandler<std::shared_ptr<std::vector<char>>>
            (retrieve_ascii_data_handler);
}

void AsciiDisplayCESystem::onAsciiCoreAvailable()
{

}

void AsciiDisplayCESystem::onECManagerAvailable()
{

}
