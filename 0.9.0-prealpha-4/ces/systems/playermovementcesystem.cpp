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

#include "playermovementcesystem.h"


// Numpad differential translators, locations are relative to
// places on the numpad
//
// Takes the numbers 1-9, NOT the chars '1'-'9'
//
// dx returns -1 for left side, 1 for right side, 0 for middle
// dy returns -1 for top, 1 for bottom, and 0 for middle
int get_numpad_dx(char in) { return ((in + 2) % 3) - 1; }
int get_numpad_dy(char in) { return -(((in - 1) / 3) - 1); }


// Maybe a compiler issue
// Can't directly make a std::pair inside GE_HND
using intpair = std::pair<int, int>;

GE_HND(on_numpad_playermovement, char,          "on_keypress_numpad "
                                       "AND (NOT on_keypress_numpad_5) ", {
    auto sd = reinterpret_cast<StatefulDispatcher *>(parent);
    intpair dt;
    dt.first = get_numpad_dx(input);
    dt.second = get_numpad_dy(input);
    GameEvent<intpair> move_request("playermovement_request_move",
                                   {"on_player_request_move"},
                                   dt);
    int response = sd->emitEvent(move_request);
    if(response == 0) {
        GameEvent<char> collision_event("playermovement_on_collide",
                                        {"on_collide"},
                                        input);
        return sd->emitEvent(collision_event);
    }
    else {
        GameEvent<char> move_command("playermovement_command_move",
                                     {"on_command_move"},
                                     response);
        return sd->emitEvent(move_command);
    }
})

void PlayerMovementCESystem::onDispatcherAvailable() {
    //on_numpad_playermovement.setThat(this);
    disp_->registerHandler<char>(on_numpad_playermovement);
}

void PlayerMovementCESystem::onECManagerAvailable() {
    auto entity    = ec_data_sys_->makeEntity("player");
    auto component = ec_data_sys_->getComponent(entity, "ascii_display_char");
    if (component.isValid()) {
        auto data = component.getData();
        if (data.size() > 0) {
            player_asciichar_ = data[0];
        }
    }
}

int PlayerMovementCESystem::onNumpad(char c, AsciiDisplayCESystem & s)
{
    switch(c) {
    case '1':
        if (s.setGridChar(player_asciichar_, pos_x - 1, pos_y + 1)) {
            s.setGridChar(DEFAULT_GROUND_ASCII_CHAR, pos_x, pos_y);
            pos_x--;
            pos_y++;
        }
        break;
    case '2':
        if (s.setGridChar(player_asciichar_, pos_x, pos_y + 1)) {
            s.setGridChar(DEFAULT_GROUND_ASCII_CHAR, pos_x, pos_y);
            pos_y++;
        }
        break;
    case '3':
        if (s.setGridChar(player_asciichar_, pos_x + 1, pos_y + 1)) {
            s.setGridChar(DEFAULT_GROUND_ASCII_CHAR, pos_x, pos_y);
            pos_x++;
            pos_y++;
        }
        break;
    case '4':
        if (s.setGridChar(player_asciichar_, pos_x - 1, pos_y)) {
            s.setGridChar(DEFAULT_GROUND_ASCII_CHAR, pos_x, pos_y);
            pos_x--;
        }
        break;
    case '6':
        if (s.setGridChar(player_asciichar_, pos_x + 1, pos_y)) {
            s.setGridChar(DEFAULT_GROUND_ASCII_CHAR, pos_x, pos_y);
            pos_x++;
        }
        break;
    case '7':
        if (s.setGridChar(player_asciichar_, pos_x - 1, pos_y - 1)) {
            s.setGridChar(DEFAULT_GROUND_ASCII_CHAR, pos_x, pos_y);
            pos_x--;
            pos_y--;
        }
        break;
    case '8':
        if (s.setGridChar(player_asciichar_, pos_x, pos_y - 1)) {
            s.setGridChar(DEFAULT_GROUND_ASCII_CHAR, pos_x, pos_y);
            pos_y--;
        }
        break;
    case '9':
        if (s.setGridChar(player_asciichar_, pos_x + 1, pos_y - 1)) {
            s.setGridChar(DEFAULT_GROUND_ASCII_CHAR, pos_x, pos_y);
            pos_x++;
            pos_y--;
        }
        break;
    }
    return 0;
}
