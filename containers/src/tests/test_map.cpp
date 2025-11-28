#include <gtest/gtest.h>

#include <map>
#include <vector>

#include "../inc/s21_map.h"

// Тесты для конструкторов
TEST(MapTest, ConstructorDefault) {
  s21::map<int, std::string> m;
  EXPECT_TRUE(m.empty());
  EXPECT_EQ(m.size(), 0);
}

TEST(MapTest, ConstructorInitializerList) {
  s21::map<int, std::string> m = {{1, "one"}, {2, "two"}, {3, "three"}};
  EXPECT_FALSE(m.empty());
  EXPECT_EQ(m.size(), 3);
  EXPECT_EQ(m.at(1), "one");
  EXPECT_EQ(m.at(2), "two");
  EXPECT_EQ(m.at(3), "three");
}

TEST(MapTest, ConstructorCopy) {
  s21::map<int, std::string> m1 = {{1, "one"}, {2, "two"}};
  s21::map<int, std::string> m2(m1);
  EXPECT_EQ(m1.size(), m2.size());
  EXPECT_EQ(m1.at(1), m2.at(1));
  EXPECT_EQ(m1.at(2), m2.at(2));
}

TEST(MapTest, ConstructorMove) {
  s21::map<int, std::string> m1 = {{1, "one"}, {2, "two"}};
  s21::map<int, std::string> m2(std::move(m1));
  EXPECT_TRUE(m1.empty());
  EXPECT_EQ(m2.size(), 2);
  EXPECT_EQ(m2.at(1), "one");
  EXPECT_EQ(m2.at(2), "two");
}

// Тесты для операторов присваивания
TEST(MapTest, OperatorAssignCopy) {
  s21::map<int, std::string> m1 = {{1, "one"}, {2, "two"}};
  s21::map<int, std::string> m2;
  m2 = m1;
  EXPECT_EQ(m1.size(), m2.size());
  EXPECT_EQ(m1.at(1), m2.at(1));
  EXPECT_EQ(m1.at(2), m2.at(2));
}

TEST(MapTest, OperatorAssignMove) {
  s21::map<int, std::string> m1 = {{1, "one"}, {2, "two"}};
  s21::map<int, std::string> m2;
  m2 = std::move(m1);
  EXPECT_TRUE(m1.empty());
  EXPECT_EQ(m2.size(), 2);
  EXPECT_EQ(m2.at(1), "one");
  EXPECT_EQ(m2.at(2), "two");
}

// Тесты для доступа к элементам
TEST(MapTest, AtExistingKey) {
  s21::map<int, std::string> m = {{1, "one"}, {2, "two"}};
  EXPECT_EQ(m.at(1), "one");
  EXPECT_EQ(m.at(2), "two");
}

TEST(MapTest, AtNonExistingKey) {
  s21::map<int, std::string> m = {{1, "one"}};
  EXPECT_THROW(m.at(2), std::out_of_range);
}

TEST(MapTest, OperatorBracketExistingKey) {
  s21::map<int, std::string> m = {{1, "one"}, {2, "two"}};
  EXPECT_EQ(m[1], "one");
  EXPECT_EQ(m[2], "two");
}

TEST(MapTest, OperatorBracketNewKey) {
  s21::map<int, std::string> m;
  m[1] = "one";
  EXPECT_EQ(m.size(), 1);
  EXPECT_EQ(m[1], "one");
}

// Тесты для итераторов
TEST(MapTest, IteratorBeginEnd) {
  s21::map<int, std::string> m = {{1, "one"}, {2, "two"}, {3, "three"}};
  auto it = m.begin();
  EXPECT_EQ((*it).first, 1);
  EXPECT_EQ((*it).second, "one");
  ++it;
  EXPECT_EQ((*it).first, 2);
  EXPECT_EQ((*it).second, "two");
  ++it;
  EXPECT_EQ((*it).first, 3);
  EXPECT_EQ((*it).second, "three");
  ++it;
  EXPECT_EQ(it, m.end());
}

TEST(MapTest, ConstIteratorBeginEnd) {
  const s21::map<int, std::string> m = {{1, "one"}, {2, "two"}};
  auto it = m.begin();
  EXPECT_EQ((*it).first, 1);
  EXPECT_EQ((*it).second, "one");
  ++it;
  EXPECT_EQ((*it).first, 2);
  EXPECT_EQ((*it).second, "two");
  ++it;
  EXPECT_EQ(it, m.end());
}

TEST(MapTest, IteratorDecrement) {
  s21::map<int, std::string> m = {{1, "one"}, {2, "two"}, {3, "three"}};
  auto it = m.end();
  --it;
  EXPECT_EQ((*it).first, 3);
  EXPECT_EQ((*it).second, "three");
  --it;
  EXPECT_EQ((*it).first, 2);
  EXPECT_EQ((*it).second, "two");
  --it;
  EXPECT_EQ((*it).first, 1);
  EXPECT_EQ((*it).second, "one");
}

