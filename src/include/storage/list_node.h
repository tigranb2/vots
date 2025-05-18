#pragma once

#include <memory>
#include <vector>

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
    std::unique_ptr<ListNode> next_;
    ListNode *prev_;

    // Make constructs a ListNode object, wrapped in an std::unique_ptr
    static auto Make(KeyType key, DataType data) -> std::unique_ptr<ListNode>;

    // Add adds the new node to the end of the provide prev node. It returns a reference to the new node.
    //
    // Neither the provided prev nor the new_node can have an underlying nullptr.
    auto Add(std::unique_ptr<ListNode> &new_node) -> std::unique_ptr<ListNode> &;

    auto Add(KeyType key, DataType data) -> std::unique_ptr<ListNode> &;

    // Delete removes the ListNode from the list, consequently deleting it. If the provided node is the head node, its
    // owner is not in the list, and thus the node is removed from the list but not destructed.
    void Delete();

    // GetOwner returns a pointer to the unique_ptr owning the provided ListNode pointer. If the head node is provided
    // the owner is not part of the list and thus cannot be determined, so a nullptr is returned instead.
    static auto GetOwner(ListNode *node) -> std::unique_ptr<ListNode> *;

    // ValidateList checks whether the list elements are in the order specified by the expected vector
    static auto ValidateList(const std::unique_ptr<ListNode> &head,
                             const std::vector<std::pair<KeyType, DataType>> &expected) -> bool;

   private:
    ListNode(KeyType key, DataType data) : key_(key), data_(std::move(data)) {};
};
}  // namespace vots
