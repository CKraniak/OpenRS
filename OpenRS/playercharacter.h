#ifndef PLAYERCHARACTER_H
#define PLAYERCHARACTER_H

class PlayerCharacter;

#include "item.h"
#include "district.h"
#include "dispatcher.h"

//#ifndef DISTRICT_H
//#endif // ! DISTRICT_H

class PlayerCharacter : public Item
{
private:
    void onNumpad(char c);
    static void onNumpad_PlayerCharacter(void * _this,
                                         int argc,
                                         std::vector<std::string *> argv) {
//        std::cout << "playerchar" << std::endl;
        if (argc < 1) {
            std::cout << "player onMoveItem noarg" << std::endl;
            return;
        }
        std::string in = *(argv[0]);
        ((PlayerCharacter *)_this)->onNumpad(in[0]);
#ifdef PRINTLINE
        std::cout << "player onMoveItem" << std::endl;
#endif
    }

public:
    PlayerCharacter(int, int, ItemType, Dispatcher *, District * parent);
};

#endif // PLAYERCHARACTER_H
