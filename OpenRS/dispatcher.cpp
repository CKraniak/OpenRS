/** Dispatches commands to registered functions.
  * Function prototype must be:
  * int function(int argc, std::vector<std::string> argv)
  */

#include <algorithm>
#include <iostream>

#include "dispatcher.h"

void CommandDescriptor::copy(CommandDescriptor &in) {
    command = in.command;
    group = in.group;
    special_type = in.special_type;
    valid_state = in.valid_state;
}

struct _dispatch_record {
    CALLBACK_PTR callback;
    int id;
    CommandDescriptor command_desc;
    void * _this;
};

// Return true means all commands will be run when execute() is called.
// Should return true if the passed-in descriptor describes a command that
// should match the last_command;
bool Dispatcher::matchesGivenCommand(CommandDescriptor *descriptor) {
    // First check if the state is passable
    // Then check the group
    // Then check the COMMANDTYPE
//    std::cout << "match function" << std::endl;
    // Only an ANY_OVERRIDE will run on a NONE command.
    if (last_command.special_type == CDESC_REJECT.special_type) {
        if (descriptor->special_type == CDESC_ANY_OVERRIDE.special_type) {
//            std::cout << "any_override" << std::endl;
            return true;
        }
//        std::cout << "reject" << std::endl;
        return false;
    }

    // The only case whem ANY doesn't run is for a NONE command.
    // Since that case is covered / returned above, go ahead and
    // run any ANY descriptors.
    if (descriptor->special_type == CDESC_ANY.special_type) {
//        std::cout << "any" << std::endl;
        return true;
    }

    // Start by direct matches of special types.
    if (last_command.special_type == descriptor->special_type) {
//        std::cout << "special eq" << std::endl;
        return true;
    }

    // Look at group types next.
    // If the state is not equivalent, they should not be run, but that behavior
    // won't be implemented until later.
    if (last_command.group == descriptor->group) {
//        std::cout << "group" << std::endl;
        return true;
    }
    // Automatically reject.
//    std::cout << (int) (descriptor->special_type) << std::endl;
//    std::cout << (int) (descriptor->group) << std::endl;
//    std::cout << (char) (descriptor->command) << std::endl;
    return false;
}

// Finds the first available id that isn't taken in the dispatch list.
int Dispatcher::getUniqueId() {
    int _id = 1;
    if (dispatch_list.size() == 0) {
        return _id;
    }
    else {
        for (; isIdTaken(_id); _id++) {}
    }
    return _id;
}

// Searches dispatch_list for occurence of id.
// Returns true if it finds one, false otherwise.
bool Dispatcher::isIdTaken(int id) {
    return findById(id) != NULL ? true: false;
}

// Iterates over the dispatch_list and returns the list item (a pointer type)
// that matches the given id. Returns NULL if nothing was found.
_dispatch_record *Dispatcher::findById(int id) {
    for (  std::list<_dispatch_record *>::iterator it = dispatch_list.begin();
           it != dispatch_list.end();
           it++) {
        if ((*it)->id == id) {
            return (*it);
        }
    }
    return NULL;
}

void Dispatcher::callFunction(_dispatch_record & record,
                              std::vector<std::string *> & argv) {
    CALLBACK_PTR function = record.callback;
    void * _this = record._this;
    function(_this, argv.size(), argv);
}

// Not sure if needed.
// The segfaults will reveal wisdom. Presumably the vector doesn't call delete
// on its elements when being cleared, but we'll find out ... THIS SUNDAY NIGHT
void Dispatcher::clearCallbackArgv() {
    for(    std::vector<std::string *>::iterator it = callback_argv.begin();
            it != callback_argv.end();
            it++) {
        std::string * ptr = *it;
        delete ptr;
    }
    callback_argv.clear();
}

Dispatcher::Dispatcher() {
}

// Registers the command with the dispatcher.
// Returns the unique id. DON'T LOSE IT! You need it to unregister.
// I may improve the efficiency of the callback process in the future by
// making more specific lists the callbacks go in so I don't end up with
// lists of millions of things registered that I have to search once per
// frame.
int Dispatcher::registerCommand(CALLBACK_PTR func,
                                CommandDescriptor * _command_desc,
                                void * _this) {
    // C++ structs can be initialized like classes.
    // This is useful as I won't have to call malloc/free.
    _dispatch_record * record = new _dispatch_record();
    record->callback = func;
    record->id = getUniqueId();
    record->command_desc.copy(*_command_desc);
    record->_this = _this;
    dispatch_list.push_back(record);
//    std::cout << "register command" << std::endl;
    return record->id;
}

// Called by whatever message loop / main loop there is, after parsing input
// with some parser subclass / parser translator class.
// The goal here is to divorce the internal message processing from the
// message receiving / parsing that is OS-dependent.
void Dispatcher::execute() {
    // Determine current command.
    std::vector<std::string *> argv = callback_argv;
    for (  std::list<_dispatch_record *>::iterator it = dispatch_list.begin();
           it != dispatch_list.end();
           it++) {
        // Execute all callbacks with matching commands.
//        std::cout << "execute" << std::endl;
        if (this->matchesGivenCommand( &((*it)->command_desc)) ) {
//            std::cout << "matched" << std::endl;
            this->callFunction(**it, argv);
//            std::cout << "was_run" << std::endl;
        }
    }
    this->clearCallbackArgv();
}

// Right now, only get the first letter of a string.
void Dispatcher::parseCinInput(std::string * input) {
    char cmd = (*input)[0];
    CommandDescriptor * cdesc = new CommandDescriptor;
    cdesc->copy(CDESC_NONE);
    cdesc->command = cmd;
    switch (cmd) {
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
        cdesc->group = GR_KEYS_NUMPAD;
        std::string * str = new std::string();
        *str = cmd;
        callback_argv.push_back(str);
//        std::cout << "Number pressed" << std::endl;
    }
    default: // do nothing for now.
        break;
    }
    this->pushCommand(cdesc);
    this->execute();
    delete cdesc;
}

// Must make a copy of all important things. Caller not guaranteed to
// leave pointer alone.
void Dispatcher::pushCommand(CommandDescriptor * command) {
    last_command.copy(*command);
}
