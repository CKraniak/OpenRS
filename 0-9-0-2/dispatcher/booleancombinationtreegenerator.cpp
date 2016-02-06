#include <stdexcept>
#include "booleancombinationtreegenerator.h"

#include <iostream>
#include <stack>
#include <memory>
#include <deque>

std::vector<BCTToken>
BooleanCombinationTreeGenerator::tokenize(std::string input)
{
    std::vector<BCTToken> ret_vec;
    BCTToken token; // current default copy constructor should work in push_back
    for(    std::string::iterator it = input.begin();
            it != input.end();
            it++) {
        // *it should be a char
        if(token.canCharBeAppended(*it)) {
            token.pushChar(*it);
        }
        else {
            // Strip whitespace from tokens here
            if (token.getType() != BCTToken::WSP)
                ret_vec.push_back(token);
            token.clear();
            token.pushChar(*it);
        }
    }
    if (token.getType() != BCTToken::NONE &&
        token.getType() != BCTToken::WSP) {
        ret_vec.push_back(token);
    }
    // Append EOF_ to token string.
    token.clear();
    token.setType(BCTToken::EOF_);
    ret_vec.push_back(token);
    return ret_vec;
}

BCTTokenParseNode
BooleanCombinationTreeGenerator::parse(std::vector<BCTToken> token_vec)
{
    std::deque<BCTTokenParseNode> tokens;
    for(    std::vector<BCTToken>::iterator it = token_vec.begin();
            it != token_vec.end();
            it++) {
        tokens.push_back(*it);
    }
    std::stack<BCTTokenParseNode> stack;
    BCTTokenParseNode lookahead, state_token, second_op;
    bctstate_t state = 1, next_state;
    std::string tmp;
    if((tokens.size() == 0) || (tokens.front().getType() == BCTToken::EOF_)) {
        // Empty token list, return a parse node with token type GOAL
        lookahead.setType(BCTToken::GOAL);
        return lookahead;
    }
    bool quit = false;
    while (!quit) {
        // LR state machine
        // I made the parser table by hand.
        switch(state) {
        case 1: // GOAL -> * EXPR EOF
            // Since this is the start state, and the EOF / token list empty
            // case was already handled above, it doesn't need to be handled
            // here. The other cases will need to handle it explicitly, though.
            lookahead = tokens.front();
            switch(lookahead.getType()) {
            case BCTToken::NOT:
                next_state = 2;
                break;

            case BCTToken::OPEN:
                next_state = 4;
                break;

            case BCTToken::WORD:
                next_state = 5;
                break;

            case BCTToken::EXPR:
                next_state = 3;
                break;

            default: // Syntax error
                syntaxErrorPrint("Syntax error: Expected !, ( or NAME; found "
                               + BCTToken::tokenTypeToStr(lookahead.getType()));
                quit = true;
                continue; // leave the while loop
            }
            state_token.setState(state);
            stack.push(state_token);
            stack.push(lookahead);
            state = next_state;
            tokens.pop_front();
            break;

        case 2: // EXPR -> NOT * EXPR
            // States 1, 2, 4, and 7 come here
            lookahead = tokens.front();
            switch(lookahead.getType()) {
            case BCTToken::NOT:
                next_state = 2;
                break;

            case BCTToken::OPEN:
                next_state = 4;
                break;

            case BCTToken::WORD:
                next_state = 5;
                break;

            case BCTToken::EXPR:
                next_state = 6;
                break;

            default: // Syntax error
                syntaxErrorPrint("Syntax error: Expected !, ( or NAME; found "
                               + BCTToken::tokenTypeToStr(lookahead.getType()));
                quit = true;
                continue;
            }
            state_token.setState(state);
            stack.push(state_token);
            stack.push(lookahead);
            state = next_state;
            tokens.pop_front();
            break;

        case 3: // GOAL -> EXPR * EOF
                // EXPR -> EXPR * OP EXPR
            lookahead = tokens.front();
            switch(lookahead.getType()) {
            case BCTToken::AND:
                next_state = 11;
                break;

            case BCTToken::OR:
                next_state = 12;
                break;

            case BCTToken::OP:
                next_state = 7;
                break;

            case BCTToken::EOF_: // End of input
                                 // Successful compilation
                quit = true;
                // Should be an EXPR on stack top
                lookahead = BCTTokenParseNode(&stack.top(),
                                              NULL,
                                              BCTToken::GOAL); // GOOOOOOOAAAL!!
                continue;

            default: // Syntax error
                syntaxErrorPrint(
                               "Syntax error: Expected &, |, OP, or EOF; found "
                               + BCTToken::tokenTypeToStr(lookahead.getType()));
                quit = true;
                continue;
            }
            state_token.setState(state);
            stack.push(state_token);
            stack.push(lookahead);
            state = next_state;
            tokens.pop_front();
            break;

        case 4: // EXPR -> OPEN * EXPR CLOSE
            lookahead = tokens.front();
            switch(lookahead.getType()) {
            case BCTToken::NOT:
                next_state = 2;
                break;

            case BCTToken::OPEN:
                next_state = 4;
                break;

            case BCTToken::WORD:
                next_state = 5;
                break;

            case BCTToken::EXPR:
                next_state = 8;
                break;

            default: // Syntax error
                syntaxErrorPrint("Syntax error: Expected !, ( or NAME; found "
                               + BCTToken::tokenTypeToStr(lookahead.getType()));
                quit = true;
                continue;
            }
            state_token.setState(state);
            stack.push(state_token);
            stack.push(lookahead);
            state = next_state;
            tokens.pop_front();
            break;

        case 5: // EXPR -> WORD *
                // Stack: 1 a 2 b X WORD
            lookahead = tokens.front();
            switch(lookahead.getType()) {
            // All fall through
            case BCTToken::AND:
            case BCTToken::OR:
            case BCTToken::CLOSE:
            case BCTToken::EOF_:
            case BCTToken::OP:
                // lookahead = stack.top();
                // Stack top goes in left of new EXPR node
                // Note that the node will delete the pointer on destruction,
                // so it doesn't need to be managed here.
                //tmp_ptr = new BCTTokenParseNode(stack.top());
                lookahead = BCTTokenParseNode(&stack.top(),
                                              NULL,
                                              BCTToken::EXPR);
                lookahead.setContents("WORD");
                stack.pop();
                next_state = stack.top().getState();
                stack.pop();
                tokens.push_front(lookahead); // < segfaulting here
                                              // Issue with copy constructor
                state = next_state;
                break;

            default: // Syntax error
                syntaxErrorPrint("Syntax error: "
                               + BCTToken::tokenTypeToStr(lookahead.getType())
                               + std::string(" not legal after ")
                               + BCTToken::tokenTypeToStr(BCTToken::WORD)
                               + std::string(": ")
                               + stack.top().getContents());
                quit = true;
                continue;
            }
            break;

        case 6: // EXPR -> NOT EXPR *
            // Need to account for the NOT here
            lookahead = tokens.front();
            switch(lookahead.getType()) {
            // All fall through
            case BCTToken::AND:
            case BCTToken::OR:
            case BCTToken::CLOSE:
            case BCTToken::EOF_:
            case BCTToken::OP:
                // Stack top goes in left of new EXPR node
                // Note that the node will delete the pointer on destruction,
                // so it doesn't need to be managed here.
                lookahead = BCTTokenParseNode(&stack.top(),
                                              NULL,
                                              BCTToken::EXPR);
                lookahead.setContents("NOT");
                stack.pop();
                stack.pop();
                stack.pop();
                next_state = stack.top().getState();
                stack.pop();
                tokens.push_front(lookahead);
                state = next_state;
                break;

            default: // Syntax error
                syntaxErrorPrint("Syntax error: "
                               + BCTToken::tokenTypeToStr(lookahead.getType())
                               + std::string(" not legal after ")
                               + BCTToken::tokenTypeToStr(BCTToken::EXPR));
                quit = true;
                continue;
            }
            break;

        case 7: // EXPR -> EXPR OP * EXPR
            lookahead = tokens.front();
            switch(lookahead.getType()) {
            case BCTToken::NOT:
                next_state = 2;
                break;

            case BCTToken::OPEN:
                next_state = 4;
                break;

            case BCTToken::WORD:
                next_state = 5;
                break;

            case BCTToken::EXPR:
                next_state = 10;
                break;

            default: // Syntax error
                syntaxErrorPrint("Syntax error: Expected !, ( or NAME; found "
                               + BCTToken::tokenTypeToStr(lookahead.getType()));
                quit = true;
                continue;
            }
            state_token.setState(state);
            stack.push(state_token);
            stack.push(lookahead);
            state = next_state;
            tokens.pop_front();
            break;

        case 8: // EXPR -> OPEN EXPR * CLOSE
            lookahead = tokens.front();
            switch(lookahead.getType()) {
            case BCTToken::AND:
                next_state = 11;
                break;

            case BCTToken::OR:
                next_state = 12;
                break;

            case BCTToken::OP:
                next_state = 7;
                break;

            case BCTToken::CLOSE:
                next_state = 9;
                break;

            default: // Syntax error
                syntaxErrorPrint(
                               "Syntax error: Expected &, |, OP, or ); found "
                               + BCTToken::tokenTypeToStr(lookahead.getType()));
                quit = true;
                continue;
            }
            state_token.setState(state);
            stack.push(state_token);
            stack.push(lookahead);
            state = next_state;
            tokens.pop_front();
            break;

        case 9: // EXPR -> OPEN EXPR CLOSE *
            lookahead = tokens.front();
            switch(lookahead.getType()) {
            // All fall through
            case BCTToken::AND:
            case BCTToken::OR:
            case BCTToken::CLOSE:
            case BCTToken::EOF_:
            case BCTToken::OP:
                stack.pop();
                stack.pop();
                lookahead = BCTTokenParseNode(&stack.top(),
                                              NULL,
                                              BCTToken::EXPR);
                lookahead.setContents("PAREN");
                stack.pop();
                stack.pop();
                stack.pop();
                next_state = stack.top().getState();
                stack.pop();
                tokens.push_front(lookahead);
                state = next_state;
                break;

            default: // Syntax error
                syntaxErrorPrint("Syntax error: "
                               + BCTToken::tokenTypeToStr(lookahead.getType())
                               + std::string(" not legal after ")
                               + BCTToken::tokenTypeToStr(BCTToken::CLOSE));
                quit = true;
                continue;
            }
            break;

        case 10: // EXPR -> EXPR OP EXPR *
            lookahead = tokens.front();
            switch(lookahead.getType()) {
            // All fall through
            case BCTToken::AND:
            case BCTToken::OR:
            case BCTToken::CLOSE:
            case BCTToken::EOF_:
            case BCTToken::OP:
                second_op = stack.top();
                stack.pop();
                stack.pop();
                tmp = stack.top().getContents();
                stack.pop();
                stack.pop();
                lookahead = stack.top();
                lookahead = BCTTokenParseNode(&stack.top(),
                                              &second_op,
                                              BCTToken::EXPR);
                lookahead.setContents(tmp);
                stack.pop();
                next_state = stack.top().getState();
                stack.pop();
                tokens.push_front(lookahead);
                state = next_state;
                break;

            default: // Syntax error
                syntaxErrorPrint("Syntax error: "
                               + BCTToken::tokenTypeToStr(lookahead.getType())
                               + std::string(" not legal after ")
                               + BCTToken::tokenTypeToStr(BCTToken::EXPR));
                quit = true;
                continue;
            }
            break;

        case 11: // OP -> AND *
            lookahead = tokens.front();
            switch(lookahead.getType()) {
            // All fall through
            case BCTToken::NOT:
            case BCTToken::OPEN:
            case BCTToken::WORD:
            case BCTToken::EXPR:
                lookahead = BCTTokenParseNode(&stack.top(),
                                              NULL,
                                              BCTToken::OP);
                lookahead.setContents("AND");
                stack.pop();
                next_state = stack.top().getState();
                stack.pop();
                tokens.push_front(lookahead);
                state = next_state;
                break;

            default: // Syntax error
                syntaxErrorPrint("Syntax error: "
                               + BCTToken::tokenTypeToStr(lookahead.getType())
                               + std::string(" not legal after ")
                               + BCTToken::tokenTypeToStr(BCTToken::AND));
                quit = true;
                continue;
            }
            break;

        case 12: // OP -> OR *
            lookahead = tokens.front();
            switch(lookahead.getType()) {
            // All fall through
            case BCTToken::NOT:
            case BCTToken::OPEN:
            case BCTToken::WORD:
            case BCTToken::EXPR:
                lookahead = stack.top();
                lookahead = BCTTokenParseNode(&stack.top(),
                                              NULL,
                                              BCTToken::OP);
                lookahead.setContents("OR");
                stack.pop();
                next_state = stack.top().getState();
                stack.pop();
                tokens.push_front(lookahead);
                state = next_state;
                break;

            default: // Syntax error
                syntaxErrorPrint("Syntax error: "
                               + BCTToken::tokenTypeToStr(lookahead.getType())
                               + std::string(" not legal after ")
                               + BCTToken::tokenTypeToStr(BCTToken::OR));
                quit = true;
                continue;
            }
            break;
        }
    }
    if (lookahead.getType() != BCTToken::GOAL) {
        lookahead.setType(BCTToken::NONE);
    }
    return lookahead;
}

