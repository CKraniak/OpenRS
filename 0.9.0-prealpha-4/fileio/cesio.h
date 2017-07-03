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
#ifndef CESIO_H
#define CESIO_H

#include <memory>
#include <string>

#include "directory.h"

const std::string DEFAULT_ENTITY_FILE_EXTENSION(".orsce");

// How will an entity come in?
//
// The files describe entity types, and the form of a default entity.
//
// Several components with the same name can be defined in a file. It is
// guaranteed that they will appear in the correct order in the structure
// CesIo uses; that is, if there are two of the same label, the one appearing
// first in the file will have a lower index than the other.

class EntityFactory;
class EntityType;

class CesIo
{
public:
    CesIo(std::string directory = "");

    void setDirectory(std::string dir) { directory_ = dir; }
    std::string getDirectory() { return directory_; }

    // Returns number of EntityTypes added.
    int populateEntityFactory(EntityFactory &factory);

    static void test();

private:
    std::string directory_;

    EntityType makeEntityTypeFromFile(std::string filename);
};

#endif // CESIO_H
