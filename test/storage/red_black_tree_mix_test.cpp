#include <algorithm>
#include <random>
#include <vector>

#include "gtest/gtest.h"
#include "storage/red_black_tree.h"

namespace vots {

TEST(RedBlackTreeMixTest, RandomMixSmall) {
    for (int i = 0; i < 5; ++i) {
        std::random_device rd;
        std::mt19937 gen(rd());

        // First batch of inserts
        std::uniform_int_distribution<int> count_dist(5, 15);
        int num_nodes = count_dist(gen);

        std::vector<int> all_keys;
        all_keys.reserve(num_nodes + 1);
        for (int i = 0; i < num_nodes; ++i) {
            all_keys.push_back(i);
        }
        std::shuffle(all_keys.begin(), all_keys.end(), gen);

        std::uniform_int_distribution<size_t> split_dist(1, all_keys.size() - 1);
        size_t first_insert_size = split_dist(gen);

        std::vector<int> first_insert(all_keys.begin(), all_keys.begin() + first_insert_size);
        std::vector<int> remaining(all_keys.begin() + first_insert_size, all_keys.end());

        auto tree = RedBlackTree<int, int>();
        for (int k : first_insert) {
            tree.Insert(k, k);
        }

        int count = 0;
        ASSERT_TRUE(tree.ValidateTree(count));
        ASSERT_EQ(count, first_insert_size);

        // First batch of deletes (some or all of the inserted)
        std::uniform_int_distribution<size_t> first_delete_dist(0, first_insert.size());
        size_t first_delete_size = first_delete_dist(gen);
        std::shuffle(first_insert.begin(), first_insert.end(), gen);

        for (size_t i = 0; i < first_delete_size; ++i) {
            int k = first_insert[i];
            tree.Delete(k);
        }

        count = 0;
        ASSERT_TRUE(tree.ValidateTree(count));
        ASSERT_EQ(count, first_insert_size - first_delete_size);

        // Second batch of inserts (rest of them)
        std::shuffle(remaining.begin(), remaining.end(), gen);
        for (int k : remaining) {
            tree.Insert(k, k);
        }

        count = 0;
        ASSERT_TRUE(tree.ValidateTree(count));
        ASSERT_EQ(count, (num_nodes - first_delete_size));

        // Second batch of deletes (rest of them)
        size_t second_delete_size = remaining.size();
        std::shuffle(remaining.begin(), remaining.end(), gen);

        for (int k : remaining) {
            tree.Delete(k);
        }

        count = 0;
        ASSERT_TRUE(tree.ValidateTree(count));
        ASSERT_EQ(count, num_nodes - first_delete_size - second_delete_size);
    }
}

TEST(RedBlackTreeMixTest, RandomMixLarge) {
    for (int i = 0; i < 5; ++i) {
        std::random_device rd;
        std::mt19937 gen(rd());

        // First batch of inserts
        std::uniform_int_distribution<int> count_dist(70000, 120000);
        int num_nodes = count_dist(gen);

        std::vector<int> all_keys;
        all_keys.reserve(num_nodes + 1);
        for (int i = 0; i < num_nodes; ++i) {
            all_keys.push_back(i);
        }
        std::shuffle(all_keys.begin(), all_keys.end(), gen);

        std::uniform_int_distribution<size_t> split_dist(1, all_keys.size() - 1);
        size_t first_insert_size = split_dist(gen);

        std::vector<int> first_insert(all_keys.begin(), all_keys.begin() + first_insert_size);
        std::vector<int> remaining(all_keys.begin() + first_insert_size, all_keys.end());

        auto tree = RedBlackTree<int, int>();
        for (int k : first_insert) {
            tree.Insert(k, k);
        }

        int count = 0;
        ASSERT_TRUE(tree.ValidateTree(count));
        ASSERT_EQ(count, first_insert_size);

        // First batch of deletes (some or all of the inserted)
        std::uniform_int_distribution<size_t> first_delete_dist(0, first_insert.size());
        size_t first_delete_size = first_delete_dist(gen);
        std::shuffle(first_insert.begin(), first_insert.end(), gen);

        for (size_t i = 0; i < first_delete_size; ++i) {
            int k = first_insert[i];
            tree.Delete(k);
        }

        count = 0;
        ASSERT_TRUE(tree.ValidateTree(count));
        ASSERT_EQ(count, first_insert_size - first_delete_size);

        // Second batch of inserts (rest of them)
        std::shuffle(remaining.begin(), remaining.end(), gen);
        for (int k : remaining) {
            tree.Insert(k, k);
        }

        count = 0;
        ASSERT_TRUE(tree.ValidateTree(count));
        ASSERT_EQ(count, (num_nodes - first_delete_size));

        // Second batch of deletes (rest of them)
        size_t second_delete_size = remaining.size();
        std::shuffle(remaining.begin(), remaining.end(), gen);

        for (int k : remaining) {
            tree.Delete(k);
        }

        count = 0;
        ASSERT_TRUE(tree.ValidateTree(count));
        ASSERT_EQ(count, num_nodes - first_delete_size - second_delete_size);
    }
}

TEST(RedBlackTreeMixTest, DISABLED_RandomMixVeryLarge) {
    for (int i = 0; i < 10; ++i) {
        std::random_device rd;
        std::mt19937 gen(rd());

        // First batch of inserts
        std::uniform_int_distribution<int> count_dist(700000, 1200000);
        int num_nodes = count_dist(gen);

        std::vector<int> all_keys;
        all_keys.reserve(num_nodes + 1);
        for (int i = 0; i < num_nodes; ++i) {
            all_keys.push_back(i);
        }
        std::shuffle(all_keys.begin(), all_keys.end(), gen);

        std::uniform_int_distribution<size_t> split_dist(1, all_keys.size() - 1);
        size_t first_insert_size = split_dist(gen);

        std::vector<int> first_insert(all_keys.begin(), all_keys.begin() + first_insert_size);
        std::vector<int> remaining(all_keys.begin() + first_insert_size, all_keys.end());

        auto tree = RedBlackTree<int, int>();
        for (int k : first_insert) {
            tree.Insert(k, k);
        }

        int count = 0;
        ASSERT_TRUE(tree.ValidateTree(count));
        ASSERT_EQ(count, first_insert_size);

        // First batch of deletes (some or all of the inserted)
        std::uniform_int_distribution<size_t> first_delete_dist(0, first_insert.size());
        size_t first_delete_size = first_delete_dist(gen);
        std::shuffle(first_insert.begin(), first_insert.end(), gen);

        for (size_t i = 0; i < first_delete_size; ++i) {
            int k = first_insert[i];
            tree.Delete(k);
        }

        count = 0;
        ASSERT_TRUE(tree.ValidateTree(count));
        ASSERT_EQ(count, first_insert_size - first_delete_size);

        // Second batch of inserts (rest of them)
        std::shuffle(remaining.begin(), remaining.end(), gen);
        for (int k : remaining) {
            tree.Insert(k, k);
        }

        count = 0;
        ASSERT_TRUE(tree.ValidateTree(count));
        ASSERT_EQ(count, (num_nodes - first_delete_size));

        // Second batch of deletes (rest of them)
        size_t second_delete_size = remaining.size();
        std::shuffle(remaining.begin(), remaining.end(), gen);

        for (int k : remaining) {
            tree.Delete(k);
        }

        count = 0;
        ASSERT_TRUE(tree.ValidateTree(count));
        ASSERT_EQ(count, num_nodes - first_delete_size - second_delete_size);
    }
}

}  // namespace vots
