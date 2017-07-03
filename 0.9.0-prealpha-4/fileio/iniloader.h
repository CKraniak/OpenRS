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

#ifndef INILOADER_H
#define INILOADER_H

#include <map>
#include <string>
#include <vector>

// This is probably not completely bugtested, but it seems to be working so far.

struct IniToken {
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

class IniLoader
{
public:
    using IniFileMap  = std::map<std::string, std::vector<std::string>>;
    const int NO_FILENAME_GIVEN = -1; // The filename hasn't been given
    const int CANT_OPEN = -2; // The filename hasn't been given
    const int MALFORMED_FILE = -3; // The file has bad syntax / something wonky

    IniLoader() : bytes_read_(NO_FILENAME_GIVEN), filename_("") {}
    IniLoader(std::string str) : filename_(str) {
        bytes_read_ = loadFromFile(str);
    }

    IniFileMap getStatements() {
        return statements_;
    }
    // Returns number of bytes read.
    int loadFromFile(std::string filename);
    int getBytesRead() { return bytes_read_; }
    //std::string getFilename() { return filename_; }
    static void test();

private:
    std::string filename_;
    std::vector<char> buffer_;
    std::vector<IniToken> token_vec_;
    IniFileMap statements_;
    int bytes_read_;

    using IniIoLine = std::pair<std::string, std::string>;
    using IniFileLineVec = std::vector<IniIoLine>;
    IniFileLineVec file_lines_;

    void mergeLinesIntoStatementMap();
    std::vector<char> stripComments(std::vector<char> &buffer);
    std::vector<IniToken> tokenize(std::vector<char> & buffer);
    // interpret() and consolidate() can be thought of as two parts of a parser.
    IniFileLineVec interpret(std::vector<IniToken> & token_vec);
    IniFileMap consolidate(IniFileLineVec lines);
};

#endif // INILOADER_H
