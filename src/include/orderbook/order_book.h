#pragma once

#include <boost/unordered/unordered_flat_map.hpp>
#include <memory>

#include "limit.h"
#include "order.h"
#include "storage/red_black_tree.h"

namespace vots {

class OrderBook {
   public:
    OrderBook() : buy_limits_(new RedBlackTree<int, Limit>()), sell_limits_(new RedBlackTree<int, Limit>()) {};

    auto Add(bool is_buy, int shares) -> int;
    void Cancel(int order_id);
    void Execute(int order_id);

   private:
    std::unique_ptr<RedBlackTree<int, Limit>> buy_limits_;
    std::unique_ptr<RedBlackTree<int, Limit>> sell_limits_;

    boost::unordered_flat_map<int, Limit> limit_map_;  // key: limit price
    boost::unordered_flat_map<int, Order> order_map_;  // key: order ID
};

}  // namespace vots
