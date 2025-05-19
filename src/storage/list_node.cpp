#include "storage/list_node.h"

namespace vots {

LN_TEMPLATE
auto LN_TYPE::Make(KeyType key, DataType data) -> std::unique_ptr<ListNode> {
    return std::unique_ptr<ListNode>(new ListNode(key, data));
}

LN_TEMPLATE
auto LN_TYPE::Add(std::unique_ptr<ListNode> &new_node) -> std::unique_ptr<ListNode> & {
    new_node->prev_ = this;
    this->next_ = std::move(new_node);
    return this->next_;
}

LN_TEMPLATE
auto LN_TYPE::Add(KeyType key, DataType data) -> std::unique_ptr<ListNode> & {
    std::unique_ptr<ListNode> new_node = LN_TYPE::Make(key, data);
    return this->Add(new_node);
}

LN_TEMPLATE
void LN_TYPE::Remove() {
    if (this->next_.get()) {
        this->next_->prev_ = this->prev_;
    }

    if (this->prev_) {
        this->prev_->next_ = std::move(this->next_);
    }
}

// GetOwner returns a pointer to the unique_ptr owning the provided ListNode pointer. If the head node is provided
// the owner cannot be determined and a nullptr is returned instead.
LN_TEMPLATE
auto LN_TYPE::GetOwner(ListNode *node) -> std::unique_ptr<ListNode> * {
    if (node->prev_ == nullptr) {
        return nullptr;
    }
    return &(node->prev_->next_);
}

LN_TEMPLATE
auto LN_TYPE::ValidateList(const std::unique_ptr<ListNode> &head,
                           const std::vector<std::pair<KeyType, DataType>> &expected) -> bool {
    ListNode *node = head.get();
    for (const auto &expect : expected) {
        if (node == nullptr) {
            return false;
        }

        if (node->key_ != expect.first || node->data_ != expect.second) {
            return false;
        }

        node = node->next_.get();
    }

    return node == nullptr;
}

template class ListNode<int, int>;

}  // namespace vots
