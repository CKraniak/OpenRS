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

// The CesIo syntax is basically an INI style file.
//
// The parser strips whitespace at the beginning of a line and just after '='.
// It also considers bare words syntax errors; placeholders must have an '='
// to be considered a valid line / file. Comments are lines starting with '#'.

#include <fstream>
#include <sstream>
#include <iterator>

#include "cesio.h"
#include "../utility.h"
#include "directory.h"

int CesIo::loadFromFile(std::string filename)
{
    filename_ = filename;
    // Empty filename test
    if(filename.compare("") == 0) {
        return NO_FILENAME_GIVEN;
    }
    // Open the file.
    std::ifstream file_hnd;
    file_hnd.open(filename);
    if(! file_hnd.is_open()) {
        std::stringstream ss;
        ss << filename << " not found";
        ERR_MSGOUT(ss.str().c_str());
        return CANT_OPEN;
    }
    // Load the buffer.
    buffer_ = std::vector<char>(std::istreambuf_iterator<char>(file_hnd),
                                std::istreambuf_iterator<char>());
    token_vec_ = tokenize(buffer_);

}

std::vector<CesIoToken> CesIo::tokenize(std::vector<char> &buffer)
{
    CesIoToken token;
    std::vector<CesIoToken> ret_vec;
    for(auto c_it : buffer) {
        if(! token.pushChar(c_it)) {
            ret_vec.push_back(token);
            token.clear();
            token.pushChar(c_it);
        }
    }
    token.clear();
    token.type = CesIoToken::EOF_;
    ret_vec.push_back(token);
    return ret_vec;
}

CesIo::CesFileLineVec CesIo::interpret(std::vector<CesIoToken> &token_vec)
{
    CesFileLineVec ret_vec;
    CesIoLine line;
    CesIoToken prev;
    std::string name = "", value = "";
    prev.type = CesIoToken::NONE;
    int state = 1;
    // 4 types of tokens that get input here: EQ, WSP, WORD, ENDLINE
    // Also EOF_
    for(auto tok_it : token_vec) { // smoke it, tok_it
        switch(state) {
        case 1: // before EQ
            switch(tok_it.type) {
            case CesIoToken::EQ:
                // Must always come after a word
                if(prev.type != CesIoToken::WORD) {
                    CesFileLineVec blank;
                    bytes_read_ = MALFORMED_FILE; // inappropriate use of
                                                  // bytes_read_ as errno
                    return blank;
                }
                line.first = name;
                name.clear();
                state = 2;
                break;

            case CesIoToken::WSP:
                if(     prev.type == CesIoToken::NONE ||
                        prev.type == CesIoToken::ENDLINE) {
                    break;
                }
                name += tok_it.value;
                break;

            case CesIoToken::WORD:
                if(     prev.type == CesIoToken::NONE ||
                        prev.type == CesIoToken::ENDLINE) {
                    if(tok_it.value[0] == '#') {
                        state = 3;
                        break;
                    }
                }
                name += tok_it.value;
                break;

            case CesIoToken::ENDLINE:
            case CesIoToken::EOF_:
                if(prev.type == CesIoToken::WORD) { // valueless words are
                                                    // not permitted
                    CesFileLineVec blank;
                    bytes_read_ = MALFORMED_FILE; // inappropriate use of
                                                  // bytes_read_ as errno
                    return blank;

                }
                break;
            }
            break;

        case 2: // after EQ
            switch(tok_it.type) {
            case CesIoToken::ENDLINE:
            case CesIoToken::EOF_: // EOF always breaks because it's always last
                line.second = value;
                ret_vec.push_back(line);
                line.first.clear();
                line.second.clear();
                value.clear();
                state = 1;
                break;

            case CesIoToken::EQ:
                value += tok_it.value;
                tok_it.type = CesIoToken::WORD; // tricks WSP case into only
                                                // trimming WSP in front of the
                                                // real EQ.
                break;

            case CesIoToken::WSP:
                if(prev.type == CesIoToken::EQ) {
                    break;
                }
                value += tok_it.value;
                break;

            case CesIoToken::WORD:
                value += tok_it.value;
                break;
            }
            break;

        case 3: // skip commented tokens up until ENDLINE
            if(tok_it.type == CesIoToken::ENDLINE) {
                state = 1;
            }
        }
        prev = tok_it;
    }
    return ret_vec;
}

CesIo::CesFileMap CesIo::consolidate(CesIo::CesFileLineVec lines)
{
    CesFileMap fmap;
    for(auto line : lines) {
        if(fmap.find(line.first) != fmap.end()) {
            std::vector<std::string> svec;
            svec.push_back(line.second);
            fmap[line.first] = svec;
        }
        else {
            fmap[line.first].push_back(line.second);
        }
    }
    return fmap;
}

bool CesIoToken::pushChar(char c)
{
    if(! canCharBeAppended(c)) {
        return false;
    }
    updateType(c);
    value += c;
    return true;
}

void CesIoToken::updateType(char c)
{
    TOKEN_TYPE c_type = typeOfChar(c);
    switch(type) {
    // Only need to do things if a type can change
    // The only types that can morph are the NONE and WSP types
    case NONE:
    case WSP:
        type = c_type;
    }
}

bool CesIoToken::canCharBeAppended(char c)
{
    TOKEN_TYPE c_type = typeOfChar(c);
    switch(type) {
    case NONE:
        return true;

    case EQ:
        return false;

    case ENDLINE:
        // ENDLINEs can be appended to each other
        // Side effect is blank lines get skipped
        return (c_type == ENDLINE);

    // Due to <windows.h>, the CesIoToken namespace must be specified
    case CesIoToken::WORD:
    case WSP:
        return (c_type == CesIoToken::WORD || c_type == WSP);
    }
}

CesIoToken::TOKEN_TYPE CesIoToken::typeOfChar(char c)
{
    switch(c) {
    case '=':
        return EQ;

        // Might be platform dependent:
    case '\n':
    case '\r':
        return ENDLINE;

    case ' ':
    case '\t':
        return WSP;

    default:
        // Due to <windows.h>, the CesIoToken namespace must be specified
        return CesIoToken::WORD;
    }
}

void CesIo::test()
{
    // Check that the test files are available
    // Open entity_player.txt
    // Check each name:value pair
    // Do the same with others
    INFO_MSGOUT_DEBUG("CesIo Test Routine");
    std::unique_ptr<Directory> d = Directory::getDirectory();
    std::string cwd = d->getCwd();
    INFO_MSGOUT_DEBUG(cwd.c_str());
}
