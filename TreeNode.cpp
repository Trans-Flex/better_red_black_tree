#include "Tree.h"

RbTree::TreeNode::TreeNode(const int v, Color c, TreeNode *l, TreeNode *r,
                           TreeNode *p)
    : value(v), color(c), left(l), right(r), parent(p) {}

RbTree::TreeNode RbTree::_nil(0, Color::black, &_nil, &_nil, &_nil);

RbTree::TreeNode *RbTree::nil = &RbTree::_nil;

RbTree::TreeNode::TreeNode(const int v)
    : value(v), color(Color::red), left(nil), right(nil), parent(nil) {}

RbTree::TreeNode *RbTree::TreeNode::child(Side side) {
    return (side == Side::left) ? this->left : this->right;
}

Case RbTree::check(TreeNode *node) {
    if (node == nil) {
        return Case::nodeIsNil;
    }
    if (node->parent == nil) {
        return Case::nodeIsRoot;
    }
    return Case::normal;
}
RbTree::insertContext::insertContext(TreeNode *node) {
    parent = node->parent;
    grandparent = parent->parent;
    if (node == parent->left) {
        side = Side::left;
    } else {
        side = Side::right;
    }
    if (parent == grandparent->left) {
        parentSide = Side::left;
        uncle = grandparent->right;
    } else {
        parentSide = Side::right;
        uncle = grandparent->left;
    }
}

RbTree::removeContext::removeContext(TreeNode *node) {
    parent = node->parent;
    if (node == parent->left) {
        side = Side::left;
        bro = parent->right;
    } else {
        side = Side::right;
        bro = parent->left;
    }
}