BooleanCombinationNode
BooleanCombinationTreeGenerator::makeBCTNodeFromBCTTPNode(
        BCTTokenParseNode node)
{
    BooleanCombinationNode ret_node, left, right;
    // PAREN nodes are pass-through as they have no boolean op they perform.
    if(node.getContents().compare("PAREN") == 0) {
        ret_node = makeBCTNodeFromBCTTPNode(*(node.getLeft()));
        return ret_node;
    }
    if(node.getType() == BCTToken::GOAL) {
        if(node.getLeft() != NULL)
            ret_node = makeBCTNodeFromBCTTPNode(*(node.getLeft()));
        else
            ret_node = BooleanCombinationNode(BooleanCombinationNode::EMPTY);
        return ret_node;
    }
    if(node.getContents().compare("AND") == 0) {
        left = makeBCTNodeFromBCTTPNode(*node.getLeft());
        right = makeBCTNodeFromBCTTPNode(*node.getRight());
        ret_node = BooleanCombinationNode(BooleanCombinationNode::AND,
                                          node.getContents(),
                                          &left,
                                          &right);
        return ret_node;
    }
    if(node.getContents().compare("OR") == 0) {
        left = makeBCTNodeFromBCTTPNode(*node.getLeft());
        right = makeBCTNodeFromBCTTPNode(*node.getRight());
        ret_node = BooleanCombinationNode(BooleanCombinationNode::OR,
                                          node.getContents(),
                                          &left,
                                          &right);
        return ret_node;
    }
    if(node.getContents().compare("NOT") == 0) {
        left = makeBCTNodeFromBCTTPNode(*node.getLeft());
        ret_node = BooleanCombinationNode(BooleanCombinationNode::NOT,
                                          node.getContents(),
                                          &left,
                                          NULL);
        return ret_node;
    }
    if(node.getContents().compare("WORD") == 0) {
        ret_node = BooleanCombinationNode(BooleanCombinationNode::TYPE,
                                          node.getLeft()->getContents(),
                                          NULL,
                                          NULL);
        return ret_node;
    }
    return ret_node;
}