// Тесты для методов состояния
TEST(MapTest, Empty) {
  s21::map<int, std::string> m;
  EXPECT_TRUE(m.empty());
  m[1] = "one";
  EXPECT_FALSE(m.empty());
}

TEST(MapTest, Size) {
  s21::map<int, std::string> m;
  EXPECT_EQ(m.size(), 0);
  m[1] = "one";
  EXPECT_EQ(m.size(), 1);
  m[2] = "two";
  EXPECT_EQ(m.size(), 2);
}

TEST(MapTest, MaxSize) {
  s21::map<int, std::string> m;
  EXPECT_GT(m.max_size(), 0);
}

// Тесты для модификации
TEST(MapTest, Clear) {
  s21::map<int, std::string> m = {{1, "one"}, {2, "two"}};
  EXPECT_FALSE(m.empty());
  m.clear();
  EXPECT_TRUE(m.empty());
  EXPECT_EQ(m.size(), 0);
}

TEST(MapTest, InsertValueType) {
  s21::map<int, std::string> m;
  auto result = m.insert({1, "one"});
  EXPECT_TRUE(result.second);
  EXPECT_EQ((*result.first).first, 1);
  EXPECT_EQ((*result.first).second, "one");
  EXPECT_EQ(m.size(), 1);

  // Попытка вставить дубликат
  result = m.insert({1, "duplicate"});
  EXPECT_FALSE(result.second);
  EXPECT_EQ(m.size(), 1);
  EXPECT_EQ(m[1], "one");  // Значение не изменилось
}

TEST(MapTest, InsertKeyValue) {
  s21::map<int, std::string> m;
  auto result = m.insert(1, "one");
  EXPECT_TRUE(result.second);
  EXPECT_EQ((*result.first).first, 1);
  EXPECT_EQ((*result.first).second, "one");
  EXPECT_EQ(m.size(), 1);
}

TEST(MapTest, InsertOrAssign) {
  s21::map<int, std::string> m = {{1, "one"}};
  auto result = m.insert_or_assign(1, "new_one");
  EXPECT_FALSE(result.second);  // Уже существовал
  EXPECT_EQ(m[1], "new_one");

  result = m.insert_or_assign(2, "two");
  EXPECT_TRUE(result.second);  // Новый
  EXPECT_EQ(m[2], "two");
}

TEST(MapTest, Erase) {
  s21::map<int, std::string> m = {{1, "one"}, {2, "two"}, {3, "three"}};
  auto it = m.find(2);
  m.erase(it);
  EXPECT_EQ(m.size(), 2);
  EXPECT_EQ(m.find(2), m.end());
  EXPECT_EQ(m[1], "one");
  EXPECT_EQ(m[3], "three");
}

TEST(MapTest, Swap) {
  s21::map<int, std::string> m1 = {{1, "one"}, {2, "two"}};
  s21::map<int, std::string> m2 = {{3, "three"}, {4, "four"}};
  m1.swap(m2);
  EXPECT_EQ(m1.size(), 2);
  EXPECT_EQ(m1[3], "three");
  EXPECT_EQ(m1[4], "four");
  EXPECT_EQ(m2.size(), 2);
  EXPECT_EQ(m2[1], "one");
  EXPECT_EQ(m2[2], "two");
}

TEST(MapTest, Merge) {
  s21::map<int, std::string> m1 = {{1, "one"}, {2, "two"}};
  s21::map<int, std::string> m2 = {{2, "duplicate"}, {3, "three"}};
  m1.merge(m2);
  EXPECT_EQ(m1.size(), 3);
  EXPECT_TRUE(m2.empty());
  EXPECT_EQ(m1[1], "one");
  EXPECT_EQ(m1[2], "two");  // Оригинальное значение сохранилось
  EXPECT_EQ(m1[3], "three");
}

// Тесты для поиска
TEST(MapTest, Contains) {
  s21::map<int, std::string> m = {{1, "one"}, {2, "two"}};
  EXPECT_TRUE(m.contains(1));
  EXPECT_TRUE(m.contains(2));
  EXPECT_FALSE(m.contains(3));
}

TEST(MapTest, Find) {
  s21::map<int, std::string> m = {{1, "one"}, {2, "two"}};
  auto it = m.find(1);
  EXPECT_NE(it, m.end());
  EXPECT_EQ((*it).first, 1);
  EXPECT_EQ((*it).second, "one");

  it = m.find(3);
  EXPECT_EQ(it, m.end());
}

TEST(MapTest, FindConst) {
  const s21::map<int, std::string> m = {{1, "one"}, {2, "two"}};
  auto it = m.find(1);
  EXPECT_NE(it, m.end());
  EXPECT_EQ((*it).first, 1);
  EXPECT_EQ((*it).second, "one");

  it = m.find(3);
  EXPECT_EQ(it, m.end());
}

