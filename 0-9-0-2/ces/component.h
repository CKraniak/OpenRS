#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <vector>
#include <memory>

class ScriptedCESystem;

class BarebonesScript {
    // Script text example:
    // "run_system(MOVEMENT, <func_name>, entity_id)"
    std::string script_text;
    std::vector<std::string> argv;
    std::string function_name;
    typedef enum {
        MOVEMENT,
        EXIT
    } SCRIPTABLE_SYSTEM;
    SCRIPTABLE_SYSTEM system_type;

    // run() will call system's script_function() with args function_name and
    // argv. The CESystem is responsible for making the script function
    // available internally.
    int run(ScriptedCESystem& system);
    // compile() fills argv, system_type, and function_name variables from the
    // given script_text.
    int compile();

public:
    BarebonesScript(std::string script_text);
};

class Component
{
    int id_;
    std::string str_data_; // String representation of data, which would go in
                           // a file after the "=".
    std::string name_;
    std::string default_value_; // Many fields will need a default value, or a
                                // default behavior.

public:
    Component();
    std::string getName() { return name_; }
    bool isSameType(const Component & that);
    bool containsScript();
};

#endif // COMPONENT_H
