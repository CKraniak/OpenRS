#include "playermovementcesystem.h"

int PlayerMovementCESystem::onNumpad(char c, AsciiDisplayCESystem & s)
{
    switch(c) {
    case '1':
        if (s.setGridChar('P', pos_x - 1, pos_y + 1)) {
            s.setGridChar(DEFAULT_GROUND_ASCII_CHAR, pos_x, pos_y);
            pos_x--;
            pos_y++;
        }
        break;
    case '2':
        if (s.setGridChar('P', pos_x, pos_y + 1)) {
            s.setGridChar(DEFAULT_GROUND_ASCII_CHAR, pos_x, pos_y);
            pos_y++;
        }
        break;
    case '3':
        if (s.setGridChar('P', pos_x + 1, pos_y + 1)) {
            s.setGridChar(DEFAULT_GROUND_ASCII_CHAR, pos_x, pos_y);
            pos_x++;
            pos_y++;
        }
        break;
    case '4':
        if (s.setGridChar('P', pos_x - 1, pos_y)) {
            s.setGridChar(DEFAULT_GROUND_ASCII_CHAR, pos_x, pos_y);
            pos_x--;
        }
        break;
    case '6':
        if (s.setGridChar('P', pos_x + 1, pos_y)) {
            s.setGridChar(DEFAULT_GROUND_ASCII_CHAR, pos_x, pos_y);
            pos_x++;
        }
        break;
    case '7':
        if (s.setGridChar('P', pos_x - 1, pos_y - 1)) {
            s.setGridChar(DEFAULT_GROUND_ASCII_CHAR, pos_x, pos_y);
            pos_x--;
            pos_y--;
        }
        break;
    case '8':
        if (s.setGridChar('P', pos_x, pos_y - 1)) {
            s.setGridChar(DEFAULT_GROUND_ASCII_CHAR, pos_x, pos_y);
            pos_y--;
        }
        break;
    case '9':
        if (s.setGridChar('P', pos_x + 1, pos_y - 1)) {
            s.setGridChar(DEFAULT_GROUND_ASCII_CHAR, pos_x, pos_y);
            pos_x++;
            pos_y--;
        }
        break;
    }
    return 0;
}
