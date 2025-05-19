#include "gtest/gtest.h"
#include "storage/list_node.h"

namespace vots {

TEST(ListNodeRemoveTest, SimpleRemove) {
    auto node1 = ListNode<int, int>::Make(1, 100);
    node1->Add(2, 200);
    node1->next_->Add(3, 300);

    node1->next_->next_->Remove();

    std::vector<std::pair<int, int>> expected = {{1, 100}, {2, 200}};
    ASSERT_TRUE((ListNode<int, int>::ValidateList(node1, expected)));
}

TEST(ListNodeAddTest, IllegalRemove) {
    auto empty_node = ListNode<int, int>::Make(1, -1);
    auto note_ptr = empty_node.release();
    delete (note_ptr);

    ASSERT_DEATH(empty_node->Remove(), "");
}

TEST(ListNodeRemoveTest, RemoveInsideList) {
    auto node1 = ListNode<int, int>::Make(1, 100);
    node1->Add(2, 200);
    node1->next_->Add(3, 300);

    auto middle = node1->next_.get();
    middle->Remove();

    std::vector<std::pair<int, int>> expected = {{1, 100}, {3, 300}};
    ASSERT_TRUE((ListNode<int, int>::ValidateList(node1, expected)));

    auto &end = node1->next_;
    node1->Remove();
    ASSERT_EQ(end->prev_, nullptr);

    auto random_del_node = ListNode<int, int>::Make(1, 10);
    random_del_node->Add(2, 20);
    random_del_node->next_->Add(3, 30);
    random_del_node->next_->next_->Add(4, 40);
    random_del_node->next_->next_->next_->Add(5, 50);

    auto tail = random_del_node->next_->next_->next_->next_.get();
    tail->Remove();
    std::vector<std::pair<int, int>> after_tail = {{1, 10}, {2, 20}, {3, 30}, {4, 40}};
    ASSERT_TRUE((ListNode<int, int>::ValidateList(random_del_node, after_tail)));

    auto node3 = random_del_node->next_->next_.get();
    node3->Remove();
    std::vector<std::pair<int, int>> after_mid = {{1, 10}, {2, 20}, {4, 40}};
    ASSERT_TRUE((ListNode<int, int>::ValidateList(random_del_node, after_mid)));

    auto node2 = random_del_node->next_.get();
    node2->Remove();
    std::vector<std::pair<int, int>> after_second = {{1, 10}, {4, 40}};
    ASSERT_TRUE((ListNode<int, int>::ValidateList(random_del_node, after_second)));
}
}  // namespace vots
