#include "Tree.h"
#include <iostream>
#include <queue>
#include <stack>

RbTree::RbTree() : root(nil) {}
RbTree::~RbTree() { this->clear(); }
RbTree::RbTree(const RbTree &other) : root(nil) {
    std::queue<TreeNode *> queue;
    queue.push(other.root);
    while (!queue.empty()) {
        TreeNode *curr = queue.front();
        queue.pop();
        if (curr != nil) {
            this->insert(curr->value);
            queue.push(curr->left);
            queue.push(curr->right);
        }
    }
}

// oprations:

void RbTree::replaceNode(TreeNode *OldOne, TreeNode *NewOne) {
    TreeNode *parent = OldOne->parent;
    NewOne->parent = parent;
    if (parent == nil) {
        this->root = NewOne;
        return;
    }
    if (OldOne == parent->left) {
        parent->left = NewOne;
    } else {
        parent->right = NewOne;
    }
}

void RbTree::leftRotate(TreeNode *node) {
    if (node == nil) {
        return;
    }
    TreeNode *right = node->right;
    if (right == nil) {
        return;
    }
    replaceNode(node, right);
    node->right = right->left;
    if (right->left != nil) {
        right->left->parent = node;
    }
    right->left = node;
    node->parent = right;
}

void RbTree::rightRotate(TreeNode *node) {

    if (node == nil) {
        return;
    }
    TreeNode *left = node->left;
    if (left == nil) {
        return;
    }
    replaceNode(node, left);
    node->left = left->right;
    if (left->right != nil) {
        left->right->parent = node;
    }
    left->right = node;
    node->parent = left;
}

void RbTree::rotate(TreeNode *node, Side side) {
    if (side == Side::left) {
        leftRotate(node);
    } else {
        rightRotate(node);
    }
}

RbTree::TreeNode *RbTree::find(const int v) {
    TreeNode *curr = this->root;
    while (curr != nil && curr->value != v) {
        if (curr->value > v) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }

    return curr;
}

void RbTree::clear() {
    std::queue<TreeNode *> queue;
    queue.push(this->root);
    while (!queue.empty()) {
        TreeNode *curr = queue.front();
        queue.pop();
        if (curr != nil) {
            queue.push(curr->left);
            queue.push(curr->right);
            delete (curr);
        }
    }
}

void RbTree::insertFix(TreeNode *node) {
    switch (check(node)) {
    case (Case::nodeIsNil):
        return;
    case (Case::nodeIsRoot):
        node->color = Color::black;
        return;
    }

    insertContext ctx = insertContext(node);
    if (ctx.parent->color == Color::black) {
        return;
    }

    if (ctx.uncle->color == Color::red) {
        ctx.grandparent->color = Color::red;
        ctx.uncle->color = Color::black;
        ctx.parent->color = Color::black;
        return insertFix(ctx.grandparent);
    }

    if (ctx.side != ctx.parentSide) {
        rotate(ctx.parent, oppoSide(ctx.side));

        mySwap(node, ctx.parent);
    }
    rotate(ctx.grandparent, oppoSide(ctx.parentSide));

    ctx.parent->color = Color::black;
    ctx.grandparent->color = Color::red;
}

void RbTree::removeFix(TreeNode *node) {
    switch (check(node)) {
    case (Case::nodeIsNil):
        return;
    case (Case::nodeIsRoot):
        node->color = Color::black;
        return;
    }

    removeContext ctx = removeContext(node);
    if (ctx.bro->color == Color::red) {

        rotate(ctx.parent, ctx.side);
        ctx.parent->color = Color::red;
        ctx.bro->color = Color::black;
        ctx.bro = ctx.bro->child(ctx.side);
    }
    if (ctx.bro->left->color == Color::black &&
        ctx.bro->right->color == Color::black) {

        ctx.bro->color = Color::red;
        return removeFix(ctx.parent);
    }
    if (ctx.bro->child(ctx.side)->color == Color::red &&
        ctx.bro->child(oppoSide(ctx.side))->color == Color::black) {

        rotate(ctx.bro, oppoSide(ctx.side));
        ctx.bro->color = Color::red;
        ctx.bro->child(ctx.side)->color = Color::black;
        ctx.bro = ctx.bro->child(ctx.side);
    }

    rotate(ctx.parent, ctx.side);
    ctx.bro->color = ctx.parent->color;
    ctx.parent->color = Color::black;
    ctx.bro->child(oppoSide(ctx.side))->color = Color::black;
    return;
}

void RbTree::insert(const int v) {
    TreeNode *node = new TreeNode(v);
    if (this->root == nil) {
        this->root = node;
        node->color = Color::black;
        return;
    }

    TreeNode *curr = this->root;
    while (curr != nil) {
        if (curr->value == v) {

            return;
        }

        if (curr->value > v) {
            if (curr->left == nil) {
                curr->left = node;
                node->parent = curr;
                break;
            }
            curr = curr->left;
        } else {
            if (curr->right == nil) {
                curr->right = node;
                node->parent = curr;
                break;
            }
            curr = curr->right;
        }
    }

    insertFix(node);
}

void RbTree::remove(const int v) {
    TreeNode *node = find(v);
    if (node == nil) {
        return;
    }
    TreeNode *del = node;
    if (node->left != nil && node->right != nil) {

        del = node->right;
        while (del->left != nil) {
            del = del->left;
        }
        node->value = del->value;
    }

    if (del->left != nil) {
        replaceNode(del, del->left);
        del->left->color = Color::black;
        delete (del);
        return;
    }
    if (del->right != nil) {
        replaceNode(del, del->right);
        del->right->color = Color::black;
        delete (del);
        return;
    }
    if (del == this->root) {
        this->root = nil;
        delete (del);
        return;
    }

    replaceNode(del, nil);

    if (del->color == Color::red) {
        delete (del);
        return;
    }
    removeFix(del);
    delete (del);
}

void RbTree::bfsPrint() {

    std::queue<TreeNode *> queue;
    queue.push(this->root);
    while (!queue.empty()) {
        TreeNode *curr = queue.front();
        queue.pop();
        if (curr != nil) {
            queue.push(curr->left);
            queue.push(curr->right);
            std::cout << curr->value << ' ';
        }
    }
}

void RbTree::dfsPrint() {

    std::stack<TreeNode *> stack;
    stack.push(this->root);
    while (!stack.empty()) {
        TreeNode *curr = stack.top();
        stack.pop();
        if (curr != nil) {
            stack.push(curr->right);
            stack.push(curr->left);
            std::cout << curr->value << ' ';
        }
    }
}

bool RbTree::contains(int v) { return find(v) != nil; }