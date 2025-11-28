#include <gtest/gtest.h>

#include <stack>  // Для сравнения с эталонной реализацией
#include <string>

#include "s21_containers_tests.h"

// Тестовый набор для конструкторов и операторов присваивания
class StackConstructorsTest : public ::testing::Test {};

TEST_F(StackConstructorsTest, DefaultConstructor) {
  s21::stack<int> s;
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

TEST_F(StackConstructorsTest, InitializerListConstructor) {
  s21::stack<int> s = {1, 2, 3, 4};
  EXPECT_EQ(s.size(), 4);
  EXPECT_EQ(s.top(), 4);  // Последний элемент в списке - вершина стека
  s.pop();
  EXPECT_EQ(s.top(), 3);
}

TEST_F(StackConstructorsTest, CopyConstructor) {
  s21::stack<int> s1 = {1, 2, 3};
  s21::stack<int> s2(s1);

  EXPECT_EQ(s1.size(), s2.size());
  EXPECT_EQ(s1.top(), s2.top());

  // Проверка на глубокое копирование
  s2.push(4);
  EXPECT_NE(s1.size(), s2.size());
  EXPECT_EQ(s1.top(), 3);
  EXPECT_EQ(s2.top(), 4);
}

TEST_F(StackConstructorsTest, MoveConstructor) {
  s21::stack<int> s1 = {1, 2, 3};
  s21::stack<int> s2(std::move(s1));

  EXPECT_TRUE(s1.empty());
  EXPECT_EQ(s2.size(), 3);
  EXPECT_EQ(s2.top(), 3);
}

TEST_F(StackConstructorsTest, CopyAssignmentOperator) {
  s21::stack<int> s1 = {1, 2};
  s21::stack<int> s2 = {3, 4, 5};
  s2 = s1;
  EXPECT_EQ(s2.size(), 2);
  EXPECT_EQ(s2.top(), 2);
  s2.pop();
  EXPECT_EQ(s2.top(), 1);
}

TEST_F(StackConstructorsTest, MoveAssignmentOperator) {
  s21::stack<int> s1 = {1, 2};
  s21::stack<int> s2 = {3, 4, 5};
  s2 = std::move(s1);
  EXPECT_TRUE(s1.empty());
  EXPECT_EQ(s2.size(), 2);
  EXPECT_EQ(s2.top(), 2);
}

// Тестовый набор для основной логики LIFO
class StackLogicTest : public ::testing::Test {};

TEST_F(StackLogicTest, PushTopPop) {
  s21::stack<std::string> s;
  EXPECT_TRUE(s.empty());

  s.push("hello");
  EXPECT_EQ(s.top(), "hello");
  EXPECT_EQ(s.size(), 1);
  EXPECT_FALSE(s.empty());

  s.push("world");
  EXPECT_EQ(s.top(), "world");
  EXPECT_EQ(s.size(), 2);

  s.push("!");
  EXPECT_EQ(s.top(), "!");
  EXPECT_EQ(s.size(), 3);

  s.pop();
  EXPECT_EQ(s.top(), "world");
  EXPECT_EQ(s.size(), 2);

  s.pop();
  EXPECT_EQ(s.top(), "hello");
  EXPECT_EQ(s.size(), 1);

  s.pop();
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

TEST_F(StackLogicTest, ConstTop) {
  s21::stack<int> s_mut = {10, 20};
  const s21::stack<int> s_const(s_mut);

  EXPECT_EQ(s_const.top(), 20);
  EXPECT_EQ(s_const.size(), 2);
  EXPECT_FALSE(s_const.empty());
}

TEST_F(StackLogicTest, Swap) {
  s21::stack<int> s1 = {1, 2};
  s21::stack<int> s2 = {3, 4, 5};

  s1.swap(s2);

  EXPECT_EQ(s1.size(), 3);
  EXPECT_EQ(s1.top(), 5);
  EXPECT_EQ(s2.size(), 2);
  EXPECT_EQ(s2.top(), 2);
}

// Тесты для пограничных случаев
class StackEdgeCasesTest : public ::testing::Test {};

TEST_F(StackEdgeCasesTest, TopOnEmpty) {
  s21::stack<int> s;
  // list::back() должен бросать исключение на пустом списке
  EXPECT_THROW(s.top(), std::out_of_range);
}

TEST_F(StackEdgeCasesTest, PopOnEmpty) {
  s21::stack<int> s;
  // list::pop_back() должен бросать исключение на пустом списке
  EXPECT_THROW(s.pop(), std::out_of_range);
}

// Сравнение поведения с std::stack
TEST(StackComparison, FullCycle) {
  s21::stack<int> s21_s;
  std::stack<int> std_s;

  // Push phase
  for (int i = 0; i < 100; ++i) {
    s21_s.push(i);
    std_s.push(i);
    ASSERT_EQ(s21_s.size(), std_s.size());
    ASSERT_EQ(s21_s.top(), std_s.top());
  }

  ASSERT_FALSE(s21_s.empty());
  ASSERT_FALSE(std_s.empty());

  // Pop phase
  for (int i = 0; i < 100; ++i) {
    ASSERT_EQ(s21_s.top(), std_s.top());
    s21_s.pop();
    std_s.pop();
    ASSERT_EQ(s21_s.size(), std_s.size());
  }

  ASSERT_TRUE(s21_s.empty());
  ASSERT_TRUE(std_s.empty());
}
