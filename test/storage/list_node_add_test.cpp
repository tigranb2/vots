#include "gtest/gtest.h"
#include "storage/list_node.h"

namespace vots {

TEST(ListNodeAddTest, SimpleAdd) {
    auto node1 = ListNode<int, int>::Make(1, -1);
    auto expected = std::vector<std::pair<int, int>>{{1, -1}};
    ASSERT_TRUE((ListNode<int, int>::ValidateList(node1, expected)));

    node1->Add(2, -2);
    expected.emplace_back(2, -2);
    ASSERT_TRUE((ListNode<int, int>::ValidateList(node1, expected)));

    auto node3 = ListNode<int, int>::Make(3, -3);
    node1->next_->Add(node3);
    expected.emplace_back(3, -3);
    ASSERT_TRUE((ListNode<int, int>::ValidateList(node1, expected)));

    node1->next_->next_->Add(12, -12);
    expected.emplace_back(12, -12);
    ASSERT_TRUE((ListNode<int, int>::ValidateList(node1, expected)));
}

TEST(ListNodeAddTest, IllegalAdd) {
    auto empty_node = ListNode<int, int>::Make(1, -1);
    auto note_ptr = empty_node.release();
    delete (note_ptr);

    ASSERT_DEATH(empty_node->Add(2, -2), "");

    auto node1 = ListNode<int, int>::Make(1, -1);
    ASSERT_DEATH(node1->Add(empty_node), "");

    node1->Add(node1);
    ASSERT_TRUE(node1.get() == nullptr);
}

TEST(ListNodeAddTest, AddingInsideList) {
    auto expected = std::vector<std::pair<int, int>>{{1, -1}, {7, -7}, {2, -2}, {3, -3}};

    auto node1 = ListNode<int, int>::Make(1, -1);
    node1->Add(7, -7);
    ASSERT_TRUE((ListNode<int, int>::ValidateList(node1, {{1, -1}, {7, -7}})));

    auto node2 = ListNode<int, int>::Make(2, -2);
    node2->Add(3, -3);
    ASSERT_TRUE((ListNode<int, int>::ValidateList(node2, {{2, -2}, {3, -3}})));

    node1->next_->Add(node2);  // adding list of length 2 to another list of length 2
    ASSERT_TRUE((ListNode<int, int>::ValidateList(node1, expected)));

    node1->Add(-50, 23);  // adding node in the middle of a list
    ASSERT_TRUE((ListNode<int, int>::ValidateList(node1, {{1, -1}, {-50, 23}})));
}

}  // namespace vots
