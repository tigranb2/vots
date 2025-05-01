#include "storage/red_black_tree.h"

namespace vots {

RBTREE_TEMPLATE
RBTREE_TYPE::RedBlackTree() : root_(nullptr){};

RBTREE_TEMPLATE
auto RBTREE_TYPE::Find(KeyType key) -> DataType * {
    Node *cur = this->root_;
    while (cur != nullptr) {
        if (cur->key_ == key) {
            return &cur->data_;
        }
        if (cur->key_ < key) {
            cur = cur->left_;
        } else {
            cur = cur->right_;
        }
    }
    return nullptr;
}

RBTREE_TEMPLATE
void RBTREE_TYPE::Insert(KeyType key, DataType data) {
    if (this->root_ == nullptr) {
        this->root_ = this->NewNode(key, data, nullptr);
        this->root_->is_red_ = false;
        return;
    }

    Node *prev = nullptr;
    Node *cur = this->root_;
    while (cur != nullptr) {
        prev = cur;

        if (key < cur->key_) {
            cur = cur->left_;
        } else {
            cur = cur->right_;
        }
    }

    Node *new_node = this->NewNode(key, data, prev);
    if (key < prev->key_) {
        prev->left_ = new_node;
    } else {
        prev->right_ = new_node;
    }
    this->InsertFix(new_node);
}

RBTREE_TEMPLATE
auto RBTREE_TYPE::ValidateTree() -> bool {
    using Node = typename RBTREE_TYPE::Node;

    std::function<int(const Node *, bool &is_red)> validate_children = [&](const Node *node, bool &is_red) -> int {
        if (node == nullptr) {
            is_red = false;
            return 1;
        }
        is_red = node->is_red_;

        bool left_is_red = false;
        bool right_is_red = false;
        int left_height = validate_children(node->left_, left_is_red);
        int right_height = validate_children(node->right_, right_is_red);

        if (is_red && (left_is_red || right_is_red)) {
            return -1;
        }
        return left_height == right_height ? left_height + 1 : -1;
    };

    bool is_red;
    return validate_children(this->root_, is_red);
}

RBTREE_TEMPLATE
void RBTREE_TYPE::InsertFix(Node *node) {
    // Red node's parent cannot be red
    while (node->parent_ != nullptr && node->parent_->is_red_) {
        // Cannot be nullptr since node->parent_'s red & root must be black
        Node *grandparent = node->parent_->parent_;
        Node *uncle;
        if (node->parent_ == grandparent->left_) {
            uncle = grandparent->right_;

            // Red uncle --> recolor grandparent and its children
            if (uncle->is_red_) {
                uncle->is_red_ = false;
                node->parent_->is_red_ = false;
                grandparent->is_red_ = true;
                node = grandparent;
                continue;
            }

            // Node *just* before grandparent in sorted order
            if (node->parent_->right_ == node) {
                node = node->parent_;
                grandparent = node->parent_->parent_;
                this->RotateLeft(node);
                // Continue to case below
            }

            // Node is left child of grandparent's left child
            node->parent_->is_red_ = false;
            grandparent->is_red_ = true;
            RotateRight(grandparent);
        } else {
            uncle = grandparent->left_;

            // Red uncle --> recolor grandparent and its children
            if (uncle->is_red_) {
                uncle->is_red_ = false;
                node->parent_->is_red_ = false;
                grandparent->is_red_ = true;
                node = grandparent;
                continue;
            }

            // Node *just* after grandparent in sorted order
            if (node->parent_->left_ == node) {
                node = node->parent_;
                grandparent = node->parent_->parent_;
                this->RotateRight(node);
            }

            // Node is right child of grandparent's right child
            node->parent_->is_red_ = false;
            grandparent->is_red_ = true;
            RotateLeft(grandparent);
        }
    }
    this->root_->is_red_ = false;
}

RBTREE_TEMPLATE
void RBTREE_TYPE::RotateLeft(Node *node) {
    // Given that the method is private, assume that neither node nor node.right
    // are nullptr
    Node *new_parent = node->right_;

    // Update root
    if (node == this->root_) {
        this->root_ = new_parent;
    }

    // Update parentage
    new_parent->parent_ = node->parent_;
    node->parent_ = new_parent;
    if (new_parent->left_) {
        new_parent->left_->parent_ = node;
    }

    // Update children
    node->right_ = new_parent->left_;
    new_parent->left_ = node;
}

RBTREE_TEMPLATE
void RBTREE_TYPE::RotateRight(Node *node) {
    // Given that the method is private, assume that neither node nor node.left
    // are nullptr
    Node *new_parent = node->left_;

    // Update root
    if (node == this->root_) {
        this->root_ = new_parent;
    }

    // Updae parentage
    new_parent->parent_ = node->parent_;
    node->parent_ = new_parent;
    if (new_parent->right_) {
        new_parent->right_->parent_ = node;
    }

    // Update children
    node->left_ = new_parent->right_;
    new_parent->right_ = node;
}

template class RedBlackTree<int, std::string>;

}  // namespace vots
