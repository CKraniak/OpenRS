#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <vector>
#include <string>
#include <list>
#include <queue>

// YES
// That reading glext.h DID have a use!
// This is the form the callback pointer uses. _this was added to facilitate
// calling object methods from within the callback.
typedef void (*CALLBACK_PTR)
            (void * _this, int argc, std::vector<std::string *> & argv);

// Compiler interprets the assigned pointers as references and stops us from
// passing them into registerCommand. This macro unfucks the compiler so that
// commands can be registered. I haven't had a problem with this yet.
#define CALLBACK_FIX(x) ( reinterpret_cast<CALLBACK_PTR>(x) )

// These are mostly convenience macros, and do not necessarily have to be used,
// but they will probably make life easier (if they don't break int the future).
//
// x: class name
// y: function name (take no args)
// z: command descriptor
// a: relevant dispatcher object (NOT a pointer)
// b: relevant "this" object to use

// I need to know if I can token-string __VA_ARGS__
// The __VA_ARGS__ here is currently untested.
#define CALLBACK_FUNCTION_NAME(x, y) callback##x##y__autogen
#define CALLBACK_FUNCTION(x, y, z, ...)                              \
void CALLBACK_FUNCTION_NAME(x, y)(void * _this,                 \
                                  int argc,                     \
                                  std::vector<std::string *> argv) {   \
    ((x *)_this)->y(__VA_ARGS__);                                          \
}
#define REGISTER_CALLBACK(x, y, z, a, b) \
    a.registerCommand(CALLBACK_FIX(CALLBACK_FUNCTION_NAME(x, y)),     \
                    z,                                         \
                    b);

// This will vary based on the main input method / whatever event system
// is being used. Since it's stdin for now, char is appropriate.
#define COMMANDTYPE char

enum Group {
    GR_KEYS_NUMPAD = 1,
    GR_KEYS_ARROW,
    GR_NONE
};

// Dispatcher can use the SpecialDescriptor to get special behavior.
// If it's part of a command description, it specifies when the command will
// be run; if it's part of a command, it specifies which commands will run.
//
// Ex: SD_ANY in a descriptor means the associated funciton is called
//     on any command being posted, while SD_ANY in a command will cause
//     all commands to match, thus running all registered commands.
enum SpecialDescriptor {
    SD_ANY = 1,
    SD_ANY_OVERRIDE,
    SD_REJECT,
    SD_NONE, // I don't got no type!
    SD_DISPLAY_REDRAW,
    SD_DISPLAY_MARKFORREDRAW,
    SD_UPDATE_DISTRICT,
    SD_ITEM_MOVED
};

// If I use a state machine to track menus and such, the StateDescriptor
// can help the Dispatcher to know which commands to run.
enum StateDescriptor {
    ST_ANY = 1,
    ST_NONE,
    ST_GAME,
    ST_MENU
};

// Note that the input mask may change form in the future.
// In particular, it may specify both keyboard and mouse inputs,
// groups of buttons, and game states where it's called in if I
// make the game a FSM.
struct CommandDescriptor {
    COMMANDTYPE command;
    Group group;
    SpecialDescriptor special_type;
    StateDescriptor valid_state;

    void copy(CommandDescriptor& in);
};

#include "defines.h"
#include "command_aliases.h"

class Dispatcher
{
private:
    std::list<struct _dispatch_record *> dispatch_list;
    std::string cin_input;
    std::queue<CommandDescriptor *> cmd_list;
    //bool is_last_command_executed;
    std::vector<std::string *> callback_argv;
    bool is_executing;

    bool matchesGivenCommand(CommandDescriptor * descriptor);
    int getUniqueId();
    bool isIdTaken(int id);
    struct _dispatch_record * findById(int id);
    void callFunction(struct _dispatch_record & record,
                     std::vector<std::string *> & argv);
    void clearCallbackArgv();

public:
    Dispatcher();
    int registerCommand(CALLBACK_PTR func,
                        CommandDescriptor * descriptor,
                        void * _this = NULL);
    int unregisterCommand(int id);
    // I won't let the Dispatcher do actual parsing later on in Windows IO,
    // but I don't want to go to the trouble of making an IO command parser
    // class quite yet.
    // In theory, this class will only define a sort of "pushCommand" function
    // and the parsers will be the ones to figure out which command to send
    // to the Dispatcher class.
    void parseCinInput(std::string * input);
    void pushCommand(CommandDescriptor * command);
    void execute();
    CommandDescriptor & getMatchingDescriptor(CommandDescriptor & desc);
};

#endif // DISPATCHER_H
