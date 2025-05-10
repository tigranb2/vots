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

    auto ValidateTree(int &count) -> bool;

   private:
    struct Node {
        DataType data_;
        Node *parent_;
        Node *left_;
        Node *right_;
        KeyType key_;
        bool is_red_;
        bool is_nil_;  // for leave's children
    };

    // InsertFix restores violated tree invariants, if any are present, after an insert
    void InsertFix(Node *node);
    // DeleteFix restores violated tree invariants, if any are present, after a delete
    void DeleteFix(Node *node);

    void RotateLeft(Node *node);
    void RotateRight(Node *node);

    // NewNode returns a red node with the specified key, data, and parent
    inline auto NewNode(KeyType key, DataType data, Node *parent) -> Node * {
        Node *n = new Node{data, parent, nullptr, nullptr, key, true, false};
        n->left_ = NewDummyNil(n);
        n->right_ = NewDummyNil(n);
        return n;
    }
    // NewDummyNil returns a node representing a parent's NIL child (by spec., these are black)
    inline auto NewDummyNil(Node *parent) -> Node * {
        return new Node{DataType{}, parent, nullptr, nullptr, KeyType{}, false, true};
    }

    auto FindDeleteReplacement(Node *to_delete) -> Node *;
    void ReplaceDeleted(Node *to_delete, Node *replacement);

    Node *root_;
};

}  // namespace vots
