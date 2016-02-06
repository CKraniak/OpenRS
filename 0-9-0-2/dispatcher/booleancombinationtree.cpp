#include "booleancombinationtree.h"

#include <iostream>
#include <stdexcept>

bool BooleanCombinationNode::eval_and(std::vector<std::string> types_list)
{
    if (    left_ != NULL  &&
            right_ != NULL) {
        if(     left_->evaluate(types_list) == true &&
                right_->evaluate(types_list) == true)
        return true;
    }
    return false;
}

bool BooleanCombinationNode::eval_or(std::vector<std::string> types_list)
{
    if (left_ != NULL) {
        if(left_->evaluate(types_list) == true) {
            return true;
        }
    }
    if (right_ != NULL) {
        if(right_->evaluate(types_list) == true) {
            return true;
        }
    }
    return false;

}

bool BooleanCombinationNode::eval_not(std::vector<std::string> types_list)
{
    // By definition, the left node is occupied, and right is NULL.
    if(left_ != NULL)
        return !(left_->evaluate(types_list));
    return false;
}

bool BooleanCombinationNode::eval_type(std::vector<std::string> types_list)
{
    for(    std::vector<std::string>::iterator it = types_list.begin();
            it != types_list.end();
            it++) {
        if (value_.compare(*it) == 0) {
            // Matched with list element.
            return true;
        }
    }
    return false;
}

BooleanCombinationNode::BooleanCombinationNode(
        BooleanCombinationNode::BCNType type,
        std::string value,
        BooleanCombinationNode *left,
        BooleanCombinationNode *right)
{
    value_ = value;
    type_ = type;
    if(left != NULL) {
        left_ = new BooleanCombinationNode(*left);
    }
    else {
        left_ = NULL;
    }
    if(right != NULL) {
        right_ = new BooleanCombinationNode(*right);
    }
    else {
        right_ = NULL;
    }
}

bool BooleanCombinationNode::evaluate(std::vector<std::string> types_list)
{
    switch(type_) {
    case AND:
        return eval_and(types_list);

    case OR:
        return eval_or(types_list);

    case NOT:
        return eval_not(types_list);

    case TYPE:
        return eval_type(types_list);

    case EMPTY:
        return false;
    }
}

BooleanCombinationNode::~BooleanCombinationNode()
{
    if(left_ != NULL) {
        delete left_;
    }
    if(right_ != NULL) {
        delete right_;
    }
}

BooleanCombinationNode::BooleanCombinationNode(BooleanCombinationNode & that)
{
    type_ = that.getType();
    value_ = that.getValue();
    left_ = NULL;
    right_ = NULL;
    if(that.getLeft() != NULL) {
        left_ = new BooleanCombinationNode(*(that.getLeft()));
    }
    if(that.getRight() != NULL) {
        right_ = new BooleanCombinationNode(*(that.getRight()));
    }
}

BooleanCombinationNode
&BooleanCombinationNode::operator=(const BooleanCombinationNode & that)
{
    if(this == &that) {
        return *this;
    }
    type_ = that.getType();
    value_ = that.getValue();
    if(left_ != NULL) {
        delete left_;
        left_ = NULL;
    }
    if(right_ != NULL) {
        delete right_;
        right_ = NULL;
    }
    if(that.getLeft() != NULL) {
        left_ = new BooleanCombinationNode(*(that.getLeft()));
    }
    if(that.getRight() != NULL) {
        right_ = new BooleanCombinationNode(*(that.getRight()));
    }
}

BooleanCombinationTree::BooleanCombinationTree()
{

}

bool BooleanCombinationTree::evaluate(std::vector<std::string> types_to_check)
{
    return root.evaluate(types_to_check);
}