// Тесты на сравнение с std::map
TEST(MapComparison, InsertAndSize) {
  s21::map<int, std::string> s21_m;
  std::map<int, std::string> std_m;

  s21_m.insert({1, "one"});
  std_m.insert({1, "one"});
  EXPECT_EQ(s21_m.size(), std_m.size());

  s21_m.insert({2, "two"});
  std_m.insert({2, "two"});
  EXPECT_EQ(s21_m.size(), std_m.size());
}

TEST(MapComparison, AccessElements) {
  s21::map<int, std::string> s21_m = {{1, "one"}, {2, "two"}};
  std::map<int, std::string> std_m = {{1, "one"}, {2, "two"}};

  EXPECT_EQ(s21_m[1], std_m[1]);
  EXPECT_EQ(s21_m[2], std_m[2]);
  EXPECT_EQ(s21_m.at(1), std_m.at(1));
  EXPECT_EQ(s21_m.at(2), std_m.at(2));
}

TEST(MapComparison, Iteration) {
  s21::map<int, std::string> s21_m = {{1, "one"}, {2, "two"}, {3, "three"}};
  std::map<int, std::string> std_m = {{1, "one"}, {2, "two"}, {3, "three"}};

  auto s21_it = s21_m.begin();
  auto std_it = std_m.begin();
  for (; s21_it != s21_m.end() && std_it != std_m.end(); ++s21_it, ++std_it) {
    EXPECT_EQ((*s21_it).first, (*std_it).first);
    EXPECT_EQ((*s21_it).second, (*std_it).second);
  }
  EXPECT_EQ(s21_it, s21_m.end());
  EXPECT_EQ(std_it, std_m.end());
}

TEST(MapComparison, EraseAndFind) {
  s21::map<int, std::string> s21_m = {{1, "one"}, {2, "two"}, {3, "three"}};
  std::map<int, std::string> std_m = {{1, "one"}, {2, "two"}, {3, "three"}};

  s21_m.erase(s21_m.find(2));
  std_m.erase(std_m.find(2));
  EXPECT_EQ(s21_m.size(), std_m.size());
  EXPECT_EQ(s21_m.find(2), s21_m.end());
  EXPECT_EQ(std_m.find(2), std_m.end());
}

TEST(MapComparison, ClearAndEmpty) {
  s21::map<int, std::string> s21_m = {{1, "one"}, {2, "two"}};
  std::map<int, std::string> std_m = {{1, "one"}, {2, "two"}};

  s21_m.clear();
  std_m.clear();
  EXPECT_TRUE(s21_m.empty());
  EXPECT_TRUE(std_m.empty());
  EXPECT_EQ(s21_m.size(), std_m.size());
}

TEST(MapComparison, Swap) {
  s21::map<int, std::string> s21_m1 = {{1, "one"}};
  s21::map<int, std::string> s21_m2 = {{2, "two"}};
  std::map<int, std::string> std_m1 = {{1, "one"}};
  std::map<int, std::string> std_m2 = {{2, "two"}};

  s21_m1.swap(s21_m2);
  std_m1.swap(std_m2);
  EXPECT_EQ(s21_m1[2], std_m1[2]);
  EXPECT_EQ(s21_m2[1], std_m2[1]);
}

TEST(MapComparison, Merge) {
  s21::map<int, std::string> s21_m1 = {{1, "one"}, {2, "two"}};
  s21::map<int, std::string> s21_m2 = {{3, "three"}, {4, "four"}};
  std::map<int, std::string> std_m1 = {{1, "one"}, {2, "two"}};
  std::map<int, std::string> std_m2 = {{3, "three"}, {4, "four"}};

  s21_m1.merge(s21_m2);
  std_m1.merge(std_m2);
  EXPECT_EQ(s21_m1.size(), std_m1.size());
  EXPECT_TRUE(s21_m2.empty());
  EXPECT_TRUE(std_m2.empty());
  EXPECT_EQ(s21_m1[1], std_m1[1]);
  EXPECT_EQ(s21_m1[2], std_m1[2]);
  EXPECT_EQ(s21_m1[3], std_m1[3]);
  EXPECT_EQ(s21_m1[4], std_m1[4]);
}

// Дополнительные тесты на краевые случаи
TEST(MapTest, LargeMap) {
  s21::map<int, int> m;
  for (int i = 0; i < 1000; ++i) {
    m[i] = i * 2;
  }
  EXPECT_EQ(m.size(), 1000);
  for (int i = 0; i < 1000; ++i) {
    EXPECT_EQ(m[i], i * 2);
  }
}

TEST(MapTest, IteratorEquality) {
  s21::map<int, std::string> m = {{1, "one"}, {2, "two"}};
  auto it1 = m.begin();
  auto it2 = m.begin();
  EXPECT_EQ(it1, it2);
  ++it1;
  EXPECT_NE(it1, it2);
}

TEST(MapTest, ConstIteratorFromIterator) {
  s21::map<int, std::string> m = {{1, "one"}};
  s21::map<int, std::string>::iterator it = m.begin();
  s21::map<int, std::string>::const_iterator cit(it);
  EXPECT_EQ((*cit).first, (*it).first);
  EXPECT_EQ((*cit).second, (*it).second);
}