BooleanCombinationTree
BooleanCombinationTreeGenerator::parseTableToBct(BCTTokenParseNode root)
{
    BooleanCombinationNode tmp;
    tmp = makeBCTNodeFromBCTTPNode(root);
    BooleanCombinationTree retval(tmp);
    return retval;
}

void BooleanCombinationTreeGenerator::printTokenList()
{
    for(    std::vector<BCTToken>::iterator it = token_list.begin();
            it != token_list.end();
            it++) {
        std::cout << (*it).getContents() << ", "
                  << BCTToken::tokenTypeToStr((*it).getType()) << std::endl;
    }
}

// RULES:
// keywords are: AND OR NOT
// special characters are: ( )
// separators are whitespace of any kind
// any non-whitespace mashup that isn't a keyword, special, or improper spacing
// near the parens should be alphanumeric or underscore/hyphen.

BooleanCombinationTreeGenerator::BooleanCombinationTreeGenerator()
{

}

BooleanCombinationTree
BooleanCombinationTreeGenerator::compile(std::string input)
{
    token_list = tokenize(input);
    parse_tree_root = parse(token_list);
    // Remember to abort here if the root node is not of type GOAL
    if(parse_tree_root.getType() != BCTToken::GOAL) {
        // Empty node as root --> invalid tree
        BooleanCombinationNode empty_node;
        BooleanCombinationTree ret_tree(empty_node);
        return ret_tree;
    }
//    std::cout << "\n----------\n\nMAKING BCT: " << std::endl;
    combination_tree = parseTableToBct(parse_tree_root);
//    std::cout << "DONE" << std::endl;
    return combination_tree;
}

