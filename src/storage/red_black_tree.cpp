#include "storage/red_black_tree.h"

#include <functional>
#include <string>

namespace vots {

RBTREE_TEMPLATE
RBTREE_TYPE::RedBlackTree() : root_(NewDummyNil(nullptr)){};

RBTREE_TEMPLATE
auto RBTREE_TYPE::Find(KeyType key) -> DataType * {
    Node *cur = this->root_;
    while (!cur->is_nil_) {
        if (cur->key_ == key) {
            return &cur->data_;
        }
        if (key < cur->key_) {
            cur = cur->left_;
        } else {
            cur = cur->right_;
        }
    }
    return nullptr;
}

RBTREE_TEMPLATE
void RBTREE_TYPE::Insert(KeyType key, DataType data) {
    if (this->root_->is_nil_) {
        this->root_ = this->NewNode(key, data, nullptr);
        this->root_->is_red_ = false;
        return;
    }

    Node *prev = nullptr;
    Node *cur = this->root_;
    while (!cur->is_nil_) {
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
void RBTREE_TYPE::Delete(KeyType key) {
    if (this->root_->is_nil_) {
        return;
    }

    Node *cur = this->root_;
    while (cur->key_ != key) {
        if (key < cur->key_) {
            cur = cur->left_;
        } else {
            cur = cur->right_;
        }
    }

    Node *replacement = this->FindDeleteReplacement(cur);
    Node *fix_candidate = replacement;
    bool should_fix = !cur->is_red_;  // run DeleteFix if replaced node is black
    if (cur == this->root_) {
        this->root_ = replacement;

        // Check if tree is now empty; if so, return
        if (replacement->is_nil_) {
            return;
        }
    }
    if (!cur->left_->is_nil_ && !cur->right_->is_nil_) {
        fix_candidate = replacement->right_;
        should_fix = !replacement->is_red_;  // in this case, run DeleteFix if replacing node was black
        replacement->is_red_ = cur->is_red_;
    }

    this->ReplaceDeleted(cur, replacement);
    if (should_fix) {
        this->DeleteFix(fix_candidate);
    }
}

RBTREE_TEMPLATE
auto RBTREE_TYPE::ValidateTree(int &count) -> bool {
    using Node = typename RBTREE_TYPE::Node;

    std::function<int(const Node *, bool &is_red, int &count)> validate_children = [&](const Node *node, bool &is_red,
                                                                                       int &count) -> int {
        if (node->is_nil_) {
            is_red = false;
            return 1;
        }

        is_red = node->is_red_;
        count++;

        bool left_is_red = false;
        bool right_is_red = false;
        int left_height = validate_children(node->left_, left_is_red, count);
        int right_height = validate_children(node->right_, right_is_red, count);
        if ((left_height != right_height) || (is_red && (left_is_red || right_is_red))) {
            return -1;
        }
        return left_height + (is_red ? 0 : 1);
    };

    bool is_red;
    return validate_children(this->root_, is_red, count) != -1;
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
            if (!uncle->is_nil_ && uncle->is_red_) {
                uncle->is_red_ = false;
                node->parent_->is_red_ = false;
                grandparent->is_red_ = true;
                node = grandparent;
                continue;
            }

            // Node *just* before grandparent in sorted order
            if (node->parent_->right_ == node) {
                node = node->parent_;
                this->RotateLeft(node);
                grandparent = node->parent_->parent_;
            }

            // Node is left child of grandparent's left child
            node->parent_->is_red_ = false;
            grandparent->is_red_ = true;
            RotateRight(grandparent);
        } else {
            uncle = grandparent->left_;

            // Red uncle --> recolor grandparent and its children
            if (!uncle->is_nil_ && uncle->is_red_) {
                uncle->is_red_ = false;
                node->parent_->is_red_ = false;
                grandparent->is_red_ = true;
                node = grandparent;
                continue;
            }

            // Node *just* after grandparent in sorted order
            if (node->parent_->left_ == node) {
                node = node->parent_;
                this->RotateRight(node);
                grandparent = node->parent_->parent_;
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
void RBTREE_TYPE::DeleteFix(Node *node) {
    using RotateFn = void (RBTREE_TYPE::*)(Node *);
    using Child = Node *Node::*;

    RotateFn rotate1;  // left if node is left child; right otherwise
    Child child1;

    RotateFn rotate2;  // right if node is right child; left otherwise
    Child child2;

    while (this->IsBlack(node) && node != this->root_) {
        if (node->parent_->left_ == node) {
            rotate1 = &RBTREE_TYPE::RotateLeft;
            rotate2 = &RBTREE_TYPE::RotateRight;

            child1 = &Node::left_;
            child2 = &Node::right_;
        } else {
            rotate1 = &RBTREE_TYPE::RotateRight;
            rotate2 = &RBTREE_TYPE::RotateLeft;

            child1 = &Node::right_;
            child2 = &Node::left_;
        }

        Node *sibling = node->parent_->*child2;
        if (sibling->is_red_) {
            sibling->is_red_ = false;
            node->parent_->is_red_ = true;
            (this->*rotate1)(node->parent_);  // rotate parent in the direction of `node`
            sibling = node->parent_->*child2;
        }

        // black sibling with black children
        if (this->IsBlack(sibling->left_) && this->IsBlack(sibling->right_)) {
            sibling->is_red_ = true;
            node = node->parent_;
            continue;
        }

        // sibling is black at least 1 red child
        Node *sibling_far_child = sibling->*child2;
        if (this->IsBlack(sibling_far_child)) {
            Node *adjacent_child = sibling->*child1;
            adjacent_child->is_red_ = false;
            sibling->is_red_ = true;
            (this->*rotate2)(sibling);  // rotate sibling in direction opposite of `node`
            sibling = node->parent_->*child2;
        }

        sibling->is_red_ = node->parent_->is_red_;
        node->parent_->is_red_ = false;
        sibling_far_child = sibling->*child2;
        sibling_far_child->is_red_ = false;
        (this->*rotate1)(node->parent_);  // rotate parent in the direction of `node`
        node = this->root_;
    }
    node->is_red_ = false;
}

RBTREE_TEMPLATE
void RBTREE_TYPE::RotateLeft(Node *node) {
    // Given that the method is private, assume that neither node nor node.right
    // are nullptr
    Node *new_parent = node->right_;

    // Update parent's data
    if (node == this->root_) {
        this->root_ = new_parent;
    } else {
        if (node == node->parent_->left_) {
            node->parent_->left_ = new_parent;
        } else {
            node->parent_->right_ = new_parent;
        }
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
    // Given that the method is private, assume that neither node nor node.left are nullptr
    Node *new_parent = node->left_;

    // Update parent's data
    if (node == this->root_) {
        this->root_ = new_parent;
    } else {
        if (node == node->parent_->left_) {
            node->parent_->left_ = new_parent;
        } else {
            node->parent_->right_ = new_parent;
        }
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

RBTREE_TEMPLATE
auto RBTREE_TYPE::FindDeleteReplacement(Node *to_delete) -> Node * {
    if (to_delete->left_->is_nil_) {
        return to_delete->right_;
    }
    if (to_delete->right_->is_nil_) {
        return to_delete->left_;
    }

    // Node's next value, in sorted order
    Node *replacement = to_delete->right_;
    while (!replacement->left_->is_nil_) {
        replacement = replacement->left_;
    }
    return replacement;
}

RBTREE_TEMPLATE
void RBTREE_TYPE::ReplaceDeleted(Node *to_delete, Node *replacement) {
    // update replacement's new parent info
    Node *old_parent = replacement->parent_;
    Node *new_parent = to_delete->parent_;
    if (new_parent != nullptr) {
        if (new_parent->left_ == to_delete) {
            new_parent->left_ = replacement;
        } else {
            new_parent->right_ = replacement;
        }
    }
    replacement->parent_ = new_parent;

    if (replacement->is_nil_) {
        return;
    }

    // invalidate replacement's old parent info
    if (old_parent->left_ == replacement) {
        old_parent->left_ = replacement->right_;
    } else {
        old_parent->right_ = replacement->right_;
    }
    replacement->right_->parent_ = old_parent;

    // update replacement's children info
    if (to_delete->left_ != replacement) {
        replacement->left_ = to_delete->left_;
        replacement->left_->parent_ = replacement;
    }
    if (to_delete->right_ != replacement) {
        replacement->right_ = to_delete->right_;
        replacement->right_->parent_ = replacement;
    }
}

template class RedBlackTree<int, std::string>;
template class RedBlackTree<int, int>;

}  // namespace vots
