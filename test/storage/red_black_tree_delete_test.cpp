#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "storage/red_black_tree.h"

namespace vots {

TEST(RedBlackTreeDeleteTest, DeleteOnEmptyTree) {
    auto tree = new RedBlackTree<int, std::string>;
    ASSERT_NO_THROW(tree->Delete(1));
}

TEST(RedBlackTreeDeleteTest, DeleteOnTinyTree) {
    auto tree_left = new RedBlackTree<int, std::string>;
    tree_left->Insert(0, "a");
    tree_left->Insert(1, "b");
    tree_left->Insert(2, "c");
    tree_left->Delete(0);

    int count_left = 0;
    ASSERT_TRUE(tree_left->ValidateTree(count_left));
    ASSERT_EQ(count_left, 2);

    auto tree_right = new RedBlackTree<int, std::string>;
    tree_right->Insert(0, "a");
    tree_right->Insert(1, "b");
    tree_right->Insert(2, "c");
    tree_right->Delete(2);

    int count_right = 0;
    ASSERT_TRUE(tree_right->ValidateTree(count_right));
    ASSERT_EQ(count_right, 2);
}

TEST(RedBlackTreeDeleteTest, DeleteRoot) {
    auto tree_left = new RedBlackTree<int, std::string>;
    tree_left->Insert(0, "a");
    tree_left->Insert(1, "b");
    tree_left->Delete(0);

    int count_left = 0;
    ASSERT_TRUE(tree_left->ValidateTree(count_left));
    ASSERT_EQ(count_left, 1);

    auto tree_right = new RedBlackTree<int, std::string>;
    tree_right->Insert(1, "a");
    tree_right->Insert(0, "b");
    tree_right->Delete(1);

    int count_right = 0;
    ASSERT_TRUE(tree_right->ValidateTree(count_right));
    ASSERT_EQ(count_right, 1);

    auto tree = new RedBlackTree<int, std::string>;
    tree->Insert(0, "a");
    tree->Insert(1, "b");
    tree->Insert(2, "c");
    tree->Delete(1);

    int count = 0;
    ASSERT_TRUE(tree->ValidateTree(count));
    ASSERT_EQ(count, 2);
}

TEST(RedBlackTreeDeleteTest, IncreasingSequentialDelete) {
    auto tree = new RedBlackTree<int, std::string>;

    std::vector<std::string> vals = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};

    for (int i = 0; i < 10; ++i) {
        tree->Insert(i, vals[i]);
    }
    for (int i = 0; i < 10; ++i) {
        tree->Delete(i);
        ASSERT_EQ(tree->Find(i), nullptr) << i;

        int count = 0;
        ASSERT_TRUE(tree->ValidateTree(count));
        ASSERT_EQ(count, 9 - i);
    }
}

TEST(RedBlackTreeDeleteTest, DecreasingSequentialDelete) {
    auto tree = new RedBlackTree<int, std::string>;

    std::vector<std::string> vals = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};

    for (int i = 9; i >= 0; --i) {
        tree->Insert(i, vals[i]);
    }
    for (int i = 9; i >= 0; --i) {
        tree->Delete(i);
        ASSERT_EQ(tree->Find(i), nullptr);

        int count = 0;
        ASSERT_TRUE(tree->ValidateTree(count));
        ASSERT_EQ(count, i);
    }
}

TEST(RedBlackTreeDeleteTest, IncreasingSequentialDeleteLarge) {
    auto tree = new RedBlackTree<int, int>;
    for (int i = 0; i < 100000; ++i) {
        tree->Insert(i, -i);
    }
    for (int i = 0; i < 100000; ++i) {
        tree->Delete(i);
        ASSERT_EQ(tree->Find(i), nullptr);
    }
    int count = 0;
    ASSERT_TRUE(tree->ValidateTree(count));
    ASSERT_EQ(count, 0);
}

