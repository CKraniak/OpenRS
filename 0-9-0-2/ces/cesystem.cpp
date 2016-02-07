#include "cesystem.h"

CESystem::CESystem()
{

}

std::vector<char> AsciiDisplayCESystem::getRenderData()
{
    std::vector<char> out;
    int i = 0;
    for(auto it = grid_.begin(); it != grid_.end(); ++it, ++i) {
        if (i && !(i % width_)) {
            out.push_back('\n');
        }
        out.push_back(grid_[i]);
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

int PlayerMovementCESystem::onNumpad(char c, AsciiDisplayCESystem & s)
{
    switch(c) {
    case '1':
        if (s.setGridChar('P', pos_x - 1, pos_y + 1)) {
            s.setGridChar('.', pos_x, pos_y);
            pos_x--;
            pos_y++;
        }
        break;
    case '2':
        if (s.setGridChar('P', pos_x, pos_y + 1)) {
            s.setGridChar('.', pos_x, pos_y);
            pos_y++;
        }
        break;
    case '3':
        if (s.setGridChar('P', pos_x + 1, pos_y + 1)) {
            s.setGridChar('.', pos_x, pos_y);
            pos_x++;
            pos_y++;
        }
        break;
    case '4':
        if (s.setGridChar('P', pos_x - 1, pos_y)) {
            s.setGridChar('.', pos_x, pos_y);
            pos_x--;
        }
        break;
    case '6':
        if (s.setGridChar('P', pos_x + 1, pos_y)) {
            s.setGridChar('.', pos_x, pos_y);
            pos_x++;
        }
        break;
    case '7':
        if (s.setGridChar('P', pos_x - 1, pos_y - 1)) {
            s.setGridChar('.', pos_x, pos_y);
            pos_x--;
            pos_y--;
        }
        break;
    case '8':
        if (s.setGridChar('P', pos_x, pos_y - 1)) {
            s.setGridChar('.', pos_x, pos_y);
            pos_y--;
        }
        break;
    case '9':
        if (s.setGridChar('P', pos_x + 1, pos_y - 1)) {
            s.setGridChar('.', pos_x, pos_y);
            pos_x++;
            pos_y--;
        }
        break;
    }
    return 0;
}
