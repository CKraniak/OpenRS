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

#include "cesio.h"
#include "../utility.h"
#include "../ces/entityfactory.h"
#include "iniloader.h"
#include "directory.h"

// Needed for test()
#include "configuration.h"
#include  "../ces/ecmanager.h"

CesIo::CesIo(std::string directory) :
    directory_(directory) {}

// Return # of entities found
int CesIo::populateEntityFactory(EntityFactory & factory)
{
    std::unique_ptr<Directory> dir_reader = Directory::getDirectory();
    auto files = dir_reader->walk(directory_, DEFAULT_ENTITY_FILE_EXTENSION);
    for(auto filename : files) {
        auto et = makeEntityTypeFromFile(filename);
        auto typename_ = et.findAndSetTypenameInComponents();
        if (! typename_.compare("")) {
            continue;
        }
        factory.pushNewType(typename_, et);
    }
    return files.size();
}

EntityType CesIo::makeEntityTypeFromFile(std::string filename)
{
    IniLoader file_reader(filename);
    EntityType et;
    auto statements = file_reader.getStatements();
    for (auto statement : statements) {
        for (auto val : statement.second) {
            et.addComponent(statement.first, val);
        }
    }
    return et;
}

void CesIo::test()
{
    std::unique_ptr<Directory> d = Directory::getDirectory();
    Configuration conf;
    auto entity_dir = conf.getConfigurationValue("core_resource_directory");
    INFO_MSGOUT_DEBUG((std::string("CRD: ") + entity_dir).c_str());
    EntityFactory efac;
    auto cwd = d->getCwd() + "/";
    CesIo ioman(cwd + entity_dir);
    ioman.populateEntityFactory(efac);
    ECList ecl;
    ECManager ecman(ecl, efac);
    // Seems to work, now move over to testing ECManager directly.
    return;
}
