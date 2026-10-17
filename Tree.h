#pragma once

enum class Color { red, black, none };
enum class Case { nodeIsNil, nodeIsRoot, normal };
enum class Side { left, right };

constexpr Side oppoSide(Side s) noexcept {
    return (s == Side::left) ? Side::right : Side::left;
}

class RbTree {
  private:
    class TreeNode {
      public:
        int value;
        Color color;
        TreeNode *left;
        TreeNode *right;
        TreeNode *parent;

        TreeNode(const int v);
        TreeNode(const int v, Color c, TreeNode *l, TreeNode *r, TreeNode *p);

        TreeNode *child(Side side);
    };

    struct insertContext {
        Side side;
        Side parentSide;
        TreeNode *parent;
        TreeNode *uncle;
        TreeNode *grandparent;

        insertContext(TreeNode *node);
    };
    struct removeContext {
        Side side;
        TreeNode *parent;
        TreeNode *bro;

        removeContext(TreeNode *node);
    };

    static TreeNode _nil;
    TreeNode *root;

    void replaceNode(TreeNode *OldOne, TreeNode *NewOne);
    void leftRotate(TreeNode *node);
    void rightRotate(TreeNode *node);
    void rotate(TreeNode *node, Side side);
    void clear();
    TreeNode *find(const int v);

    void insertFix(TreeNode *node);
    void removeFix(TreeNode *node);
    Case check(TreeNode *node);

  public:
    static TreeNode *nil;

    RbTree();
    ~RbTree();
    RbTree(const RbTree &other);

    void remove(const int v);
    void insert(const int v);
    void bfsPrint();
    void dfsPrint();
    bool contains(int v);
};

template <typename T> void mySwap(T &a, T &b) {
    T temp = a;
    a = b;
    b = temp;
}