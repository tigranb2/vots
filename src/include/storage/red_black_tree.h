#pragma once

namespace vots {

/*
 * RedBlackTree implements a Red-Black tree, and is used to store limit objects.
 *
 * The self-balancing property of Red-Black trees ensures that the bid and ask
 * limit trees always provide worst-case O(log m) operations, where m is the
 * size of the tree. A Red-Black tree is preferred over other self-balancing
 * trees as its relaxed balancing property result in fewer (costly) rotations in
 * the write-heavy workloads typical of the market. This comes at the trade-off
 * of less-efficient lookups, but the use of a hashmap for tracking limit prices
 * eliminates the need for lookups in the limit trees altogether.
 */
template <typename KeyType, typename DataType> class RedBlackTree {
  public:
    RedBlackTree() = default;

    // Find returns a std::nullptr if the data with the specified key was not
    // found
    auto Find(KeyType key) -> DataType *;
    void Insert(KeyType key, DataType data);
    void Delete(KeyType key);

  private:
    struct Node {
        DataType data_;
        Node *parent_;
        Node *left_;
        Node *right_;
        KeyType key_;
        bool is_red_;
    };

    void RotateLeft(Node *node);
    void RotateRight(Node *node);

    // InsertFix restores violated tree invariants, if any are present, after an
    // insert
    void InsertFix(Node *node);
    // DeleteFix restores violated tree invariants, if any are present, after a
    // delete
    void DeleteFix(Node *node);

    Node *root_;
};
} // namespace vots
