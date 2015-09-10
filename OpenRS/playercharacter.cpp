#include "playercharacter.h"

void PlayerCharacter::onNumpad(char c) {
    this->_parent->moveItemRelativeByNumpad(this, c);
}

static void onBump(void * _this, int argc, std::vector<std::string *> argv) {

}

PlayerCharacter::PlayerCharacter(int start_x,
                                 int start_y,
                                 ActorType type,
                                 Dispatcher * dispatcher,
                                 District * parent) :
    Actor(start_x, start_y, type, dispatcher, parent) {
    dispatcher->registerCommand(CALLBACK_FIX(this->onNumpad_PlayerCharacter),
                                &CDESC_KEYS_NUMPAD,
                                this);
    display_character = 'P';
}

void PlayerCharacter::update() {}
