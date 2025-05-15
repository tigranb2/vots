#pragma once

#include <memory>

namespace vots {

#define LN_TEMPLATE template <typename KeyType, typename DataType>
#define LN_TYPE ListNode<KeyType, DataType>

/*
 * ListNode implements a simple doubly linked-list *node* and is used to store order objects.
 *
 * Each node contains an identifying key, though with no uniqueness guarantee, and data, accessible directly through
 * public fields.
 */
LN_TEMPLATE class ListNode {
   public:
    KeyType key_;
    DataType data_;

    // Make constructs a ListNode object, wrapped in an std::unique_ptr
    static auto Make(KeyType key, DataType data) -> std::unique_ptr<ListNode>;

    // Add adds the new node to the end of the provide prev node. It returns a reference to the new node.
    auto Add(std::unique_ptr<ListNode> &prev, std::unique_ptr<ListNode> &new_node) -> std::unique_ptr<ListNode> &;

    auto Add(std::unique_ptr<ListNode> &prev, KeyType key, DataType data) -> std::unique_ptr<ListNode> &;

    void Delete(std::unique_ptr<ListNode> &to_delete);

    // GetOwner returns a pointer to the unique_ptr owning the provided ListNode pointer. If the head node is provided
    // the owner cannot be determined and a nullptr is returned instead.
    auto GetOwner(ListNode *node) -> std::unique_ptr<ListNode> *;

   private:
    std::unique_ptr<ListNode> next_;
    ListNode *prev_;

    ListNode(KeyType key, DataType data) : key_(key), data_(std::move(data)) {};
};
}  // namespace vots
