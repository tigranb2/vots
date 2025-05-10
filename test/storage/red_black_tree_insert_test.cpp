#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "storage/red_black_tree.h"

namespace vots {

TEST(RedBlackTreeInsertTest, InsertOnEmptyTree) {
    auto tree = new RedBlackTree<int, std::string>;
    tree->Insert(1, "a");
    int count = 0;
    ASSERT_TRUE(tree->ValidateTree(count));
    ASSERT_EQ(count, 1);
    ASSERT_EQ(*tree->Find(1), "a");
}

TEST(RedBlackTreeInsertTest, IncreasingSequentialInsert) {
    auto tree = new RedBlackTree<int, std::string>;

    std::vector<std::string> vals = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};

    for (int i = 0; i < 10; ++i) {
        tree->Insert(i, vals[i]);
        int count = 0;
        ASSERT_TRUE(tree->ValidateTree(count));
        ASSERT_EQ(count, i + 1);
    }
    for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(*tree->Find(i), vals[i]);
    }
}

TEST(RedBlackTreeInsertTest, DecreasingSequentialInsert) {
    auto tree = new RedBlackTree<int, std::string>;

    std::vector<std::string> vals = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};

    for (int i = 9; i >= 0; --i) {
        tree->Insert(i, vals[i]);
        int count = 0;
        ASSERT_TRUE(tree->ValidateTree(count));
        ASSERT_EQ(count, 10 - i);
    }
    for (int i = 9; i >= 0; --i) {
        ASSERT_EQ(*tree->Find(i), vals[i]);
    }
}

TEST(RedBlackTreeInsertTest, IncreasingSequentialInsertLarge) {
    auto tree = new RedBlackTree<int, int>;
    for (int i = 0; i < 100000; ++i) {
        tree->Insert(i, -i);
    }
    int count = 0;
    ASSERT_TRUE(tree->ValidateTree(count));
    ASSERT_EQ(count, 100000);
    for (int i = 0; i < 100000; ++i) {
        ASSERT_EQ(*tree->Find(i), -i);
    }
}

TEST(RedBlackTreeInsertTest, DecreasingSequentialInsertLarge) {
    auto tree = new RedBlackTree<int, int>;

    for (int i = 99999; i >= 0; --i) {
        tree->Insert(i, -i);
    }
    int count = 0;
    ASSERT_TRUE(tree->ValidateTree(count));
    ASSERT_EQ(count, 100000);
    for (int i = 99999; i >= 0; --i) {
        ASSERT_EQ(*tree->Find(i), -i);
    }
}

TEST(RedBlackTreeInsertTest, RedUncle) {
    auto tree = new RedBlackTree<int, std::string>;
    tree->Insert(2, "a");
    tree->Insert(3, "b");
    tree->Insert(1, "c");
    tree->Insert(0, "d");  // uncle 3 will be red

    int count = 0;
    ASSERT_TRUE(tree->ValidateTree(count));
    ASSERT_EQ(count, 4);

    ASSERT_EQ(*tree->Find(2), "a");
    ASSERT_EQ(*tree->Find(3), "b");
    ASSERT_EQ(*tree->Find(1), "c");
    ASSERT_EQ(*tree->Find(0), "d");
}

TEST(RedBlackTreeInsertTest, Case4) {
    auto tree_left = new RedBlackTree<int, std::string>;
    tree_left->Insert(2, "a");
    tree_left->Insert(3, "b");
    tree_left->Insert(1, "c");
    tree_left->Insert(0, "d");
    tree_left->Insert(-1, "e");  // red parent (d), left child of left child

    int count_left = 0;
    ASSERT_TRUE(tree_left->ValidateTree(count_left));
    ASSERT_EQ(count_left, 5);

    ASSERT_EQ(*tree_left->Find(2), "a");
    ASSERT_EQ(*tree_left->Find(3), "b");
    ASSERT_EQ(*tree_left->Find(1), "c");
    ASSERT_EQ(*tree_left->Find(0), "d");
    ASSERT_EQ(*tree_left->Find(-1), "e");

    auto tree_right = new RedBlackTree<int, std::string>;
    tree_right->Insert(2, "a");
    tree_right->Insert(3, "b");
    tree_right->Insert(1, "c");
    tree_right->Insert(4, "d");
    tree_right->Insert(5, "e");  // red parent (d), right child of right child

    int count_right = 0;
    ASSERT_TRUE(tree_right->ValidateTree(count_right));
    ASSERT_EQ(count_right, 5);

    ASSERT_EQ(*tree_right->Find(2), "a");
    ASSERT_EQ(*tree_right->Find(3), "b");
    ASSERT_EQ(*tree_right->Find(1), "c");
    ASSERT_EQ(*tree_right->Find(4), "d");
    ASSERT_EQ(*tree_right->Find(5), "e");
}

