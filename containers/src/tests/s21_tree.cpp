#include <gtest/gtest.h>

#include "s21_containers_tests.h"

// Тест конструкторов
TEST(TreeTest, Constructors) {
  // Конструктор по умолчанию
  s21::tree<int> t1;
  EXPECT_TRUE(t1.empty());
  EXPECT_EQ(t1.size(), 0);

  // Конструктор со списком инициализации
  s21::tree<int> t2 = {50, 25, 75, 20, 30};
  EXPECT_FALSE(t2.empty());
  EXPECT_EQ(t2.size(), 5);

  // Конструктор копирования
  s21::tree<int> t3(t2);
  EXPECT_EQ(t3.size(), 5);
  auto it2 = t2.begin();
  auto it3 = t3.begin();
  while (it2 != t2.end()) {
    EXPECT_EQ(*it2, *it3);
    ++it2;
    ++it3;
  }

  // Конструктор перемещения
  s21::tree<int> t4(std::move(t2));
  EXPECT_EQ(t4.size(), 5);
  EXPECT_TRUE(t2.empty());  // t2 должен быть пуст после перемещения
}

// Тест вставки и порядка элементов
TEST(TreeTest, InsertAndCheckOrder) {
  s21::tree<int> t;
  t.insert(50);
  t.insert(25);
  t.insert(75);
  t.insert(20);
  t.insert(30);

  // Проверяем in-order обход
  std::vector<int> result;
  for (const auto& val : t) {
    result.push_back(val);
  }
  std::vector<int> expected = {20, 25, 30, 50, 75};
  EXPECT_EQ(result, expected);
}

// Тест запрета дубликатов (AllowDuplicates = false)
TEST(TreeTest, NoDuplicates) {
  s21::tree<int, false> t = {50, 25, 75};
  EXPECT_EQ(t.size(), 3);
  t.insert(25);            // Попытка вставить дубликат
  EXPECT_EQ(t.size(), 3);  // Размер не должен измениться
}

// Тест разрешения дубликатов (AllowDuplicates = true)
TEST(TreeTest, WithDuplicates) {
  s21::tree<int, true> t = {50, 25, 75};
  EXPECT_EQ(t.size(), 3);
  t.insert(25);  // Вставляем дубликат
  EXPECT_EQ(t.size(), 4);

  std::vector<int> result;
  for (const auto& val : t) {
    result.push_back(val);
  }
  // Дубликат вставляется в правое поддерево
  std::vector<int> expected = {25, 25, 50, 75};
  EXPECT_EQ(result, expected);
}

// Тест поиска элементов
TEST(TreeTest, Find) {
  s21::tree<int> t = {50, 25, 75, 20, 30};

  auto it_found = t.find(30);
  EXPECT_NE(it_found, t.end());
  EXPECT_EQ(*it_found, 30);

  auto it_not_found = t.find(99);
  EXPECT_EQ(it_not_found, t.end());
}

// --- Тесты для Erase ---

TEST(TreeTest, EraseLeafNode) {
  s21::tree<int> t = {50, 25, 75, 20};
  t.erase(t.find(20));  // Удаляем лист
  EXPECT_EQ(t.size(), 3);
  EXPECT_EQ(t.find(20), t.end());
  std::vector<int> expected = {25, 50, 75};
  std::vector<int> result;
  for (const auto& val : t) result.push_back(val);
  EXPECT_EQ(result, expected);
}

TEST(TreeTest, EraseNodeWithOneRightChild) {
  s21::tree<int> t = {50, 25, 75, 30};
  t.erase(t.find(25));  // Удаляем узел с одним правым потомком (30)
  EXPECT_EQ(t.size(), 3);
  EXPECT_EQ(t.find(25), t.end());
  std::vector<int> expected = {30, 50, 75};
  std::vector<int> result;
  for (const auto& val : t) result.push_back(val);
  EXPECT_EQ(result, expected);
}

TEST(TreeTest, EraseNodeWithOneLeftChild) {
  s21::tree<int> t = {50, 25, 75, 20};
  t.erase(t.find(25));  // Удаляем узел с одним левым потомком (20)
  EXPECT_EQ(t.size(), 3);
  EXPECT_EQ(t.find(25), t.end());
  std::vector<int> expected = {20, 50, 75};
  std::vector<int> result;
  for (const auto& val : t) result.push_back(val);
  EXPECT_EQ(result, expected);
}

TEST(TreeTest, EraseNodeWithTwoChildren) {
  s21::tree<int> t = {50, 25, 75, 20, 30, 60, 80};
  /* Дерево:
  /      50
  /     /  \
  /    25   75
  /   / \   / \
  /  20 30 60 80 */
  t.erase(t.find(25));  // Удаляем узел с двумя потомками
  EXPECT_EQ(t.size(), 6);
  EXPECT_EQ(t.find(25), t.end());
  // Successor для 25 это 30. Значение 30 скопируется на место 25, а старый узел
  // 30 удалится.
  std::vector<int> expected = {20, 30, 50, 60, 75, 80};
  std::vector<int> result;
  for (const auto& val : t) result.push_back(val);
  EXPECT_EQ(result, expected);
}

TEST(TreeTest, EraseRoot) {
  s21::tree<int> t = {50, 25, 75, 60, 80};
  /* Дерево:
  /      50
  /     /  \
  /    25   75
  /        /  \
  /       60  80 */
  t.erase(t.find(50));  // Удаляем корень
  EXPECT_EQ(t.size(), 4);
  EXPECT_EQ(t.find(50), t.end());
  // Successor для 50 это 60.
  std::vector<int> expected = {25, 60, 75, 80};
  std::vector<int> result;
  for (const auto& val : t) result.push_back(val);
  EXPECT_EQ(result, expected);
}

// Тест очистки и слияния
TEST(TreeTest, ClearAndMerge) {
  s21::tree<int> t1 = {10, 20};
  s21::tree<int> t2 = {15, 5};

  t1.merge(t2);
  EXPECT_EQ(t1.size(), 4);
  EXPECT_TRUE(t2.empty());
  std::vector<int> expected = {5, 10, 15, 20};
  std::vector<int> result;
  for (const auto& val : t1) result.push_back(val);
  EXPECT_EQ(result, expected);

  t1.clear();
  EXPECT_TRUE(t1.empty());
  EXPECT_EQ(t1.size(), 0);
}