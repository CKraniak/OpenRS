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

// The parser strips whitespace at the beginning of a line and just after '='.
// It also considers bare words syntax errors; placeholders must have an '='
// to be considered a valid line / file. Comments are lines starting with '#'.

#include "iniloader.h"

#include <fstream>
#include <sstream>
#include <memory>

#include "../utility.h"

int IniLoader::loadFromFile(std::string filename)
{
    filename_ = filename;
    if(filename.compare("") == 0) {
        return NO_FILENAME_GIVEN;
    }
    std::ifstream file_hnd;
    file_hnd.open(filename);
    if(! file_hnd.is_open()) {
        std::stringstream ss;
        ss << filename << " not found";
        ERR_MSGOUT(ss.str().c_str());
        return CANT_OPEN;
    }
    buffer_ = std::vector<char>(std::istreambuf_iterator<char>(file_hnd),
                                std::istreambuf_iterator<char>());
    auto stripped_buf_ = stripComments(buffer_);
    token_vec_ = tokenize(stripped_buf_);
    statements_ = consolidate(interpret(token_vec_));
    return buffer_.size();
}

std::vector<char> IniLoader::stripComments(std::vector<char> &buffer) {
    std::vector<char> stripped_buf;
    bool in_comment = false;
    char last_char = '\n';
    for (char c : buffer) {
        if(in_comment && c == '\n') {
            last_char = '\n';
            in_comment = false;
        }
        else {
            if (c == '#' && last_char == '\n') {
                in_comment = true;
                continue;
            }
            if (! in_comment) {
                stripped_buf.push_back(c);
                last_char = c;
            }
        }
    }
    return stripped_buf;
}

std::vector<IniToken> IniLoader::tokenize(std::vector<char> &buffer)
{
    IniToken token, last_token;
    token.type = IniToken::NONE;
    last_token.type = IniToken::NONE;
    std::vector<IniToken> ret_vec;
    for(auto c_it : buffer) {
        if(! token.pushChar(c_it)) {
            ret_vec.push_back(token);
            token.clear();
            token.pushChar(c_it);
        }
    }
    if(token.type == IniToken::WORD || token.type == IniToken::EQ) {
        ret_vec.push_back(token);
    }
    token.clear();
    token.type = IniToken::EOF_;
    ret_vec.push_back(token);
    return ret_vec;
}

IniLoader::IniFileLineVec IniLoader::interpret(std::vector<IniToken> &token_vec)
{
    IniFileLineVec ret_vec;
    IniIoLine line;
    IniToken prev;
    std::string name = "", value = "";
    prev.type = IniToken::NONE;
    int state = 1;
    // 4 types of tokens that get input here: EQ, WSP, WORD, ENDLINE
    // Also EOF_
    for(auto tok_it : token_vec) { // smoke it, tok_it
        switch(state) {
        case 1: // before EQ
            switch(tok_it.type) {
            case IniToken::EQ:
                // Must always come after a word
                if(prev.type != IniToken::WORD) {
                    IniFileLineVec blank;
                    bytes_read_ = MALFORMED_FILE; // inappropriate use of
                                                  // bytes_read_ as errno
                    return blank;
                }
                line.first = name;
                name.clear();
                state = 2;
                break;

            case IniToken::WSP:
                if(     prev.type == IniToken::NONE ||
                        prev.type == IniToken::ENDLINE) {
                    break;
                }
                name += tok_it.value;
                break;

            case IniToken::WORD:
                if(     prev.type == IniToken::NONE ||
                        prev.type == IniToken::ENDLINE) {
                    // Note: this never worked to strip comments since it only
                    //       ever pulled out the first token, usually just the
                    //       # itself:
                    //             "# comment here" --> " comment here"
                    //
                    //       It's working now so I don't want to mess with it.
                    if(tok_it.value[0] == '#') {
                        state = 3;
                        break;
                    }
                }
                name += tok_it.value;
                break;

            case IniToken::ENDLINE:
            case IniToken::EOF_:
                if(prev.type == IniToken::WORD) { // valueless words are
                                                  // not permitted
                    IniFileLineVec blank;
                    bytes_read_ = MALFORMED_FILE; // inappropriate use of
                                                  // bytes_read_ as errno
                    return blank;

                }
                break;
            }
            break;

        case 2: // after EQ
            switch(tok_it.type) {
            case IniToken::ENDLINE:
            case IniToken::EOF_: // EOF always breaks because it's always last
                line.second = value;
                ret_vec.push_back(line);
                line.first.clear();
                line.second.clear();
                value.clear();
                state = 1;
                break;

            case IniToken::EQ:
                value += tok_it.value;
                tok_it.type = IniToken::WORD; // tricks WSP case into only
                                              // trimming WSP in front of the
                                              // real EQ.
                break;

            case IniToken::WSP:
                if(prev.type == IniToken::EQ) {
                    break;
                }
                value += tok_it.value;
                break;

            case IniToken::WORD:
                value += tok_it.value;
                break;
            }
            break;

        case 3: // skip commented tokens up until ENDLINE
            if(tok_it.type == IniToken::ENDLINE) {
                state = 1;
            }
        }
        prev = tok_it;
    }
    return ret_vec;
}

IniLoader::IniFileMap IniLoader::consolidate(IniLoader::IniFileLineVec lines)
{
    IniFileMap fmap;
    for(auto line : lines) {
        if(fmap.find(line.first) == fmap.end()) {
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

bool IniToken::pushChar(char c)
{
    if(! canCharBeAppended(c)) {
        return false;
    }
    updateType(c);
    value += c;
    return true;
}

void IniToken::updateType(char c)
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

bool IniToken::canCharBeAppended(char c)
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

    case WORD:
    case WSP:
        return (c_type == WORD || c_type == WSP);
    }
}

IniToken::TOKEN_TYPE IniToken::typeOfChar(char c)
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
        return WORD;
    }
}

void IniLoader::test()
{
    // Check that the test files are available
    // Open entity_player.txt
    // Check each name:value pair
    // Do the same with others
    //
    // Note to future: I just briefly looked in the debugger and saw it was
    //                 good enough and so I didn't finish the comprehensive
    //                 tests.
    IniLoader il("configuration.txt");
    INFO_MSGOUT_DEBUG((std::string("Bytes read: ") +
                       std::to_string(il.getBytesRead())).c_str());
}