TEST(RedBlackTreeDeleteTest, DecreasingSequentialDeleteLarge) {
    auto tree = new RedBlackTree<int, int>;

    for (int i = 99999; i >= 0; --i) {
        tree->Insert(i, -i);
    }
    for (int i = 99999; i >= 0; --i) {
        tree->Delete(i);
        ASSERT_EQ(tree->Find(i), nullptr);
    }
    int count = 0;
    ASSERT_TRUE(tree->ValidateTree(count));
    ASSERT_EQ(count, 0);
}

TEST(RedBlackTreeDeleteTest, SiblingWithAdjacentRedChild) {
    auto tree_left = new RedBlackTree<int, std::string>;
    tree_left->Insert(0, "a");
    tree_left->Insert(1, "b");
    tree_left->Insert(3, "c");
    tree_left->Insert(2, "d");

    tree_left->Delete(0);

    int count_left = 0;
    ASSERT_TRUE(tree_left->ValidateTree(count_left));
    ASSERT_EQ(count_left, 3);

    ASSERT_EQ(tree_left->Find(0), nullptr);
    ASSERT_EQ(*tree_left->Find(1), "b");
    ASSERT_EQ(*tree_left->Find(2), "d");
    ASSERT_EQ(*tree_left->Find(3), "c");

    auto tree_right = new RedBlackTree<int, std::string>;
    tree_right->Insert(3, "c");
    tree_right->Insert(2, "d");
    tree_right->Insert(0, "a");
    tree_right->Insert(1, "b");

    tree_right->Delete(3);

    int count_right = 0;
    ASSERT_TRUE(tree_right->ValidateTree(count_right));
    ASSERT_EQ(count_left, 3);

    ASSERT_EQ(*tree_right->Find(0), "a");
    ASSERT_EQ(*tree_right->Find(1), "b");
    ASSERT_EQ(*tree_right->Find(2), "d");
    ASSERT_EQ(tree_right->Find(3), nullptr);
}

TEST(RedBlackTreeDeleteTest, RandomDelete) {
    auto tree = new RedBlackTree<int, int>;
    tree->Insert(1, -1);
    tree->Insert(7, -7);
    tree->Insert(3, -3);
    tree->Insert(12, -12);
    tree->Insert(-12, 12);
    tree->Insert(4, -4);
    tree->Insert(0, 0);
    tree->Insert(90, -90);
    tree->Insert(55, -55);
    tree->Insert(6, -6);
    tree->Insert(9, -9);
    tree->Insert(2, -2);

    tree->Delete(2);
    int count = 0;
    ASSERT_TRUE(tree->ValidateTree(count));
    ASSERT_EQ(tree->Find(2), nullptr);
    ASSERT_EQ(count, 11);

    tree->Delete(90);
    count = 0;
    ASSERT_TRUE(tree->ValidateTree(count));
    ASSERT_EQ(tree->Find(90), nullptr);
    ASSERT_EQ(count, 10);

    tree->Delete(6);
    count = 0;
    ASSERT_TRUE(tree->ValidateTree(count));
    ASSERT_EQ(tree->Find(6), nullptr);
    ASSERT_EQ(count, 9);

    tree->Delete(-12);
    count = 0;
    ASSERT_TRUE(tree->ValidateTree(count));
    ASSERT_EQ(tree->Find(-12), nullptr);
    ASSERT_EQ(count, 8);

    tree->Delete(3);
    count = 0;
    ASSERT_TRUE(tree->ValidateTree(count));
    ASSERT_EQ(tree->Find(3), nullptr);
    ASSERT_EQ(count, 7);
}

TEST(RedBlackTreeDeleteTest, RandomDeleteLarge) {
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

        std::shuffle(vec.begin(), vec.end(), g);
        for (int i = 0; i < size; ++i) {
            tree->Delete(vec[i]);
            ASSERT_EQ(tree->Find(vec[i]), nullptr);
        }

        int count = 0;
        ASSERT_TRUE(tree->ValidateTree(count));
        ASSERT_EQ(count, 0);
    }
}

}  // namespace vots
