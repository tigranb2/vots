#pragma once

#include <boost/unordered/unordered_flat_map.hpp>
#include <memory>

#include "limit.h"
#include "order.h"

namespace vots {

struct OrderBook {
   public:
    void Add(bool is_buy, int shares);
    void Cancel();
    void Execute();

   private:
    std::unique_ptr<Limit> buy_limits_;
    std::unique_ptr<Limit> sell_limits_;

    boost::unordered_flat_map<int, Limit> limit_map_;  // key: limit price
    boost::unordered_flat_map<int, Order> order_map_;  // key: order ID
};

}  // namespace vots
