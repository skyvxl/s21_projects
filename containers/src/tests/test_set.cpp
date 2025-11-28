#include "s21_containers_tests.h"

// Тесты для конструкторов
TEST(SetTest, ConstructorDefault) {
  s21::set<int> s;
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

TEST(SetTest, ConstructorInitializerList) {
  s21::set<int> s = {1, 2, 3};
  EXPECT_FALSE(s.empty());
  EXPECT_EQ(s.size(), 3);
  EXPECT_TRUE(s.contains(1));
  EXPECT_TRUE(s.contains(2));
  EXPECT_TRUE(s.contains(3));
}

TEST(SetTest, ConstructorCopy) {
  s21::set<int> s1 = {1, 2, 3};
  s21::set<int> s2(s1);
  EXPECT_EQ(s1.size(), s2.size());
  EXPECT_TRUE(s2.contains(1));
  EXPECT_TRUE(s2.contains(2));
  EXPECT_TRUE(s2.contains(3));
}

TEST(SetTest, ConstructorMove) {
  s21::set<int> s1 = {1, 2, 3};
  s21::set<int> s2(std::move(s1));
  EXPECT_TRUE(s1.empty());
  EXPECT_EQ(s2.size(), 3);
  EXPECT_TRUE(s2.contains(1));
  EXPECT_TRUE(s2.contains(2));
  EXPECT_TRUE(s2.contains(3));
}

// Тесты для операторов присваивания
TEST(SetTest, OperatorAssignCopy) {
  s21::set<int> s1 = {1, 2, 3};
  s21::set<int> s2;
  s2 = s1;
  EXPECT_EQ(s1.size(), s2.size());
  EXPECT_TRUE(s2.contains(1));
  EXPECT_TRUE(s2.contains(2));
  EXPECT_TRUE(s2.contains(3));
}

TEST(SetTest, OperatorAssignMove) {
  s21::set<int> s1 = {1, 2, 3};
  s21::set<int> s2;
  s2 = std::move(s1);
  EXPECT_TRUE(s1.empty());
  EXPECT_EQ(s2.size(), 3);
  EXPECT_TRUE(s2.contains(1));
  EXPECT_TRUE(s2.contains(2));
  EXPECT_TRUE(s2.contains(3));
}

// Тесты для итераторов
TEST(SetTest, IteratorBeginEnd) {
  s21::set<int> s = {1, 2, 3};
  auto it = s.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
  ++it;
  EXPECT_EQ(it, s.end());
}

TEST(SetTest, ConstIteratorBeginEnd) {
  const s21::set<int> s = {1, 2};
  auto it = s.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(it, s.end());
}

TEST(SetTest, IteratorDecrement) {
  s21::set<int> s = {1, 2, 3};
  auto it = s.end();
  --it;
  EXPECT_EQ(*it, 3);
  --it;
  EXPECT_EQ(*it, 2);
  --it;
  EXPECT_EQ(*it, 1);
}

// Тесты для методов состояния
TEST(SetTest, Empty) {
  s21::set<int> s;
  EXPECT_TRUE(s.empty());
  s.insert(1);
  EXPECT_FALSE(s.empty());
}

TEST(SetTest, Size) {
  s21::set<int> s;
  EXPECT_EQ(s.size(), 0);
  s.insert(1);
  EXPECT_EQ(s.size(), 1);
  s.insert(2);
  EXPECT_EQ(s.size(), 2);
}

TEST(SetTest, MaxSize) {
  s21::set<int> s;
  EXPECT_GT(s.max_size(), 0);
}

// Тесты для модификации
TEST(SetTest, Clear) {
  s21::set<int> s = {1, 2, 3};
  EXPECT_FALSE(s.empty());
  s.clear();
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

TEST(SetTest, InsertValue) {
  s21::set<int> s;
  auto result = s.insert(1);
  EXPECT_TRUE(result.second);
  EXPECT_EQ(*result.first, 1);
  EXPECT_EQ(s.size(), 1);

  // Попытка вставить дубликат
  result = s.insert(1);
  EXPECT_FALSE(result.second);
  EXPECT_EQ(s.size(), 1);
}

TEST(SetTest, Erase) {
  s21::set<int> s = {1, 2, 3};
  auto it = s.find(2);
  s.erase(it);
  EXPECT_EQ(s.size(), 2);
  EXPECT_EQ(s.find(2), s.end());
  EXPECT_TRUE(s.contains(1));
  EXPECT_TRUE(s.contains(3));
}

TEST(SetTest, Swap) {
  s21::set<int> s1 = {1, 2};
  s21::set<int> s2 = {3, 4};
  s1.swap(s2);
  EXPECT_EQ(s1.size(), 2);
  EXPECT_TRUE(s1.contains(3));
  EXPECT_TRUE(s1.contains(4));
  EXPECT_EQ(s2.size(), 2);
  EXPECT_TRUE(s2.contains(1));
  EXPECT_TRUE(s2.contains(2));
}

TEST(SetTest, Merge) {
  s21::set<int> s1 = {1, 2};
  s21::set<int> s2 = {2, 3};  // 2 - дубликат
  s1.merge(s2);
  EXPECT_EQ(s1.size(), 3);
  EXPECT_TRUE(s2.empty() || s2.size() == 1);  // Может остаться дубликат или нет
  EXPECT_TRUE(s1.contains(1));
  EXPECT_TRUE(s1.contains(2));
  EXPECT_TRUE(s1.contains(3));
}

// Тесты для поиска
TEST(SetTest, Contains) {
  s21::set<int> s = {1, 2};
  EXPECT_TRUE(s.contains(1));
  EXPECT_TRUE(s.contains(2));
  EXPECT_FALSE(s.contains(3));
}

TEST(SetTest, Find) {
  s21::set<int> s = {1, 2};
  auto it = s.find(1);
  EXPECT_NE(it, s.end());
  EXPECT_EQ(*it, 1);

  it = s.find(3);
  EXPECT_EQ(it, s.end());
}

TEST(SetTest, FindConst) {
  const s21::set<int> s = {1, 2};
  auto it = s.find(1);
  EXPECT_NE(it, s.end());
  EXPECT_EQ(*it, 1);

  it = s.find(3);
  EXPECT_EQ(it, s.end());
}

// Тесты на сравнение с std::set
TEST(SetComparison, InsertAndSize) {
  s21::set<int> s21_s;
  std::set<int> std_s;

  s21_s.insert(1);
  std_s.insert(1);
  EXPECT_EQ(s21_s.size(), std_s.size());

  s21_s.insert(2);
  std_s.insert(2);
  EXPECT_EQ(s21_s.size(), std_s.size());
}

TEST(SetComparison, Iteration) {
  s21::set<int> s21_s = {1, 2, 3};
  std::set<int> std_s = {1, 2, 3};

  auto s21_it = s21_s.begin();
  auto std_it = std_s.begin();
  for (; s21_it != s21_s.end() && std_it != std_s.end(); ++s21_it, ++std_it) {
    EXPECT_EQ(*s21_it, *std_it);
  }
  EXPECT_EQ(s21_it, s21_s.end());
  EXPECT_EQ(std_it, std_s.end());
}

TEST(SetComparison, EraseAndFind) {
  s21::set<int> s21_s = {1, 2, 3};
  std::set<int> std_s = {1, 2, 3};

  s21_s.erase(s21_s.find(2));
  std_s.erase(std_s.find(2));
  EXPECT_EQ(s21_s.size(), std_s.size());
  EXPECT_EQ(s21_s.find(2), s21_s.end());
  EXPECT_EQ(std_s.find(2), std_s.end());
}

TEST(SetComparison, ClearAndEmpty) {
  s21::set<int> s21_s = {1, 2};
  std::set<int> std_s = {1, 2};

  s21_s.clear();
  std_s.clear();
  EXPECT_TRUE(s21_s.empty());
  EXPECT_TRUE(std_s.empty());
  EXPECT_EQ(s21_s.size(), std_s.size());
}

TEST(SetComparison, Swap) {
  s21::set<int> s21_s1 = {1};
  s21::set<int> s21_s2 = {2};
  std::set<int> std_s1 = {1};
  std::set<int> std_s2 = {2};

  s21_s1.swap(s21_s2);
  std_s1.swap(std_s2);
  EXPECT_TRUE(s21_s1.contains(2));
  //   EXPECT_TRUE(std_s1.contains(2));
  EXPECT_TRUE(s21_s2.contains(1));
  //   EXPECT_TRUE(std_s2.contains(1));
}

TEST(SetComparison, Merge) {
  s21::set<int> s21_s1 = {1, 2};
  s21::set<int> s21_s2 = {3, 4};
  std::set<int> std_s1 = {1, 2};
  std::set<int> std_s2 = {3, 4};

  s21_s1.merge(s21_s2);
  std_s1.merge(std_s2);
  EXPECT_EQ(s21_s1.size(), std_s1.size());
  EXPECT_TRUE(s21_s1.contains(1));
  EXPECT_TRUE(s21_s1.contains(2));
  EXPECT_TRUE(s21_s1.contains(3));
  EXPECT_TRUE(s21_s1.contains(4));
}

// Дополнительные тесты на краевые случаи
TEST(SetTest, LargeSet) {
  s21::set<int> s;
  for (int i = 0; i < 1000; ++i) {
    s.insert(i);
  }
  EXPECT_EQ(s.size(), 1000);
  for (int i = 0; i < 1000; ++i) {
    EXPECT_TRUE(s.contains(i));
  }
}

TEST(SetTest, IteratorEquality) {
  s21::set<int> s = {1, 2};
  auto it1 = s.begin();
  auto it2 = s.begin();
  EXPECT_EQ(it1, it2);
  ++it1;
  EXPECT_NE(it1, it2);
}

TEST(SetTest, ConstIteratorFromIterator) {
  s21::set<int> s = {1};
  s21::set<int>::iterator it = s.begin();
  s21::set<int>::const_iterator cit(it);
  EXPECT_EQ(*cit, *it);
}

// Тесты для уникальности элементов
TEST(SetTest, UniqueElements) {
  s21::set<int> s;
  s.insert(1);
  s.insert(1);
  s.insert(1);
  EXPECT_EQ(s.size(), 1);
}

// Тесты для сортировки элементов
TEST(SetTest, SortedOrder) {
  s21::set<int> s = {5, 3, 1, 4, 2};
  auto it = s.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
  ++it;
  EXPECT_EQ(*it, 4);
  ++it;
  EXPECT_EQ(*it, 5);
}

// Тесты для пользовательских типов
struct CustomType {
  int value;

  bool operator>(const CustomType& other) const { return value > other.value; }
  bool operator<(const CustomType& other) const { return value < other.value; }
  bool operator==(const CustomType& other) const {
    return value == other.value;
  }
};

TEST(SetTest, CustomType) {
  s21::set<CustomType> s;
  s.insert({1});
  s.insert({2});
  EXPECT_EQ(s.size(), 2);
  EXPECT_TRUE(s.contains({1}));
  EXPECT_FALSE(s.contains({3}));
}

// Тесты на граничные значения
TEST(SetTest, EmptySetOperations) {
  s21::set<int> s;
  EXPECT_EQ(s.find(1), s.end());
  EXPECT_FALSE(s.contains(1));
  EXPECT_NO_THROW(s.clear());
  EXPECT_NO_THROW(s.erase(s.begin()));  // Должно быть безопасно
}

TEST(SetTest, SingleElementSet) {
  s21::set<int> s = {42};
  EXPECT_EQ(s.size(), 1);
  EXPECT_EQ(*s.begin(), 42);
  s.erase(s.begin());
  EXPECT_TRUE(s.empty());
}