bool BCTToken::canCharBeAppended(char c)
{

    switch(type_) {
    case NONE:
        return true;

    case WORD:
    case AND:
    case OR:
    case NOT:
        if (typeOfChar(c) == WORD)
            return true;
        else
            return false;

    case WSP:
        if (typeOfChar(c) == WSP)
            return true;
        else
            return false;

    case OPEN:
        return false;

    case CLOSE:
        return false;
    }
}

// Returns 0 if it can't be appended, 1 if it was appended
// If 0, it isn't appended, if 1, it is
int BCTToken::pushChar(char c)
{
    if (!this->canCharBeAppended(c)) {
        return 0;
    }
    switch(type_) {
    case NONE:
        type_ = typeOfChar(c);
        // fall through intended
    case WSP:
    case WORD:
    case AND:
    case OR:
    case NOT:
        contents_ += c;
        break;

    case OPEN:
    case CLOSE:
        // wuut
        // canCharBeAppended() should never let it get here
        throw std::range_error("BCTToken::pushChar: invalid type to append to");
        break;
    }
    // compare < 0 if input is longer than contained value
    if(type_ == WORD) {
        if(contents_.compare("AND") == 0)
            type_ = AND;
        if(contents_.compare("OR") == 0)
            type_ = OR;
        if(contents_.compare("NOT") == 0)
            type_ = NOT;
    }
    return 1;
}

