#pragma once

#include <boost/unordered/unordered_flat_map.hpp>
#include <memory>
#include <vector>

namespace vots {

#define LL_TEMPLATE template <typename KeyType, typename DataType>
#define LL_TYPE LinkedList<KeyType, DataType>

/*
 * LinkedList implements a simple doubly linked-list and is used to store order objects.
 *
 * Each node contains an identifying key, though with no uniqueness guarantee, and data. The list is supported by a
 * hash-map for efficient look up and deletion on key
 */
LL_TEMPLATE class LinkedList {
   public:
    // Add adds the new node to the end of the tail node and reassigns the tail node
    void Add(KeyType key, DataType data);

    // Remove removes the LinkedList from the list, consequently deleting it.
    void Remove(KeyType key);

    auto GetHead() -> std::optional<std::pair<KeyType, DataType>> {
        if (this->head_.get() == nullptr) {
            return std::nullopt;
        }
        return std::make_pair(this->head_->key_, this->head_->data_);
    }

    auto GetTail() -> std::optional<std::pair<KeyType, DataType>> {
        if (this->tail_ == nullptr) {
            return std::nullopt;
        }
        return std::make_pair(this->tail_->key_, this->tail_->data_);
    }

    // ValidateList checks whether the list elements are in the order specified by the expected vector
    static auto ValidateList(const LinkedList &list, const std::vector<std::pair<KeyType, DataType>> &expected) -> bool;

   private:
    struct Node {
        KeyType key_;
        DataType data_;
        std::unique_ptr<Node> next_;
        Node *prev_;

        Node(KeyType key, DataType data) : key_(key), data_(std::move(data)), next_(nullptr), prev_(nullptr){};
    };

    std::unique_ptr<Node> head_;
    Node *tail_;
    boost::unordered_flat_map<KeyType, Node *> node_map_;
};
}  // namespace vots
