/* Copyright (C) 2015-2016, Chesley Kraniak
 *
 * This code is distributed under the terms of the GPLv3 license, which should
 * have been included with this file in the root directory as "LICENSE.txt". If
 * you do not have a copy of the license, see:
 *     http://www.gnu.org/licenses/gpl-3.0.txt
 * or:
 *     https://github.com/CKraniak/OpenRS
 * for the license.
 *
 * booleancombinationtreegenerator.h: Defines the event handler's "match-string
 *                                    compiler", the
 *                                    BooleanCombinationTreeGenerator class.
 *
 * Nothing in this file is supposed to be used by Dispatcher users. It should
 * only ever be found in the GameEventHandler<T> class.
 *
 * The boolean combination tree generator takes inputs such as:
 * "condition_1 AND (condition_2 OR (condition_3 AND NOT condition_1))"
 * and creates a boolean combination tree representing the string input.
 *
 * See the documentation on boolean combination trees for more information on
 * how they work.
 *
 * The tree is generated by parsing the string input and generating
 * a tree of BCTTokenParseNodes which represents the input. For example, the
 * above input would become:
 *
 *  , GOAL
 *  |  AND, EXPRESSION
 *  |  |  WORD, EXPRESSION
 *  |  |  |  condition_1, NAME
 *  |  |  PAREN, EXPRESSION
 *  |  |  |  OR, EXPRESSION
 *  |  |  |  |  WORD, EXPRESSION
 *  |  |  |  |  |  condition_2, NAME
 *  |  |  |  |  PAREN, EXPRESSION
 *  |  |  |  |  |  AND, EXPRESSION
 *  |  |  |  |  |  |  WORD, EXPRESSION
 *  |  |  |  |  |  |  |  condition_3, NAME
 *  |  |  |  |  |  |  NOT, EXPRESSION
 *  |  |  |  |  |  |  |  WORD, EXPRESSION
 *  |  |  |  |  |  |  |  |  condition_1, NAME
 *
 * (Note: the GOAL node has an empty "contents" field.)
 *
 * This is used to build a tree of BooleanCombinationNodes, which makes up the
 * tree.
 *
 * Currently, the system will print an error message to STDOUT if given wrong
 * input and return a tree that always evaluates to "false". The isEmpty()
 * function (of the BooleanCombinationTree) will return "true" in this case.
 */


#ifndef BOOLEANCOMBINATIONTREEGENERATOR_H
#define BOOLEANCOMBINATIONTREEGENERATOR_H

#include <string>
#include <vector>
#include <iostream>

#include "booleancombinationtree.h"

typedef int bctstate_t;

// BCTToken is the basic element of the token vector that gets passed to the
// parser for parse tree construction. The parse tree nodes also use this
// class to hold the contents and type of the parse tree node, as the code
// is already here.
class BCTToken
{
public:
    typedef enum {
        // Tokenizer types:
        WSP,
        OPEN,
        CLOSE,
        AND,
        OR,
        NOT,
        WORD,
        NONE,
        EOF_,
        // Parser types:
        EXPR,
        OP,
        GOAL
    } BCTTokenType;

    BCTToken(BCTTokenType type = NONE) : type_(type) {}
    bool canCharBeAppended(char c);
    int pushChar(char c);
    BCTTokenType getType() const { return type_; }
    std::string getContents() const { return contents_; }
    void clear();
    void setType(BCTTokenType type) { type_ = type; }
    void setContents(std::string contents) { contents_ = contents; }
    static std::string tokenTypeToStr(BCTTokenType);

private:
    BCTTokenType type_;
    std::string contents_;

    static BCTTokenType typeOfChar(char c);
};

// Parse tree is built from parse nodes.
// The parse stack is also built of these nodes, which is why it has a state.
class BCTTokenParseNode {
    BCTTokenParseNode * left_;
    BCTTokenParseNode * right_;
    BCTToken node_token;
    bctstate_t state_;

public:
    BCTTokenParseNode():
        left_(NULL), right_(NULL), node_token(), state_(0) {}
    // FUCK EVERYTHING ABOUT THIS POINTER-BASED CONSTRUCTOR
    // At least it stopped segfaulting.
    // Note to self: never make a constructor which takes a pointer to its class
    // as the first argument again; it makes the copy constructor and the
    // pointer constructor easy to confuse, miswrite, and bug out because the
    // compiler won't catch it.
    BCTTokenParseNode(BCTTokenParseNode * left,
                      BCTTokenParseNode * right = NULL,
                      BCTToken token = BCTToken());
    BCTTokenParseNode(BCTToken token):
        left_(NULL), right_(NULL), node_token(token) {}
    BCTTokenParseNode(const BCTTokenParseNode&);
    virtual ~BCTTokenParseNode();
    BCTTokenParseNode & operator=(const BCTTokenParseNode&);
    void setType(BCTToken::BCTTokenType type) { node_token.setType(type); }
    BCTToken::BCTTokenType getType() const { return node_token.getType(); }
    void setState(bctstate_t state) { state_ = state; }
    bctstate_t getState() const { return state_; }
    BCTTokenParseNode * getLeft() const { return left_; }
    BCTTokenParseNode * getRight() const { return right_; }
    std::string getContents() const { return node_token.getContents(); }
    void setContents(std::string contents) { node_token.setContents(contents); }
    void printTree(int level = 0);
};

// This is the actual tree generator class.
// The compiler core is implemented here.
class BooleanCombinationTreeGenerator
{
    std::vector<BCTToken> token_list;
    BCTTokenParseNode parse_tree_root;
    BooleanCombinationTree combination_tree;
    std::string input_;

private:
    std::vector<BCTToken> tokenize(std::string input);
    BCTTokenParseNode parse(std::vector<BCTToken> tokens);
    BooleanCombinationNode makeBCTNodeFromBCTTPNode(BCTTokenParseNode node);
    BooleanCombinationTree parseTableToBct(BCTTokenParseNode root);
    void syntaxErrorPrint(std::string str) {
        std::cout << str << std::endl;
    }
    void printTokenList();

public:
    BooleanCombinationTreeGenerator();
    BooleanCombinationTree compile(std::string input);
    static void test();
};

#endif // BOOLEANCOMBINATIONTREEGENERATOR_H