void BCTToken::clear()
{
    type_ = NONE;
    contents_.clear();
}

std::string BCTToken::tokenTypeToStr(BCTToken::BCTTokenType token)
{
    switch(token) {
    case WSP:
        return "whitespace";

    case OPEN:
        return "(";

    case CLOSE:
        return ")";

    case AND:
        return "&";

    case OR:
        return "|";

    case NOT:
        return "!";

    case WORD:
        return "NAME";

    case NONE:
        return "NONE";

    case EOF_:
        return "EOF";

    case EXPR:
        return "EXPRESSION";

    case OP:
        return "OPERATOR";

    case GOAL:
        return "GOAL";
    }
}

BCTToken::BCTTokenType BCTToken::typeOfChar(char c)
{
    switch(c) {
    case '(':
        return OPEN;

    case ')':
        return CLOSE;

    case '\n':
    case '\t':
    case ' ':
    case '\v':
    case '\f':
    case '\r':
        return WSP;

    default:
        return WORD;
    }
}

// Here be dragons
// Note: FUCK this class, and this function in particular. The problem with all
// the stupid NONE nodes in everything was with operator= calling this instead
// of the copy constructor.
BCTTokenParseNode::BCTTokenParseNode(BCTTokenParseNode *left,
                                     BCTTokenParseNode *right,
                                     BCTToken token)
{
    node_token = BCTToken(token);
    if(left != NULL) {
        left_ = new BCTTokenParseNode((*left));
    }
    else {
        left_ = NULL;
    }
    if(right != NULL) {
        right_ = new BCTTokenParseNode((*right));
    }
    else {
        right_ = NULL;
    }
}

