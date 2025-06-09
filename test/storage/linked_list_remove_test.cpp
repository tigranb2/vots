#include "gtest/gtest.h"
#include "storage/linked_list.h"

namespace vots {

TEST(LinkedListRemoveTest, SimpleRemove) {
    auto ll = LinkedList<int, int>();
    ll.Add(1, -1);
    ASSERT_EQ(ll.GetHead().value(), std::pair(1, -1));
    ASSERT_EQ(ll.GetTail().value(), std::pair(1, -1));

    ll.Remove(1);
    ASSERT_FALSE(ll.GetHead().has_value());
    ASSERT_FALSE(ll.GetTail().has_value());
}

TEST(LinkedListAddTest, IllegalRemove) {
    auto ll = LinkedList<int, int>();
    ASSERT_ANY_THROW(ll.Remove(23));

    ll.Add(1, 1);
    ASSERT_ANY_THROW(ll.Remove(2));

    ll.Remove(1);
    ASSERT_DEATH({ ll.Remove(1); }, "");
}

TEST(LinkedListRemoveTest, RemoveInsideList) {
    auto ll = LinkedList<int, int>();

    ll.Add(1, -1);
    ll.Add(2, -2);
    ll.Add(5, -5);
    ll.Add(3, -3);
    ASSERT_TRUE(ll.GetHead());
    ASSERT_TRUE(ll.GetTail());
    ASSERT_EQ(ll.GetHead().value(), std::pair(1, -1));
    ASSERT_EQ(ll.GetTail().value(), std::pair(3, -3));

    ll.Remove(2);
    ASSERT_TRUE(ll.GetHead());
    ASSERT_TRUE(ll.GetTail());
    ASSERT_EQ(ll.GetHead().value(), std::pair(1, -1));
    ASSERT_EQ(ll.GetTail().value(), std::pair(3, -3));

    ll.Remove(1);
    ASSERT_TRUE(ll.GetHead());
    ASSERT_TRUE(ll.GetTail());
    ASSERT_EQ(ll.GetHead().value(), std::pair(5, -5));
    ASSERT_EQ(ll.GetTail().value(), std::pair(3, -3));

    ll.Remove(3);
    ASSERT_TRUE(ll.GetHead());
    ASSERT_TRUE(ll.GetTail());
    ASSERT_EQ(ll.GetHead().value(), std::pair(5, -5));
    ASSERT_EQ(ll.GetTail().value(), std::pair(5, -5));

    ll.Remove(5);
    ASSERT_FALSE(ll.GetHead().has_value());
    ASSERT_FALSE(ll.GetTail().has_value());
}
}  // namespace vots
