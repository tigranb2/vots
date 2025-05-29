#pragma once

namespace vots {

class Order {
   public:
    const int ID_NUMBER;
    const bool IS_BUY;
    const int SHARES;
    const int ENTRY_TIME;
    const int EVENT_TIME;

    struct Builder {
        int id_number_;
        bool is_buy_;
        int shares_;
        int entry_time_;
        int event_time_;

        auto WithIdNumber(int id_number) -> Builder & {
            this->id_number_ = id_number;
            return *this;
        }

        auto WithIsBuy(bool is_buy) -> Builder & {
            this->is_buy_ = is_buy;
            return *this;
        }

        auto WithShares(int shares) -> Builder & {
            this->shares_ = shares;
            return *this;
        }

        auto WithEntryTime(int entry_time) -> Builder & {
            this->entry_time_ = entry_time;
            return *this;
        }

        auto WithEventTime(int event_time) -> Builder & {
            this->event_time_ = event_time;
            return *this;
        }

        auto Build() -> Order {
            return {this->id_number_, this->is_buy_, this->shares_, this->entry_time_, this->event_time_};
        }
    };

   private:
    Order(int id_number, bool is_buy, int shares, int entry_time, int event_time)
        : ID_NUMBER(id_number), IS_BUY(is_buy), SHARES(shares), ENTRY_TIME(entry_time), EVENT_TIME(event_time) {}
};

}  // namespace vots
