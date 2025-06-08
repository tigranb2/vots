#include "storage/linked_list.h"

namespace vots {

LL_TEMPLATE
void LL_TYPE::Add(KeyType key, DataType data) {
    auto new_node = std::unique_ptr<LinkedList::Node>(new LinkedList::Node(key, data));
    this->node_map_.emplace(key, new_node.get());
    if (this->tail_ == nullptr) {
        head_ = std::move(new_node);
        tail_ = this->head_.get();
        return;
    }

    new_node->prev_ = this->tail_;
    this->tail_->next_ = std::move(new_node);
    this->tail_ = this->tail_->next_.get();
}

LL_TEMPLATE
void LL_TYPE::Remove(KeyType key) {
    Node &to_delete = *this->node_map_.at(key);
    if (to_delete.next_.get() != nullptr) {
        to_delete.next_->prev_ = to_delete.prev_;
    } else {
        this->tail_ = to_delete.prev_;
    }

    if (to_delete.prev_ != nullptr) {
        to_delete.prev_->next_ = std::move(to_delete.next_);
    } else {
        this->head_ = std::move(to_delete.next_);
    }
}

LL_TEMPLATE
auto LL_TYPE::ValidateList(const LinkedList &list, const std::vector<std::pair<KeyType, DataType>> &expected) -> bool {
    Node *node = list.head_.get();
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

template class LinkedList<int, int>;

}  // namespace vots
