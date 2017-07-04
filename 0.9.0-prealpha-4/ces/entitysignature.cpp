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

#include "entitysignature.h"

EntitySignature::EntitySignature(std::string component_name)
{
    list_[component_name] = 1;
}

EntitySignature::EntitySignature(std::vector<std::string> component_names)
{
    for(auto name : component_names) {
        if (list_.find(name) == list_.end()) {
            list_[name] = 1;
        }
        else {
            list_[name] += 1;
        }
    }
}

int EntitySignature::setComponentName(std::string name, int count)
{
    list_[name] = count;
    return count;
}

void EntitySignature::unsetComponentName(std::string name)
{
    list_.erase(name);
}

bool EntitySignature::matchesSignature(std::vector<Component> components,
                                       bool strict)
{
    std::map<std::string, int> track;
    for(auto c : components) {
        if (track.find(c.getName()) == track.end()) {
            track[c.getName()] = 1;
        }
        else {
            track[c.getName()] += 1;
        }
    }
    for (auto li : list_) {
        if (track.find(li.first) == track.end()) {
            return false;
        }
        if (track[li.first] < li.second ||
            (strict && (track[li.first] != li.second))) {
            return false;
        }
    }
    return true;
}