BCTTokenParseNode::BCTTokenParseNode(const BCTTokenParseNode &copy_from)
{
    if (copy_from.getLeft() != NULL) {
        left_ = new BCTTokenParseNode(*(copy_from.getLeft()));
    }
    else {
        left_ = NULL;
    }
    if (copy_from.getRight() != NULL) {
        right_ = new BCTTokenParseNode(*(copy_from.getRight()));
    }
    else {
        right_ = NULL;
    }
    node_token = BCTToken();
    node_token.setType(copy_from.getType());
    node_token.setContents(copy_from.getContents());
    state_ = copy_from.getState();
}

// virtual
BCTTokenParseNode::~BCTTokenParseNode()
{
    if (left_ != NULL) {
        delete left_;
        left_ = NULL;
    }
    if (right_ != NULL) {
        delete right_;
        right_ = NULL;
    }
}

BCTTokenParseNode &BCTTokenParseNode::operator=(const BCTTokenParseNode &that)
{
    if (this == &that) {
        return *this;
    }
    if(left_ != NULL) {
        delete left_;
        left_ = NULL;
    }
    if(right_ != NULL) {
        delete right_;
        right_ = NULL;
    }
    if(that.getLeft() != NULL) {
        left_ = new BCTTokenParseNode(*(that.getLeft()));
    }
    if(that.getRight() != NULL) {
        right_ = new BCTTokenParseNode(*(that.getRight()));
    }
    node_token.setType(that.getType());
    node_token.setContents(that.getContents());
    state_ = that.getState();
    return *this;
}

void BCTTokenParseNode::printTree(int level)
{
    for(int i = 0; i < level; i++) {
        std::cout << "|  ";
    }
    std::cout << node_token.getContents() << ", "
              << BCTToken::tokenTypeToStr(node_token.getType()) << std::endl;
    if (left_ != NULL) {
        left_->printTree(level + 1);
    }
    if (right_ != NULL) {
        right_->printTree(level + 1);
    }
}

void BooleanCombinationTreeGenerator::test() {
    BooleanCombinationTreeGenerator gen;
    BooleanCombinationTree tree;
    std::vector<std::string> vs;

    tree = gen.compile("TEST AND (TEST OR NOT other_try)");
    std::cout << tree.evaluate(vs) << std::endl; // false

    tree = gen.compile("");
    std::cout << tree.evaluate(vs) << std::endl; // false

    tree = gen.compile("under AND one AND two OR NOT (plus OR NOT three AND four)");
    std::cout << tree.evaluate(vs) << std::endl; // true

    tree = gen.compile("(under AND one AND two) OR NOT (plus OR NOT three AND four)");
    std::cout << tree.evaluate(vs) << std::endl; // true

    tree = gen.compile("yes");
    std::cout << tree.evaluate(vs) << std::endl; // false

    tree = gen.compile("NOT yes");
    std::cout << tree.evaluate(vs) << std::endl; // true

    vs.push_back("one");
    vs.push_back("three");
    tree = gen.compile("one AND (two OR three)");
    std::cout << tree.evaluate(vs) << std::endl; // true

    vs.pop_back();
    tree = gen.compile("one AND (two OR three)");
    std::cout << tree.evaluate(vs) << std::endl; // false

    vs.pop_back();
    tree = gen.compile("one OR (two OR NOT three)");
    std::cout << tree.evaluate(vs) << std::endl; // true

    tree = gen.compile(" condition_1 AND (condition_2 OR "
                       "(condition_3 AND NOT condition_1))");
    std::cout << tree.evaluate(vs) << std::endl; // false
}
