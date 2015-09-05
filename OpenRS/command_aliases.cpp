#include "command_aliases.h"

#ifdef ORS_ASCII_CONSOLE
struct CommandDescriptor CDESC_ANY =
                {0, GR_NONE, SD_ANY, ST_NONE};
struct CommandDescriptor CDESC_ANY_OVERRIDE =
                {0, GR_NONE, SD_ANY_OVERRIDE, ST_NONE};
struct CommandDescriptor CDESC_REJECT =
                {0, GR_NONE, SD_REJECT, ST_NONE};
struct CommandDescriptor CDESC_NONE =
                {0, GR_NONE, SD_NONE, ST_NONE};
struct CommandDescriptor CDESC_KEYS_NUMPAD =
                {0, GR_KEYS_NUMPAD, SD_NONE, ST_NONE};
struct CommandDescriptor CDESC_DISPLAY_REDRAW =
                {0, GR_NONE, SD_DISPLAY_REDRAW, ST_NONE};
struct CommandDescriptor CDESC_DISPLAY_MARKFORREDRAW =
                {0, GR_NONE, SD_DISPLAY_MARKFORREDRAW, ST_NONE};
struct CommandDescriptor CDESC_UPDATE_DISTRICT =
                {0, GR_NONE, SD_UPDATE_DISTRICT, ST_NONE};
struct CommandDescriptor CDESC_ITEM_MOVED =
                {0, GR_NONE, SD_ITEM_MOVED, ST_NONE};
#endif // ORS_ASCII_CONSOLE
