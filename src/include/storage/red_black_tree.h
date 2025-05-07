#pragma once

namespace vots {

#define RBTREE_TEMPLATE template <typename KeyType, typename DataType>
#define RBTREE_TYPE RedBlackTree<KeyType, DataType>

/*
 * RedBlackTree implements a Red-Black tree, and is used to store limit objects.
 *
 * The self-balancing property of Red-Black trees ensures that the bid and ask limit trees always provide worst-case
 * O(log m) operations, where m is the size of the tree. A Red-Black tree is preferred over other self-balancing
 * trees as its relaxed balancing property result in fewer (costly) rotations in the write-heavy workloads typical
 * of the market. This comes at the trade-off of less-efficient lookups, but the use of a hashmap for tracking limit
 * prices eliminates the need for lookups in the limit trees altogether.
 */
RBTREE_TEMPLATE class RedBlackTree {
   public:
    RedBlackTree();

    // Find returns a nullptr if the data with the specified key was not found
    auto Find(KeyType key) -> DataType *;
    void Insert(KeyType key, DataType data);
    void Delete(KeyType key);

    auto ValidateTree() -> bool;

   private:
    struct Node {
        DataType data_;
        Node *parent_;
        Node *left_;
        Node *right_;
        KeyType key_;
        bool is_red_;
    };

    // InsertFix restores violated tree invariants, if any are present, after an insert
    void InsertFix(Node *node);
    // DeleteFix restores violated tree invariants, if any are present, after a delete
    void DeleteFix(Node *node);

    void RotateLeft(Node *node);
    void RotateRight(Node *node);

    // NewNode returns a red node with the specified key, data, and parent
    inline auto NewNode(KeyType key, DataType data, Node *parent) -> Node * {
        return new Node{data, parent, nullptr, nullptr, key, true};
    }

    // A node is black if it is a leaf's child (i.e. null) or if it is not marked as red
    inline auto IsBlack(Node *node) -> bool { return node == nullptr || !node->is_red_; }

    auto FindDeleteReplacement(Node *to_delete) -> Node *;
    void ReplaceDeleted(Node *to_delete, Node *replacement);

    Node *root_;
};

}  // namespace vots
