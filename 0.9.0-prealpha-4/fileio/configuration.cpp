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

#include "configuration.h"
#include "iniloader.h"
#include <cassert>

#include "../utility.h"

bool isConfigMalformed(IniLoader::IniFileMap statements) {
    for (auto statement : statements) {
        if (statement.second.size() > 1) {
            return true;
        }
    }
    return false;
}

int Configuration::loadConfiguration(std::string filename)
{
    IniLoader loader(filename);
    assert((bytes_read_ = loader.getBytesRead()) > 0); // The config is 100% required.
    auto config = loader.getStatements();
    if (isConfigMalformed(config)) {
        bytes_read_ = MALFORMED_CONFIG;
        return bytes_read_;
    }
    for (auto statement : config) {
        config_map_[statement.first] = statement.second[0];
    }
    return bytes_read_;
}

bool Configuration::hasConfigurationValue(std::string configuration_name)
{
    return config_map_.find(configuration_name) != config_map_.end();
}

std::string Configuration::getConfigurationValue(std::string configuration_name)
{
    if (hasConfigurationValue(configuration_name)) {
        return config_map_[configuration_name];
    }
    return "";
}

bool Configuration::setConfiguration(std::string conf_name,
                                     std::string conf_val)
{
    if (hasConfigurationValue(conf_name)) {
        return false;
    }
    config_map_[conf_name] = conf_val;
    return true;
}

void Configuration::test()
{
    Configuration c;
    auto disp = c.getConfigurationValue("display");
    INFO_MSGOUT_DEBUG((std::string("Config: display = ") + disp).c_str());
}
