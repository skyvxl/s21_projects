#include <queue>

#include "s21_containers_tests.h"

TEST(QueueTest, DefaultConstructor_Test1) {
  s21::queue<int> q;
  EXPECT_TRUE(q.empty());
  EXPECT_EQ(q.size(), 0);
}

TEST(QueueTest, DefaultConstructor_Test2) {
  s21::queue<double> queue;
  EXPECT_TRUE(queue.empty());
  EXPECT_EQ(queue.size(), 0);
}

TEST(QueueTest, DefaultConstructor_Test3) {
  s21::queue<s21::queue<int>> queue;
  EXPECT_TRUE(queue.empty());
  EXPECT_EQ(queue.size(), 0);
}

TEST(QueueTest, DefaultConstructor_Test11) {
  s21::queue<int> q;
  EXPECT_TRUE(q.empty());
  EXPECT_EQ(q.size(), 0);
}

// * constructor with initializer list

TEST(QueueTest, InitializerListConstructor_EmptyList) {
  s21::queue<int> q = {};
  EXPECT_TRUE(q.empty());
  EXPECT_EQ(q.size(), 0);
}

TEST(QueueTest, InitializerListConstructor_NonEmptyList) {
  s21::queue<int> q = {1, 2, 3, 4, 5};
  EXPECT_FALSE(q.empty());
  EXPECT_EQ(q.size(), 5);
  EXPECT_EQ(q.front(), 1);
  EXPECT_EQ(q.back(), 5);
}

// * copy constructor

TEST(QueueTest, CopyConstructor_EmptyQueue) {
  s21::queue<int> q1;
  s21::queue<int> q2(q1);
  EXPECT_TRUE(q2.empty());
  EXPECT_EQ(q2.size(), 0);
}

TEST(QueueTest, CopyConstructor_NonEmptyQueue) {
  s21::queue<int> q1 = {1, 2, 3};
  s21::queue<int> q2(q1);
  EXPECT_EQ(q2.size(), q1.size());
  EXPECT_EQ(q2.front(), 1);
  EXPECT_EQ(q2.back(), 3);
}

// * move constructor

TEST(QueueTest, MoveConstructor_EmptyQueue) {
  s21::queue<int> q1;
  s21::queue<int> q2(std::move(q1));
  EXPECT_TRUE(q2.empty());
  EXPECT_EQ(q2.size(), 0);
}

TEST(QueueTest, MoveConstructor_NonEmptyQueue) {
  s21::queue<int> q1 = {1, 2, 3};
  s21::queue<int> q2(std::move(q1));
  EXPECT_EQ(q2.size(), 3);
  EXPECT_TRUE(q1.empty());
}

// * copy assignment operator

TEST(QueueTest, CopyAssignmentOperator_EmptyToEmpty) {
  s21::queue<int> q1;
  s21::queue<int> q2;
  q2 = q1;
  EXPECT_TRUE(q2.empty());
  EXPECT_EQ(q2.size(), 0);
}

TEST(QueueTest, CopyAssignmentOperator_NonEmptyToEmpty) {
  s21::queue<int> q1 = {1, 2, 3};
  s21::queue<int> q2;
  q2 = q1;
  EXPECT_EQ(q2.size(), 3);
  EXPECT_EQ(q1.size(), 3);
  EXPECT_EQ(q2.front(), 1);
  EXPECT_EQ(q2.back(), 3);
}

// * move assignment operator

TEST(QueueTest, MoveAssignmentOperator_NonEmptyToEmpty) {
  s21::queue<int> q1 = {1, 2, 3};
  s21::queue<int> q2;
  q2 = std::move(q1);
  EXPECT_EQ(q2.size(), 3);
  EXPECT_TRUE(q1.empty());
}

// * element access - ИСПРАВЛЕННЫЕ ТЕСТЫ

TEST(QueueTest, Front_ValidQueue) {
  s21::queue<int> q = {1, 2, 3, 4, 5};
  EXPECT_EQ(q.front(), 1);
}

TEST(QueueTest, Front_EmptyQueue) {
  s21::queue<int> q;
  EXPECT_THROW(q.front(), std::out_of_range);
}

TEST(QueueTest, Back_ValidQueue) {
  s21::queue<int> q = {1, 2, 3, 4, 5};
  EXPECT_EQ(q.back(), 5);
}

TEST(QueueTest, Back_EmptyQueue) {
  s21::queue<int> q;
  EXPECT_THROW(q.back(), std::out_of_range);
}

TEST(QueueTest, FrontConst_ValidQueue) {
  const s21::queue<int> q = {1, 2, 3, 4, 5};
  EXPECT_EQ(q.front(), 1);
}

TEST(QueueTest, FrontConst_EmptyQueue) {
  const s21::queue<int> q;
  EXPECT_THROW(q.front(), std::out_of_range);
}

TEST(QueueTest, BackConst_ValidQueue) {
  const s21::queue<int> q = {1, 2, 3, 4, 5};
  EXPECT_EQ(q.back(), 5);
}

