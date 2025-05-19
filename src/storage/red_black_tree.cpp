#include "storage/red_black_tree.h"

#include <cassert>
#include <functional>
#include <string>
#include <utility>

namespace vots {

RBTREE_TEMPLATE
RBTREE_TYPE::RedBlackTree() : root_(std::move(NewDummyNil(nullptr))){};

RBTREE_TEMPLATE
auto RBTREE_TYPE::Find(KeyType key) -> DataType * {
    Node *cur = this->root_.get();
    while (!cur->is_nil_) {
        if (cur->key_ == key) {
            return &cur->data_;
        }
        if (key < cur->key_) {
            cur = cur->left_.get();
        } else {
            cur = cur->right_.get();
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
    Node *cur = this->root_.get();
    while (!cur->is_nil_) {
        prev = cur;

        if (key < cur->key_) {
            cur = cur->left_.get();
        } else {
            cur = cur->right_.get();
        }
    }

    std::unique_ptr<Node> new_node = this->NewNode(key, data, prev);
    if (key < prev->key_) {
        prev->left_ = std::move(new_node);
        this->InsertFix(&prev->left_);
    } else {
        prev->right_ = std::move(new_node);
        this->InsertFix(&prev->right_);
    }
}

RBTREE_TEMPLATE
void RBTREE_TYPE::Delete(KeyType key) {
    if (this->root_->is_nil_) {
        return;
    }

    Node *cur = this->root_.get();
    while (cur->key_ != key) {
        if (key < cur->key_) {
            cur = cur->left_.get();
        } else {
            cur = cur->right_.get();
        }
    }

    std::unique_ptr<Node> &replacement = this->FindDeleteReplacement(cur);
    Node *fix_candidate = replacement.get();
    bool should_fix = !cur->is_red_;  // run DeleteFix if replaced node is black
    if (!cur->left_->is_nil_ && !cur->right_->is_nil_) {
        fix_candidate = replacement->right_.get();
        should_fix = !replacement->is_red_;  // in this case, run DeleteFix if replacing node was black
        replacement->is_red_ = cur->is_red_;
    }

    this->ReplaceDeleted(cur, &replacement);
    if (should_fix) {
        std::unique_ptr<Node> &fc_unique_ptr = this->GetNodeOwner(fix_candidate);
        this->DeleteFix(&fc_unique_ptr);
    }
}

RBTREE_TEMPLATE
auto RBTREE_TYPE::ValidateTree(int &count) -> bool {
    std::function<int(const std::unique_ptr<Node> &, bool &is_red, int &count)> validate_children =
        [&](const std::unique_ptr<Node> &node, bool &is_red, int &count) -> int {
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
void RBTREE_TYPE::InsertFix(std::unique_ptr<Node> *node) {
    using RotateFn = void (RBTREE_TYPE::*)(std::unique_ptr<Node> & node);
    using Child = std::unique_ptr<Node> Node::*;

    RotateFn rotate1;  // left if node is left child; right otherwise

    RotateFn rotate2;  // right if node is right child; left otherwise
    Child child2;

    // Red node's parent cannot be red
    while ((*node)->parent_ && (*node)->parent_->is_red_) {
        Node *grandparent = (*node)->parent_->parent_;
        if (grandparent->left_.get() == (*node)->parent_) {
            rotate1 = &RBTREE_TYPE::RotateLeft;
            rotate2 = &RBTREE_TYPE::RotateRight;
            child2 = &Node::right_;
        } else {
            rotate1 = &RBTREE_TYPE::RotateRight;
            rotate2 = &RBTREE_TYPE::RotateLeft;
            child2 = &Node::left_;
        }

        // Cannot be nullptr since node->parent_ is red and therefore not the root
        Node *uncle = (grandparent->*child2).get();

        // Red uncle --> recolor grandparent and its children
        if (!uncle->is_nil_ && uncle->is_red_) {
            uncle->is_red_ = false;
            (*node)->parent_->is_red_ = false;
            grandparent->is_red_ = true;
            node = &this->GetNodeOwner(grandparent);
            continue;
        }

        // Node *just* before grandparent in sorted order
        if ((*node)->parent_->*child2 == (*node)) {
            node = &this->GetNodeOwner((*node)->parent_);
            Node *node_ptr = node->get();
            (this->*rotate1)((*node));
            node = &this->GetNodeOwner(node_ptr);
            grandparent = node_ptr->parent_->parent_;
        }

        // Node is grandparent's far child
        (*node)->parent_->is_red_ = false;
        grandparent->is_red_ = true;
        std::unique_ptr<Node> &gp_unique_ptr = this->GetNodeOwner(grandparent);
        (this->*rotate2)(gp_unique_ptr);
    }
    this->root_->is_red_ = false;
}

RBTREE_TEMPLATE
void RBTREE_TYPE::DeleteFix(std::unique_ptr<Node> *node) {
    using RotateFn = void (RBTREE_TYPE::*)(std::unique_ptr<Node> & node);
    using Child = std::unique_ptr<Node> Node::*;

    RotateFn rotate1;  // left if node is left child; right otherwise
    Child child1;

    RotateFn rotate2;  // right if node is right child; left otherwise
    Child child2;

    assert(node);
    assert((*node).get());

    while (!(*node)->is_red_ && (*node) != this->root_) {
        if ((*node)->parent_->left_ == (*node)) {
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

        std::unique_ptr<Node> *sibling = &((*node)->parent_->*child2);
        if ((*sibling)->is_red_) {
            (*sibling)->is_red_ = false;
            (*node)->parent_->is_red_ = true;
            std::unique_ptr<Node> &p_unique_ptr = this->GetNodeOwner((*node)->parent_);
            (this->*rotate1)(p_unique_ptr);  // rotate parent in the direction of `node`
            sibling = &((*node)->parent_->*child2);
        }

        // black sibling with black children
        if (!(*sibling)->left_->is_red_ && !(*sibling)->right_->is_red_) {
            (*sibling)->is_red_ = true;
            node = &this->GetNodeOwner((*node)->parent_);

            assert(node);
            assert((*node).get());
            continue;
        }

        // sibling is black at least 1 red child
        std::unique_ptr<Node> *sibling_far_child = &(sibling->get()->*child2);
        if (!(*sibling_far_child)->is_red_) {
            std::unique_ptr<Node> &adjacent_child = sibling->get()->*child1;
            adjacent_child->is_red_ = false;
            (*sibling)->is_red_ = true;
            (this->*rotate2)((*sibling));  // rotate sibling in direction opposite of `node`
            sibling = &((*node)->parent_->*child2);
        }

        assert(sibling);
        assert((*sibling).get());

        (*sibling)->is_red_ = (*node)->parent_->is_red_;
        (*node)->parent_->is_red_ = false;
        sibling_far_child = &(sibling->get()->*child2);
        (*sibling_far_child)->is_red_ = false;
        std::unique_ptr<Node> &p_unique_ptr = this->GetNodeOwner((*node)->parent_);
        (this->*rotate1)(p_unique_ptr);  // rotate parent in the direction of `node`
        node = &this->root_;
    }
    (*node)->is_red_ = false;
}

RBTREE_TEMPLATE
void RBTREE_TYPE::RotateLeft(std::unique_ptr<Node> &node) {
    assert(node.get());
    std::unique_ptr<Node> new_parent = std::move(node->right_);

    // move over new_parent's old left child
    node->right_ = std::move(new_parent->left_);
    node->right_->parent_ = node.get();

    Node *node_parent = node->parent_;
    bool node_was_left = node_parent && (node_parent->left_ == node);

    // move node over to be new_parent's child
    new_parent->left_ = std::move(node);
    new_parent->left_->parent_ = new_parent.get();

    // update new_parent's parentage
    if (node_parent == nullptr) {
        this->root_ = std::move(new_parent);
        this->root_->parent_ = nullptr;
    } else {
        if (node_was_left) {
            node_parent->left_ = std::move(new_parent);
            node_parent->left_->parent_ = node_parent;
        } else {
            node_parent->right_ = std::move(new_parent);
            node_parent->right_->parent_ = node_parent;
        }
    }
}

RBTREE_TEMPLATE
void RBTREE_TYPE::RotateRight(std::unique_ptr<Node> &node) {
    assert(node.get());
    std::unique_ptr<Node> new_parent = std::move(node->left_);

    // move over new_parent's old right child
    node->left_ = std::move(new_parent->right_);
    node->left_->parent_ = node.get();

    Node *node_parent = node->parent_;
    bool node_was_left = node_parent && (node_parent->left_ == node);

    // move node over to be new_parent's child
    new_parent->right_ = std::move(node);
    new_parent->right_->parent_ = new_parent.get();

    // update new_parent's parentage
    if (node_parent == nullptr) {
        this->root_ = std::move(new_parent);
        this->root_->parent_ = nullptr;
    } else {
        if (node_was_left) {
            node_parent->left_ = std::move(new_parent);
            node_parent->left_->parent_ = node_parent;
        } else {
            node_parent->right_ = std::move(new_parent);
            node_parent->right_->parent_ = node_parent;
        }
    }
}

RBTREE_TEMPLATE
auto RBTREE_TYPE::FindDeleteReplacement(Node *to_delete) -> std::unique_ptr<Node> & {
    if (to_delete->left_->is_nil_) {
        return to_delete->right_;
    }
    if (to_delete->right_->is_nil_) {
        return to_delete->left_;
    }

    // Node's next value, in sorted order
    std::unique_ptr<Node> *replacement = &to_delete->right_;
    while (!(*replacement)->left_->is_nil_) {
        replacement = &(*replacement)->left_;
    }
    return *replacement;
}

RBTREE_TEMPLATE
void RBTREE_TYPE::ReplaceDeleted(Node *to_delete, std::unique_ptr<Node> *replacement) {
    std::unique_ptr<Node> replacement_holder;
    if (!(*replacement)->is_nil_) {
        // invalidate replacement's old parent info
        Node *old_parent = (*replacement)->parent_;
        if (old_parent != to_delete) {
            if (old_parent->left_ == (*replacement)) {
                replacement_holder = std::move(old_parent->left_);
                replacement = &replacement_holder;
                old_parent->left_ = std::move((*replacement)->right_);
                old_parent->left_->parent_ = old_parent;
            } else {
                replacement_holder = std::move(old_parent->right_);
                replacement = &replacement_holder;
                old_parent->right_ = std::move((*replacement)->right_);
                old_parent->right_->parent_ = old_parent;
            }
        }

        // update replacement's children info
        if (to_delete->left_ != (*replacement)) {
            (*replacement)->left_ = std::move(to_delete->left_);
            (*replacement)->left_->parent_ = (*replacement).get();
        }
        if (to_delete->right_ != (*replacement)) {
            (*replacement)->right_ = std::move(to_delete->right_);
            (*replacement)->right_->parent_ = (*replacement).get();
        }
    }

    // update replacement's new parent info
    Node *new_parent = to_delete->parent_;
    if (new_parent) {
        if (new_parent->left_.get() == to_delete) {
            new_parent->left_ = std::move((*replacement));
            new_parent->left_->parent_ = new_parent;
        } else {
            new_parent->right_ = std::move((*replacement));
            new_parent->right_->parent_ = new_parent;
        }
    } else {
        this->root_ = std::move((*replacement));
        this->root_->parent_ = nullptr;
    }
}

template class RedBlackTree<int, std::string>;
template class RedBlackTree<int, int>;

}  // namespace vots
