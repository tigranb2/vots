#pragma once

#include <gsl/gsl>

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
        std::unique_ptr<Node> left_;
        std::unique_ptr<Node> right_;
        KeyType key_;
        bool is_red_;
        bool is_nil_;  // for leave's children

        Node(DataType data, Node *parent, KeyType key, bool is_red, bool is_nil)
            : data_(std::move(data)),
              parent_(parent),
              left_(nullptr),
              right_(nullptr),
              key_(key),
              is_red_(is_red),
              is_nil_(is_nil) {}
    };

    using NotNullNode = gsl::not_null<Node *>;

    // InsertFix restores violated tree invariants, if any are present, after an insert
    void InsertFix(std::unique_ptr<Node> *node);
    // DeleteFix restores violated tree invariants, if any are present, after a delete
    void DeleteFix(std::unique_ptr<Node> *Node);

    void RotateLeft(std::unique_ptr<Node> &node);
    void RotateRight(std::unique_ptr<Node> &node);

    // NewNode returns a red node with the specified key, data, and parent
    inline auto NewNode(KeyType key, DataType data, Node *parent) -> std::unique_ptr<Node> {
        auto n = std::make_unique<Node>(data, parent, key, true, false);
        n->left_ = std::move(NewDummyNil(n.get()));
        n->right_ = std::move(NewDummyNil(n.get()));
        return n;
    }
    // NewDummyNil returns a node representing a parent's NIL child (by spec., these are black)
    inline auto NewDummyNil(Node *parent) -> std::unique_ptr<Node> {
        return std::make_unique<Node>(DataType{}, parent, KeyType{}, false, true);
    }

    // GetNodeOwner returns the unique_ptr owning the provided, not_null Node pointer
    inline auto GetNodeOwner(NotNullNode node) -> std::unique_ptr<Node> & {
        Node *parent = node->parent_;
        if (parent == nullptr) {
            return this->root_;
        }

        if (parent->left_.get() == node) {
            return parent->left_;
        }
        return parent->right_;
    }

    auto FindDeleteReplacement(NotNullNode to_delete) -> std::unique_ptr<Node> &;
    void ReplaceDeleted(NotNullNode to_delete, std::unique_ptr<Node> *replacement);

    std::unique_ptr<Node> root_;
};

}  // namespace vots