TEST(RedBlackTreeInsertTest, Case3) {
    auto tree_left = new RedBlackTree<int, std::string>;
    tree_left->Insert(3, "a");
    tree_left->Insert(4, "b");
    tree_left->Insert(2, "c");
    tree_left->Insert(0, "d");
    tree_left->Insert(1, "e");  // red parent (d), right child of left child

    int count_left = 0;
    ASSERT_TRUE(tree_left->ValidateTree(count_left));
    ASSERT_EQ(count_left, 5);

    ASSERT_EQ(*tree_left->Find(3), "a");
    ASSERT_EQ(*tree_left->Find(4), "b");
    ASSERT_EQ(*tree_left->Find(2), "c");
    ASSERT_EQ(*tree_left->Find(0), "d");
    ASSERT_EQ(*tree_left->Find(1), "e");

    auto tree_right = new RedBlackTree<int, std::string>;
    tree_right->Insert(2, "a");
    tree_right->Insert(3, "b");
    tree_right->Insert(1, "c");
    tree_right->Insert(5, "d");
    tree_right->Insert(4, "e");  // red parent (d), left child of right child

    int count_right = 0;
    ASSERT_TRUE(tree_right->ValidateTree(count_right));
    ASSERT_EQ(count_right, 5);

    ASSERT_EQ(*tree_right->Find(2), "a");
    ASSERT_EQ(*tree_right->Find(3), "b");
    ASSERT_EQ(*tree_right->Find(1), "c");
    ASSERT_EQ(*tree_right->Find(5), "d");
    ASSERT_EQ(*tree_right->Find(4), "e");
}

TEST(RedBlackTreeInsertTest, RandomInsert) {
    auto tree = new RedBlackTree<int, int>;
    int c = 0;
    tree->Insert(1, -1);
    ASSERT_TRUE(tree->ValidateTree(c));
    tree->Insert(7, -7);
    ASSERT_TRUE(tree->ValidateTree(c));
    tree->Insert(3, -3);
    ASSERT_TRUE(tree->ValidateTree(c));
    tree->Insert(12, -12);
    ASSERT_TRUE(tree->ValidateTree(c));
    tree->Insert(-12, 12);
    ASSERT_TRUE(tree->ValidateTree(c));
    tree->Insert(4, -4);
    ASSERT_TRUE(tree->ValidateTree(c));
    tree->Insert(0, 0);
    ASSERT_TRUE(tree->ValidateTree(c));
    tree->Insert(90, -90);
    ASSERT_TRUE(tree->ValidateTree(c));
    tree->Insert(55, -55);
    ASSERT_TRUE(tree->ValidateTree(c));
    tree->Insert(6, -6);
    ASSERT_TRUE(tree->ValidateTree(c));
    tree->Insert(9, -9);
    ASSERT_TRUE(tree->ValidateTree(c));
    tree->Insert(2, -2);
    ASSERT_TRUE(tree->ValidateTree(c));

    ASSERT_EQ(*tree->Find(1), -1);
    ASSERT_EQ(*tree->Find(7), -7);
    ASSERT_EQ(*tree->Find(3), -3);
    ASSERT_EQ(*tree->Find(12), -12);
    ASSERT_EQ(*tree->Find(4), -4);
    ASSERT_EQ(*tree->Find(0), 0);
    ASSERT_EQ(*tree->Find(90), -90);
    ASSERT_EQ(*tree->Find(55), -55);
    ASSERT_EQ(*tree->Find(6), -6);
    ASSERT_EQ(*tree->Find(9), -9);
    ASSERT_EQ(*tree->Find(2), -2);
}

TEST(RedBlackTreeInsertTest, RandomInsertLarge) {
    for (int i = 0; i < 5; ++i) {
        auto tree = new RedBlackTree<int, int>;

        std::vector<int> vec(100000);
        std::iota(vec.begin(), vec.end(), 0);  // Fill with 0 to 99999
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(vec.begin(), vec.end(), g);

        int size = static_cast<int>(vec.size());
        for (int i = 0; i < size; ++i) {
            tree->Insert(vec[i], -vec[i]);
        }

        int count = 0;
        ASSERT_TRUE(tree->ValidateTree(count));
        ASSERT_EQ(count, 100000);

        for (int i = 0; i < size; ++i) {
            ASSERT_EQ(*tree->Find(vec[i]), -vec[i]);
        }
    }
}

}  // namespace vots
