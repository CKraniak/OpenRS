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

#include <fstream>
#include <vector>
#include <string>
#include <map>

// How will an entity come in?
//
// The files describe entity types, and the form of a default entity.
//
// Several components with the same name can be defined in a file. It is
// guaranteed that they will appear in the correct order in the structure
// CesIo uses; that is, if there are two of the same label, the one appearing
// first in the file will have a lower index than the other.

struct CesIoToken {
    typedef enum {
        NONE = 0,
        WORD,  // tokenizer type
        NAME,  // parser type
        VALUE, // parser type
        WSP,
        EQ,
        EOF_,
        ENDLINE
    } TOKEN_TYPE;

    TOKEN_TYPE type;
    std::string value;
    // pushChar() will push the char to the string if it can.
    // It returns true if it both could and did push
    // It returns false it it could not and did not push
    bool pushChar(char c);
    void clear() {
        type = NONE;
        value = "";
    }

private:
    void updateType(char c);
    bool canCharBeAppended(char c);
    TOKEN_TYPE typeOfChar(char c);
};

class CesIo
{
public:
    using CesFileMap  = std::map<std::string, std::vector<std::string>>;
    const int NO_FILENAME_GIVEN = -1; // The filename hasn't been given
    const int CANT_OPEN = -2; // The filename hasn't been given
    const int MALFORMED_FILE = -3; // The file has bad syntax / something wonky

    CesIo() : bytes_read_(NO_FILENAME_GIVEN), filename_("") {}
    CesIo(std::string str) : filename_(str) {
        bytes_read_ = loadFromFile(str);
    }

    CesFileMap getStatements() {
        return statements_;
    }
    // Returns number of bytes read.
    int loadFromFile(std::string filename);
    int getBytesRead() { return bytes_read_; }
    std::string getFilename() { return filename_; }
    static void test();

private:
    std::string filename_;
    std::vector<char> buffer_;
    std::vector<CesIoToken> token_vec_;
    CesFileMap statements_;
    int bytes_read_;

    using CesIoLine = std::pair<std::string, std::string>;
    using CesFileLineVec = std::vector<CesIoLine>;
    CesFileLineVec file_lines_;

    void mergeLinesIntoStatementMap();
    std::vector<CesIoToken> tokenize(std::vector<char> & buffer);
    // interpret() and consolidate() can be thought of as two parts of a parser.
    CesFileLineVec interpret(std::vector<CesIoToken> & token_vec);
    CesFileMap consolidate(CesFileLineVec lines);
};

#endif // CESIO_H
