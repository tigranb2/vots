#include <numeric>
#include <random>
#include <string>

#include "gtest/gtest.h"
#include "storage/red_black_tree.h"

namespace vots {

TEST(RedBlackTreeInsertTest, ExtreamOfEmptyTree) {
    auto tree = RedBlackTree<int, std::string>();
    ASSERT_EQ(tree.GetMin(), nullptr);
    ASSERT_EQ(tree.GetMax(), nullptr);

    tree.Insert(1, "a");
    tree.Insert(2, "a");
    tree.Insert(4, "a");
    tree.Insert(-333, "a");

    tree.Delete(2);
    tree.Delete(-333);
    tree.Delete(1);
    tree.Delete(4);

    ASSERT_EQ(tree.GetMin(), nullptr);
    ASSERT_EQ(tree.GetMax(), nullptr);
}

TEST(RedBlackTreeInsertTest, ExtremeOfSequentialIncreasing) {
    auto tree = RedBlackTree<int, std::string>();

    std::vector<std::string> vals = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};

    RedBlackTree<int, std::string>::Node *min_inserted;
    for (int i = 0; i < 10; ++i) {
        RedBlackTree<int, std::string>::Node &inserted = tree.Insert(i, vals[i]);
        if (i == 0) {
            min_inserted = &inserted;
        }

        ASSERT_EQ(tree.GetMin(), min_inserted);
        ASSERT_EQ(tree.GetMax(), &inserted);
    }
}

TEST(RedBlackTreeInsertTest, ExtremeOfSequentialDecreasing) {
    auto tree = RedBlackTree<int, std::string>();

    std::vector<std::string> vals = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};

    RedBlackTree<int, std::string>::Node *max_inserted;
    for (int i = 9; i >= 0; --i) {
        RedBlackTree<int, std::string>::Node &inserted = tree.Insert(i, vals[i]);
        if (i == 9) {
            max_inserted = &inserted;
        }

        ASSERT_EQ(tree.GetMin(), &inserted);
        ASSERT_EQ(tree.GetMax(), max_inserted);
    }
}

TEST(RedBlackTreeInsertTest, ExtremeOfRandomInsert) {
    auto tree = RedBlackTree<int, int>();

    std::vector<int> vec(100000);
    std::iota(vec.begin(), vec.end(), 0);  // Fill with 0 to 99999
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vec.begin(), vec.end(), g);

    RedBlackTree<int, int>::Node *min_inserted;
    RedBlackTree<int, int>::Node *max_inserted;
    for (int i = 0; i < 100000; ++i) {
        RedBlackTree<int, int>::Node &inserted = tree.Insert(vec[i], -vec[i]);

        if (vec[i] == 0) {
            min_inserted = &inserted;
        } else if (vec[i] == 99999) {
            max_inserted = &inserted;
        }
    }

    ASSERT_EQ(tree.GetMin(), min_inserted);
    ASSERT_EQ(tree.GetMax(), max_inserted);
}

TEST(RedBlackTreeInsertTest, ExtremeAfterDelete) {
    auto tree = RedBlackTree<int, int>();
    RedBlackTree<int, int>::Node &min_inserted = tree.Insert(0, 0);
    RedBlackTree<int, int>::Node &max_inserted = tree.Insert(2, 2);
    RedBlackTree<int, int>::Node &inserted = tree.Insert(1, 1);

    ASSERT_EQ(tree.GetMin(), &min_inserted);
    ASSERT_EQ(tree.GetMax(), &max_inserted);

    tree.Delete(*tree.GetMin());
    ASSERT_EQ(tree.GetMin(), &inserted);
    tree.Delete(*tree.GetMax());
    ASSERT_EQ(tree.GetMax(), &inserted);
}

TEST(RedBlackTreeInsertTest, ExtremeAfterDeleteLarge) {
    for (int idx = 0; idx < 5; ++idx) {
        auto tree = RedBlackTree<int, int>();
        int target_min = 2234;
        int target_max = 5927;

        std::vector<int> vec(100000);
        std::iota(vec.begin(), vec.end(), 0);  // Fill with 0 to 99999
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(vec.begin(), vec.end(), g);

        int size = static_cast<int>(vec.size());

        RedBlackTree<int, int>::Node *min_inserted;
        RedBlackTree<int, int>::Node *max_inserted;
        for (int i = 0; i < size; ++i) {
            RedBlackTree<int, int>::Node &inserted = tree.Insert(vec[i], -vec[i]);

            if (vec[i] == target_min) {
                min_inserted = &inserted;
            } else if (vec[i] == target_max) {
                max_inserted = &inserted;
            }
        }

        std::shuffle(vec.begin(), vec.end(), g);
        for (int i = 0; i < size; ++i) {
            if (vec[i] < target_min || vec[i] > target_max) {
                tree.Delete(vec[i]);
            }
        }

        ASSERT_EQ(tree.GetMin(), min_inserted);
        ASSERT_EQ(tree.GetMax(), max_inserted);
    }
}

}  // namespace vots
