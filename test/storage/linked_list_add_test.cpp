#include "gtest/gtest.h"
#include "storage/linked_list.h"

namespace vots {

TEST(LinkedListAddTest, SimpleAdd) {
    auto ll = LinkedList<int, int>();

    auto expected = std::vector<std::pair<int, int>>{};
    ASSERT_TRUE((LinkedList<int, int>::ValidateList(ll, expected)));

    ASSERT_FALSE(ll.GetHead().has_value());
    ASSERT_FALSE(ll.GetTail().has_value());

    ll.Add(2, -2);
    expected.emplace_back(2, -2);
    ASSERT_TRUE((LinkedList<int, int>::ValidateList(ll, expected)));

    ASSERT_EQ(ll.GetHead().value(), std::pair(2, -2));
    ASSERT_EQ(ll.GetTail().value(), std::pair(2, -2));

    ll.Add(3, -3);
    expected.emplace_back(3, -3);
    ASSERT_TRUE((LinkedList<int, int>::ValidateList(ll, expected)));

    ASSERT_EQ(ll.GetHead().value(), std::pair(2, -2));
    ASSERT_EQ(ll.GetTail().value(), std::pair(3, -3));

    ll.Add(-12, 12);
    expected.emplace_back(-12, 12);
    ASSERT_TRUE((LinkedList<int, int>::ValidateList(ll, expected)));

    ASSERT_EQ(ll.GetHead().value(), std::pair(2, -2));
    ASSERT_EQ(ll.GetTail().value(), std::pair(-12, 12));
}

}  // namespace vots
