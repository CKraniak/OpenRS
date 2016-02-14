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
