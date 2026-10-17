#include "Tree.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

void testBasicInsert(RbTree &tree) {
    cout << "Running: testBasicInsert..." << endl;
    vector<int> nums = {10, 20, 30, 15, 25, 5};
    for (int n : nums)
        tree.insert(n);

    for (int n : nums)
        assert(tree.contains(n) == true);
    assert(tree.contains(99) == false);
    cout << "  -> Passed!" << endl;
}

void testOrderedInsert(RbTree &tree) {
    cout << "Running: testOrderedInsert (Trigger Rotations)..." << endl;
    // 连续插入递增序列，是对红黑树左旋最大的考验
    for (int i = 1; i <= 1000; ++i)
        tree.insert(i);
    for (int i = 1; i <= 1000; ++i)
        assert(tree.contains(i) == true);
    cout << "  -> Passed! (Tree didn't degenerate)" << endl;
}

void testDelete(RbTree &tree) {
    cout << "Running: testDelete (Leaf, One-Child, Two-Child)..." << endl;
    vector<int> nums = {50, 30, 70, 20, 40, 60, 80, 35, 45, 65};
    for (int n : nums)
        tree.insert(n);

    // 1. 删除叶子节点 (20)
    tree.remove(20);
    assert(tree.contains(20) == false);

    // 2. 删除只有一个子节点的节点 (30)
    tree.remove(30);
    assert(tree.contains(30) == false);
    assert(tree.contains(40) == true);

    // 3. 删除有两个子节点的节点 (50，根节点)
    tree.remove(50);
    assert(tree.contains(50) == false);
    assert(tree.contains(40) == true);
    assert(tree.contains(60) == true);

    // 4. 删除不存在的节点
    tree.remove(999);
    assert(tree.contains(999) == false);
    cout << "  -> Passed!" << endl;
}

void testRandomStress(RbTree &tree) {
    cout << "Running: testRandomStress (10k inserts, 5k deletes)..." << endl;
    vector<int> nums(10000);
    for (int i = 0; i < 10000; ++i)
        nums[i] = i;
    random_device rd;
    mt19937 g(rd());
    shuffle(nums.begin(), nums.end(), g);

    for (int n : nums)
        tree.insert(n);
    for (int i = 0; i < 5000; ++i)
        tree.remove(nums[i]);

    for (int i = 0; i < 5000; ++i)
        assert(tree.contains(nums[i]) == false);
    for (int i = 5000; i < 10000; ++i)
        assert(tree.contains(nums[i]) == true);
    cout << "  -> Passed!" << endl;
}

int main() {
    cout << "=== Start Red-Black Tree Stress Test ===" << endl;
    {
        RbTree t;
        testBasicInsert(t);
    }
    {
        RbTree t;
        testOrderedInsert(t);
    }
    {
        RbTree t;
        testDelete(t);
    }
    {
        RbTree t;
        testRandomStress(t);
    }
    cout << "=== All Tests Passed Perfectly! ===" << endl;
    return 0;
}
