#include "storage/list_node.h"

namespace vots {

LN_TEMPLATE
auto LN_TYPE::Make(KeyType key, DataType data) -> std::unique_ptr<ListNode> {
    return std::unique_ptr<ListNode>(new ListNode(key, data));
}

LN_TEMPLATE
auto LN_TYPE::Add(std::unique_ptr<ListNode> &prev, std::unique_ptr<ListNode> new_node) -> std::unique_ptr<ListNode> & {
    new_node->prev_ = prev.get();
    prev->next_ = std::move(new_node);
    return prev->next_;
}

LN_TEMPLATE
auto LN_TYPE::Add(std::unique_ptr<ListNode> &prev, KeyType key, DataType data) -> std::unique_ptr<ListNode> & {
    std::unique_ptr<ListNode> new_node = LN_TYPE::Make(key, data);
    return this->Add(prev, new_node);
}

LN_TEMPLATE
void LN_TYPE::Delete(std::unique_ptr<ListNode> &to_delete) {
    if (to_delete->next_.get()) {
        to_delete->next_->prev_ = to_delete->prev_;
    }

    if (to_delete->prev_) {
        to_delete->prev_->next_ = to_delete->next_;
    }
}

// GetOwner returns a pointer to the unique_ptr owning the provided ListNode pointer. If the head node is provided
// the owner cannot be determined and a nullptr is returned instead.
LN_TEMPLATE
auto LN_TYPE::GetOwner(ListNode *node) -> std::unique_ptr<ListNode> * {
    if (!(*node)->prev_) {
        return nullptr;
    }
    return (*node)->prev_->next_;
}

}  // namespace vots
