#ifndef COMMAND_ALIASES_H
#define COMMAND_ALIASES_H

// Bring in the command definition.
#include "dispatcher.h"


//struct CommandDescriptor {
//    COMMANDTYPE command;
//    Group group;
//    SpecialDescriptor special_type;

//    void copy(CommandDescriptor& in);
//};


#ifdef ORS_ASCII_CONSOLE
extern struct CommandDescriptor CDESC_ANY;
extern struct CommandDescriptor CDESC_ANY_OVERRIDE;
extern struct CommandDescriptor CDESC_REJECT;
extern struct CommandDescriptor CDESC_NONE;
extern struct CommandDescriptor CDESC_KEYS_NUMPAD;
extern struct CommandDescriptor CDESC_DISPLAY_REDRAW;
extern struct CommandDescriptor CDESC_DISPLAY_MARKFORREDRAW;
extern struct CommandDescriptor CDESC_UPDATE_DISTRICT;
extern struct CommandDescriptor CDESC_ITEM_MOVED;

#endif // ORS_ASCII_CONSOLE

#endif // COMMAND_ALIASES_H
