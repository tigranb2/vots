// tests/list_node_owner_test.cpp

#include <gtest/gtest.h>

#include "storage/list_node.h"

using std::pair;
using std::unique_ptr;
using std::vector;
using vots::ListNode;

TEST(ListNodeGetOwnerTest, GetOwnerHead) {
    auto head = ListNode<int, int>::Make(1, 100);
    EXPECT_EQ((ListNode<int, int>::GetOwner(head.get())), nullptr);
}

TEST(ListNodeGetOwnerTest, IllegalGetOwner) {
    auto node = ListNode<int, int>::Make(1, 100);
    auto node_ptr = node.release();
    delete (node_ptr);
    ASSERT_DEATH((ListNode<int, int>::GetOwner(node.get())), "");
    ASSERT_DEATH((ListNode<int, int>::GetOwner(nullptr)), "");
}

TEST(ListNodeGetOwnerTest, ValidGetOwner) {
    auto head = ListNode<int, int>::Make(1, 100);
    head->Add(2, 200);
    head->next_->Add(3, 300);

    ListNode<int, int> *middle = head->next_.get();

    unique_ptr<ListNode<int, int>> *owner_ptr = ListNode<int, int>::GetOwner(middle);
    ASSERT_NE(owner_ptr, nullptr);

    EXPECT_EQ(owner_ptr->get(), middle);

    (*owner_ptr)->Remove();
    vector<pair<int, int>> expected = {{1, 100}, {3, 300}};
    ASSERT_TRUE((ListNode<int, int>::ValidateList(head, expected)));
}

TEST(ListNodeGetOwnerTest, ResettingTailOwner) {
    auto head = ListNode<int, int>::Make(1, 100);
    head->Add(2, 200);
    head->next_->Add(3, 300);

    ListNode<int, int> *tail = head->next_->next_.get();

    auto owner_ptr = ListNode<int, int>::GetOwner(tail);
    ASSERT_NE(owner_ptr, nullptr);

    owner_ptr->reset();

    vector<pair<int, int>> expected = {{1, 100}, {2, 200}};
    ASSERT_TRUE((ListNode<int, int>::ValidateList(head, expected)));
}
