#pragma once

namespace vots {

struct Order {
    int id_number_;
    bool is_buy_;
    int shares_;
    int entry_time_;
    int event_time_;

    Order(int id_number, bool is_buy, int shares, int entry_time, int event_time)
        : id_number_(id_number), is_buy_(is_buy), shares_(shares), entry_time_(entry_time), event_time_(event_time) {}
};

}  // namespace vots
