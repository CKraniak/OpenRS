/* Copyright (C) 2015, Chesley Kraniak
 *
 * License indeterminate. It'll be at least GPL, maybe MIT. Definitely open
 * source, though.
 *
 *
 * A boolean combination tree represents a boolean combination of presence
 * conditions. The tree is sent a list of strings to evaluate for presence
 * or non-presence and returns the result of the computation. For example,
 * if the given rule is:
 *
 * "condition_1 AND NOT condition_2"
 *
 * and the tree is given the list of strings {"condition_5", "condition_1",
 * "condition_3"}, then it would evaluate to "true", since "condition_1" is
 * found while "condition_2" is not found.
 *
 * Empty conditions never match, and always evaluate to "false". Empty
 * conditions occur when the generator is given "" (the empty string, or an
 * equivalent) as the rule or when a syntatically incorrect rule is given
 * (e.g. "cond_1 AND AND NOT cond_2"). The isEmpty() function can be used to
 * tell if this has happened; in combination with checking that the generating
 * rule is not the empty string, it will tell you if there was a syntax error.
 *
 * The evaluation rules can be arbitrarily complicated. Realistically speaking,
 * you are limited by the amount of memory your target has.
 */

#ifndef BOOLEANCOMBINATIONTREE_H
#define BOOLEANCOMBINATIONTREE_H

#include <vector>
#include <string>

class BooleanCombinationNode
{
public:
    typedef enum {
        AND,
        OR,
        NOT,
        TYPE,
        EMPTY
    } BCNType;

    BooleanCombinationNode(BCNType type = EMPTY,
                           std::string value = "",
                           BooleanCombinationNode * left  = NULL,
                           BooleanCombinationNode * right = NULL);
    bool evaluate(std::vector<std::string> types_list);

    virtual ~BooleanCombinationNode();
    BooleanCombinationNode(BooleanCombinationNode&);
    BooleanCombinationNode& operator=(const BooleanCombinationNode&);
    bool isEmpty() { return type_ == EMPTY; }

private:
    BCNType type_;
    // Have to use pointers for this to make sense.
    BooleanCombinationNode * left_;
    BooleanCombinationNode * right_;
    std::string value_;

    bool eval_and (std::vector<std::string> types_list);
    bool eval_or  (std::vector<std::string> types_list);
    bool eval_not (std::vector<std::string> types_list);
    bool eval_type(std::vector<std::string> types_list);

    BCNType getType() const { return type_; }
    std::string getValue() const { return value_; }
    BooleanCombinationNode * getLeft()  const { return left_;  }
    BooleanCombinationNode * getRight() const { return right_; }
};


// Push the "present" vector to the boolean combiner
// Boolean combiner does something like:
    // IsPresent(ID_1) AND (IsPresent(ID_2) OR IsPresent(ID_3))
class BooleanCombinationTree
{
    BooleanCombinationNode root;

public:
    BooleanCombinationTree();
    BooleanCombinationTree(BooleanCombinationNode & root_node):
        root(root_node) {}
    bool evaluate(std::vector<std::string> types_to_check);
    bool isEmpty() { return root.isEmpty(); }
};

#endif // BOOLEANCOMBINATIONTREE_H
