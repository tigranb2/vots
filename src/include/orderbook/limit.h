#pragma once

#include <memory>

#include "orderbook/order.h"
#include "storage/list_node.h"

namespace vots {

class Limit {
   public:
    int limit_price_;
    int size_;
    int total_volume_;

    auto GetHeadOrder() -> std::unique_ptr<ListNode<int, Order>> &;
    auto GetTailOrder() -> std::unique_ptr<ListNode<int, Order>> &;

   private:
    std::unique_ptr<ListNode<int, Order>> *head_order_;
    std::unique_ptr<ListNode<int, Order>> *tail_order_;
};

}  // namespace vots
