#include "playercharacter.h"

void PlayerCharacter::onNumpad(char c) {
    this->_parent->moveItemRelativeByNumpad(this, c);
}

PlayerCharacter::PlayerCharacter(int start_x,
                                 int start_y,
                                 ItemType type,
                                 Dispatcher * dispatcher,
                                 District * parent) :
    Item(start_x, start_y, type, dispatcher, parent) {
    dispatcher->registerCommand(CALLBACK_FIX(this->onNumpad_PlayerCharacter),
                                &CDESC_KEYS_NUMPAD,
                                this);
}
