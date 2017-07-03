/* Copyright (C) 2017, Chesley Kraniak
 *
 * This code is distributed under the terms of the GPLv3 license, which should
 * have been included with this file in the root directory as "LICENSE.txt". If
 * you do not have a copy of the license, see:
 *     http://www.gnu.org/licenses/gpl-3.0.txt
 * or:
 *     https://github.com/CKraniak/OpenRS
 * for the license.
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <map>
#include <string>

const std::string DEFAULT_CONFIG_FILENAME = "configuration.txt";

// Should hold everything from a configuration.txt file

class Configuration
{
    std::map<std::string, std::string> config_map_;
    int bytes_read_; // Use to make sure there's a sensible configuration. 0
                     // bytes means probably that the filename is wrong.
    std::string filename_;

public:
    const int MALFORMED_CONFIG = -1;

    Configuration(std::string filename = DEFAULT_CONFIG_FILENAME) :
            filename_(filename) {
        bytes_read_ = loadConfiguration(filename_);
    }

    int bytesRead() { return bytes_read_; }
    int loadConfiguration(std::string filename);

    bool hasConfigurationValue(std::string configuration_name);

    std::string getConfigurationValue(std::string configuration_name);
    std::string get(std::string configuration_name) {
        return getConfigurationValue(configuration_name);
    }

    bool setConfiguration(std::string conf_name, std::string conf_val);

    static void test();
};

#endif // CONFIGURATION_H
