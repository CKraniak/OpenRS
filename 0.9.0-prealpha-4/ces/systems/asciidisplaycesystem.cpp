#include "asciidisplaycesystem.h"

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
    adces_on_esc<decltype(this)> hnd(this);
    test_esc_handler.setThat(this);
    disp_->registerHandler<int>(test_esc_handler);
    disp_->registerHandler<int>(hnd);
}
