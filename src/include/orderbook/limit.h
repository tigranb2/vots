#pragma once

#include <memory>

#include "order.h"
#include "storage/list_node.h"

namespace vots {

class Limit {
   public:
    int limit_price_;
    int size_;
    int total_volume_;

    auto GetHeadOrder() -> std::unique_ptr<ListNode<int, Order>> & { return *head_order_; }
    auto GetTailOrder() -> std::unique_ptr<ListNode<int, Order>> & { return *tail_order_; }

   private:
    std::unique_ptr<ListNode<int, Order>> *head_order_;
    std::unique_ptr<ListNode<int, Order>> *tail_order_;
};

}  // namespace vots
