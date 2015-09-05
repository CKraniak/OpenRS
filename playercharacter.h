#ifndef PLAYERCHARACTER_H
#define PLAYERCHARACTER_H

#include "item.h"
#include "dispatcher.h"

class PlayerCharacter : public Item
{
private:
    void onNumpad(char c);
    static void onNumpad_PlayerCharacter(void * _this,
                                         int argc,
                                         std::vector<std::string *> argv) {
        std::cout << "playerchar" << std::endl;
        if (argc < 1) {
            return;
        }
        std::string in = *(argv[0]);
        ((PlayerCharacter *)_this)->onNumpad(in[0]);
    }

public:
    PlayerCharacter(int, int, ItemType, Dispatcher *, District * parent);
};

#endif // PLAYERCHARACTER_H