TEST(QueueTest, BackConst_EmptyQueue) {
  const s21::queue<int> q;
  EXPECT_THROW(q.back(), std::out_of_range);
}

// * capacity

TEST(QueueTest, Empty_EmptyQueue) {
  s21::queue<int> q;
  EXPECT_TRUE(q.empty());
}

TEST(QueueTest, Empty_NonEmptyQueue) {
  s21::queue<int> q = {1, 2, 3};
  EXPECT_FALSE(q.empty());
}

TEST(QueueTest, Size_EmptyQueue) {
  s21::queue<int> q;
  EXPECT_EQ(q.size(), 0);
}

TEST(QueueTest, Size_NonEmptyQueue) {
  s21::queue<int> q = {1, 2, 3};
  EXPECT_EQ(q.size(), 3);
}

// * modifiers - ИСПРАВЛЕННЫЕ ТЕСТЫ

TEST(QueueTest, Push_EmptyQueue) {
  s21::queue<int> q;
  q.push(1);
  EXPECT_FALSE(q.empty());
  EXPECT_EQ(q.size(), 1);
  EXPECT_EQ(q.front(), 1);
  EXPECT_EQ(q.back(), 1);
}

TEST(QueueTest, Pop_NonEmptyQueue) {
  s21::queue<int> q = {1, 2, 3};
  q.pop();
  EXPECT_EQ(q.size(), 2);
  EXPECT_EQ(q.front(), 2);
  EXPECT_EQ(q.back(), 3);
}

TEST(QueueTest, Pop_EmptyQueue) {
  s21::queue<int> q;
  EXPECT_THROW(q.pop(), std::out_of_range);
}

TEST(QueueTest, Pop_AllElements) {
  s21::queue<int> q = {1, 2, 3};
  q.pop();
  q.pop();
  q.pop();
  EXPECT_TRUE(q.empty());
  EXPECT_EQ(q.size(), 0);
  EXPECT_THROW(q.pop(), std::out_of_range);
}

TEST(QueueTest, Swap_Basic) {
  s21::queue<int> q1 = {1, 2, 3};
  s21::queue<int> q2 = {4, 5};
  q1.swap(q2);
  EXPECT_EQ(q1.size(), 2);
  EXPECT_EQ(q2.size(), 3);
}

// * emplace

TEST(QueueTest, Emplace_Back) {
  s21::queue<int> q = {1, 2, 3};
  q.push(4);
  EXPECT_EQ(q.size(), 4);
  EXPECT_EQ(q.back(), 4);
}

// * comparison operators

TEST(QueueTest, OperatorEqual_EqualQueues) {
  s21::queue<int> q1 = {1, 2, 3};
  s21::queue<int> q2 = {1, 2, 3};
  EXPECT_TRUE(q1 == q2);
}

TEST(QueueTest, OperatorEqual_DifferentSizes) {
  s21::queue<int> q1 = {1, 2, 3};
  s21::queue<int> q2 = {1, 2};
  EXPECT_FALSE(q1 == q2);
}

// * basic functionality test

TEST(QueueTest, BasicFIFO) {
  s21::queue<int> q;

  // Push elements
  for (int i = 1; i <= 5; ++i) {
    q.push(i);
    EXPECT_EQ(q.back(), i);
    EXPECT_EQ(q.front(), 1);  // First element should remain
  }

  EXPECT_EQ(q.size(), 5);

  // Pop elements in FIFO order
  for (int i = 1; i <= 5; ++i) {
    EXPECT_EQ(q.front(), i);
    q.pop();
  }

  EXPECT_TRUE(q.empty());
}

// * test with strings

TEST(QueueTest, StringQueue) {
  s21::queue<std::string> q;
  q.push("hello");
  q.push("world");

  EXPECT_EQ(q.front(), "hello");
  EXPECT_EQ(q.back(), "world");
  EXPECT_EQ(q.size(), 2);

  q.pop();
  EXPECT_EQ(q.front(), "world");
  EXPECT_EQ(q.size(), 1);
}

// * test copy semantics

TEST(QueueTest, DeepCopy) {
  s21::queue<int> q1 = {1, 2, 3};
  s21::queue<int> q2 = q1;  // copy constructor

  // Modify original
  q1.push(4);

  // Copy should not be affected
  EXPECT_EQ(q2.size(), 3);
  EXPECT_EQ(q2.front(), 1);
  EXPECT_EQ(q2.back(), 3);

  // Original should be modified
  EXPECT_EQ(q1.size(), 4);
  EXPECT_EQ(q1.back(), 4);
}

// * test move semantics

TEST(QueueTest, MoveSemantics) {
  s21::queue<int> q1 = {1, 2, 3};
  s21::queue<int> q2 = std::move(q1);

  // q1 should be empty after move
  EXPECT_TRUE(q1.empty());

  // q2 should have the elements
  EXPECT_EQ(q2.size(), 3);
  EXPECT_EQ(q2.front(), 1);
  EXPECT_EQ(q2.back(), 3);
}