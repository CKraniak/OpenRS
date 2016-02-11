/* Copyright (C) 2016, Chesley Kraniak
 *
 * This code is distributed under the terms of the GPLv3 license, which should
 * have been included with this file in the root directory as "LICENSE.txt". If
 * you do not have a copy of the license, see:
 *     http://www.gnu.org/licenses/gpl-3.0.txt
 * or:
 *     https://github.com/CKraniak/OpenRS
 * for the license.
 */

// How am I doing this?
//
// A component is a name:value pair that gets stored as a string usually.
// How will others use them?
//

#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <vector>

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
    bool is_compilable;

    // run() will call system's script_function() with args function_name and
    // argv. The CESystem is responsible for making the script function
    // available internally.
    int run(ScriptedCESystem& system);
    // compile() fills argv, system_type, and function_name variables from the
    // given script_text.
    int compile();

public:
    BarebonesScript(std::string script_text);
    bool isCompilable() { return is_compilable; }
};

class Component
{
    int id_;
    std::string str_data_; // String representation of data, which would go in
                           // a file after the "=".
    std::string name_; // What comes before the "=".
    BarebonesScript script_;

    int generateUniqueId();
public:
    Component() : script_("") {}
    Component(std::string name, std::string value) :
        name_(name), str_data_(value), script_(value) {}
    std::string getName() { return name_; }
    bool hasSameName(const Component & that);

    bool containsScript() { return script_.isCompilable(); }
    void setData(std::string data) {
        str_data_ = data;
        script_ = BarebonesScript(data);
    }
    std::string getData() { return str_data_; }
    int getDataAsInt() { return std::stoi(str_data_); }
    double getDataAsDouble() { return std::stod(str_data_); }
    BarebonesScript getDataAsScript() { return script_; }
};

#endif // COMPONENT_H
