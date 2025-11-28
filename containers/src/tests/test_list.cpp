#include <gtest/gtest.h>

#include <iostream>
#include <list>

#include "s21_containers_tests.h"

// Тест на конструктор по умолчанию
TEST(ListTest, DefaultConstructor) {
  s21::list<int> l;
  EXPECT_EQ(l.size(), 0);
  EXPECT_TRUE(l.empty());
}

// Тест на конструктор перемещения
TEST(ListTest, MoveConstructor) {
  s21::list<int> l1;
  s21::list<int> l2 = std::move(l1);  // l2 будет типа int, все верно

  EXPECT_EQ(l2.size(), 0);
  EXPECT_TRUE(l2.empty());
  EXPECT_EQ(l1.size(), 0);
  EXPECT_TRUE(l1.empty());
}

// Тест на перемещение
TEST(ListTest, MoveAssignment) {
  s21::list<int> l1;
  s21::list<int> l2;

  l2 = std::move(l1);

  EXPECT_EQ(l2.size(), 0);
  EXPECT_TRUE(l2.empty());
  EXPECT_EQ(l1.size(), 0);
  EXPECT_TRUE(l1.empty());
}

TEST(ListTest, SizeConstructor) {
  // Тест с ненулевым размером
  s21::list<int> l(5);
  EXPECT_EQ(l.size(), 5);
  EXPECT_FALSE(l.empty());
  // Проверяем, что все элементы инициализированы нулем (по умолчанию для int)
  for (const auto& item : l) {
    EXPECT_EQ(item, 0);
  }

  // Тест с нулевым размером
  s21::list<double> l_empty(0);
  EXPECT_EQ(l_empty.size(), 0);
  EXPECT_TRUE(l_empty.empty());
}

// Тест конструктора со списком инициализации (initializer_list)
TEST(ListTest, InitializerListConstructor) {
  // Тест с непустым списком
  s21::list<int> l = {10, 20, 30, 40, 50};
  std::list<int> std_l = {10, 20, 30, 40, 50};

  EXPECT_EQ(l.size(), 5);
  EXPECT_EQ(l.front(), 10);
  EXPECT_EQ(l.back(), 50);

  // Сравниваем каждый элемент с эталоном
  auto s21_it = l.begin();
  auto std_it = std_l.begin();
  while (s21_it != l.end()) {
    EXPECT_EQ(*s21_it, *std_it);
    ++s21_it;
    ++std_it;
  }

  // Тест с пустым списком
  s21::list<int> l_empty = {};
  EXPECT_TRUE(l_empty.empty());
}

// Тест конструктора копирования (list(const list& l))
TEST(ListTest, CopyConstructor) {
  // 1. Создаем оригинальный список
  s21::list<int> l1 = {1, 2, 3};

  // 2. Создаем копию
  s21::list<int> l2(l1);

  // 3. Проверяем, что копия идентична оригиналу
  EXPECT_EQ(l1.size(), l2.size());
  auto it1 = l1.begin();
  auto it2 = l2.begin();
  while (it1 != l1.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1;
    ++it2;
  }

  // 4. ВАЖНАЯ ПРОВЕРКА: Глубокое копирование
  //    Изменяем копию и проверяем, что оригинал НЕ изменился.
  //    Это доказывает, что мы скопировали узлы, а не просто указатели.
  l2.push_back(4);
  EXPECT_NE(l1.size(), l2.size());
  EXPECT_EQ(l1.back(), 3);
  EXPECT_EQ(l2.back(), 4);

  // 5. Тест копирования пустого списка
  s21::list<int> l_empty_orig;
  s21::list<int> l_empty_copy(l_empty_orig);
  EXPECT_TRUE(l_empty_copy.empty());
  EXPECT_EQ(l_empty_copy.size(), 0);
}

// Проверка на самоприсваивание перемещением
TEST(ListTest, MoveSelfAssignment) {
  s21::list<double> l1;

// Вот это чисто чтобы не жаловался
// что пытаюсь переместить объект в себя же
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-move"
#endif

  l1 = std::move(l1);

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

  EXPECT_EQ(l1.size(), 0);
  EXPECT_TRUE(l1.empty());
}

// Тест на безопасность деструктора
TEST(ListTest, DestructorSafety) {
  { s21::list<int> l; }
  s21::list<int>* l_ptr = new s21::list<int>();
  delete l_ptr;
  {
    s21::list<char> l1;
    s21::list<char> l2 = std::move(l1);
  }
  SUCCEED();
}

TEST(ListTest, PushBack) {
  s21::list<int> s21_l;
  std::list<int> std_l;

  s21_l.push_back(1);
  s21_l.push_back(2);
  s21_l.push_back(3);

  std_l.push_back(1);
  std_l.push_back(2);
  std_l.push_back(3);

  EXPECT_EQ(s21_l.size(), std_l.size());
  EXPECT_EQ(s21_l.front(), std_l.front());
  EXPECT_EQ(s21_l.back(), std_l.back());

  auto s21_it = s21_l.begin();
  auto std_it = std_l.begin();
  while (s21_it != s21_l.end()) {
    EXPECT_EQ(*s21_it, *std_it);
    ++s21_it;
    ++std_it;
  }
}

TEST(ListTest, PushFront) {
  s21::list<int> s21_l;
  std::list<int> std_l;

  s21_l.push_front(1);
  s21_l.push_front(2);
  s21_l.push_front(3);

  std_l.push_front(1);
  std_l.push_front(2);
  std_l.push_front(3);

  EXPECT_EQ(s21_l.size(), std_l.size());
  EXPECT_EQ(s21_l.front(), std_l.front());  // front() should be 3
  EXPECT_EQ(s21_l.back(), std_l.back());    // back() should be 1

  // Проверка через range-based for, который использует begin()/end()
  int expected[] = {3, 2, 1};
  int i = 0;
  for (const auto& item : s21_l) {
    EXPECT_EQ(item, expected[i++]);
  }
}

TEST(ListTest, PopBack) {
  s21::list<int> s21_l;
  s21_l.push_back(1);
  s21_l.push_back(2);
  s21_l.push_back(3);

  s21_l.pop_back();
  EXPECT_EQ(s21_l.size(), 2);
  EXPECT_EQ(s21_l.back(), 2);

  s21_l.pop_back();
  EXPECT_EQ(s21_l.size(), 1);
  EXPECT_EQ(s21_l.back(), 1);
  EXPECT_EQ(s21_l.front(), 1);

  s21_l.pop_back();
  EXPECT_EQ(s21_l.size(), 0);
  EXPECT_TRUE(s21_l.empty());
}

TEST(ListTest, PopFront) {
  s21::list<int> s21_l;
  s21_l.push_back(1);
  s21_l.push_back(2);
  s21_l.push_back(3);

  s21_l.pop_front();
  EXPECT_EQ(s21_l.size(), 2);
  EXPECT_EQ(s21_l.front(), 2);

  s21_l.pop_front();
  EXPECT_EQ(s21_l.size(), 1);
  EXPECT_EQ(s21_l.front(), 3);
  EXPECT_EQ(s21_l.back(), 3);

  s21_l.pop_front();
  EXPECT_EQ(s21_l.size(), 0);
  EXPECT_TRUE(s21_l.empty());
}

TEST(ListTest, ExceptionsOnEmpty) {
  s21::list<int> l;
  EXPECT_TRUE(l.empty());

  // Проверяем, что методы бросают исключение на пустом списке
  EXPECT_THROW(l.front(), std::out_of_range);
  EXPECT_THROW(l.back(), std::out_of_range);
  EXPECT_THROW(l.pop_front(), std::out_of_range);
  EXPECT_THROW(l.pop_back(), std::out_of_range);
}

TEST(ListTest, MaxSize) {
  s21::list<int> s21_l;
  std::list<int> std_l;

  // В нашей реализации max_size() просто возвращает максимальное значение
  // size_t, в то время как у std::list оно может быть другим. Поэтому просто
  // проверяем, что наше значение соответствует ожидаемому.
  EXPECT_EQ(s21_l.max_size(), std::numeric_limits<size_t>::max());
}

TEST(ListTest, Clear) {
  s21::list<int> l = {1, 2, 3};
  EXPECT_FALSE(l.empty());
  l.clear();
  EXPECT_EQ(l.size(), 0);
  EXPECT_TRUE(l.empty());
  // Проверяем, что в очищенный список можно добавлять элементы
  l.push_back(5);
  EXPECT_EQ(l.size(), 1);
  EXPECT_EQ(l.back(), 5);
}

TEST(ListTest, Insert) {
  s21::list<int> l = {1, 5};
  auto it = l.begin();
  ++it;  // it указывает на 5

  l.insert(it, 2);
  l.insert(it, 3);
  l.insert(it, 4);

  // Ожидаемый список: {1, 2, 3, 4, 5}
  EXPECT_EQ(l.size(), 5);
  int expected[] = {1, 2, 3, 4, 5};
  int i = 0;
  for (const auto& val : l) {
    EXPECT_EQ(val, expected[i++]);
  }
}

TEST(ListTest, Erase) {
  s21::list<int> l = {1, 2, 99, 3};
  auto it = l.begin();
  ++it;
  ++it;  // it указывает на 99

  auto returned_it = l.erase(it);  // Удаляем 99

  EXPECT_EQ(*returned_it, 3);  // erase должен вернуть итератор на 3
  EXPECT_EQ(l.size(), 3);
  EXPECT_EQ(l.front(), 1);
  EXPECT_EQ(l.back(), 3);
}

TEST(ListTest, Swap) {
  s21::list<int> l1 = {1, 2};
  s21::list<int> l2 = {3, 4, 5};

  l1.swap(l2);

  EXPECT_EQ(l1.size(), 3);
  EXPECT_EQ(l1.front(), 3);
  EXPECT_EQ(l2.size(), 2);
  EXPECT_EQ(l2.front(), 1);
}

TEST(ListTest, Splice) {
  s21::list<int> l1 = {1, 5};
  s21::list<int> l2 = {2, 3, 4};

  auto pos = l1.begin();
  ++pos;  // pos указывает на 5

  l1.splice(pos, l2);
  // Ожидаемый l1: {1, 2, 3, 4, 5}
  // Ожидаемый l2: пустой

  EXPECT_TRUE(l2.empty());
  EXPECT_EQ(l1.size(), 5);
  int expected[] = {1, 2, 3, 4, 5};
  int i = 0;
  for (const auto& val : l1) {
    EXPECT_EQ(val, expected[i++]);
  }
}

TEST(ListTest, Merge) {
  s21::list<int> l1 = {1, 5, 10};
  s21::list<int> l2 = {2, 3, 8};

  l1.merge(l2);

  EXPECT_TRUE(l2.empty());
  EXPECT_EQ(l1.size(), 6);
  int expected[] = {1, 2, 3, 5, 8, 10};
  int i = 0;
  for (const auto& val : l1) {
    EXPECT_EQ(val, expected[i++]);
  }
}

TEST(ListTest, Reverse) {
  s21::list<int> l = {5, 4, 3, 2, 1};
  l.reverse();

  int expected[] = {1, 2, 3, 4, 5};
  int i = 0;
  for (const auto& val : l) {
    EXPECT_EQ(val, expected[i++]);
  }
}

TEST(ListTest, Unique) {
  s21::list<int> l = {1, 1, 2, 3, 3, 3, 2, 2, 4};
  l.unique();  // std::list::unique требует отсортированного списка, ваш - нет
               // но стандартное поведение - удаление только СОСЕДНИХ
               // дубликатов.

  // Ожидаемый результат {1, 2, 3, 2, 4}
  EXPECT_EQ(l.size(), 5);
  int expected[] = {1, 2, 3, 2, 4};
  int i = 0;
  for (const auto& val : l) {
    EXPECT_EQ(val, expected[i++]);
  }
}

TEST(ListTest, Sort) {
  s21::list<int> l = {9, 2, 5, 1, 8, 4};
  l.sort();

  EXPECT_EQ(l.size(), 6);
  int expected[] = {1, 2, 4, 5, 8, 9};
  int i = 0;
  for (const auto& val : l) {
    EXPECT_EQ(val, expected[i++]);
  }
}

// --- Тесты для operator= (копирование) ---
TEST(ListTest, CopyAssignmentOperator) {
  s21::list<int> l1 = {1, 2, 3};
  s21::list<int> l2 = {4, 5};
  l1 = l2;  // Присваивание
  EXPECT_EQ(l1.size(), 2);
  EXPECT_EQ(l1.front(), 4);
  l1.push_back(6);
  EXPECT_EQ(l2.size(), 2);  // Проверка на глубокое копирование
  EXPECT_EQ(l2.back(), 5);
}

TEST(ListTest, CopyAssignmentToEmpty) {
  s21::list<int> l1;
  s21::list<int> l2 = {1, 2, 3};
  l1 = l2;
  EXPECT_EQ(l1.size(), 3);
  EXPECT_EQ(l1.back(), 3);
}

TEST(ListTest, CopyAssignmentFromEmpty) {
  s21::list<int> l1 = {1, 2, 3};
  s21::list<int> l2;
  l1 = l2;
  EXPECT_TRUE(l1.empty());
}

TEST(ListTest, CopyAssignmentSelf) {
  s21::list<int> l1 = {1, 2, 3};
  l1 = l1;  // Самоприсваивание
  EXPECT_EQ(l1.size(), 3);
  EXPECT_EQ(l1.front(), 1);
}

// --- Тесты для insert ---
TEST(ListTest, InsertAtBegin) {
  s21::list<int> l = {100, 200};
  l.insert(l.begin(), 50);
  EXPECT_EQ(l.size(), 3);
  EXPECT_EQ(l.front(), 50);
}

TEST(ListTest, InsertAtEnd) {
  s21::list<int> l = {100, 200};
  l.insert(l.end(), 300);  // end() - валидная позиция для insert
  EXPECT_EQ(l.size(), 3);
  EXPECT_EQ(l.back(), 300);
}

// --- Тесты для erase ---
TEST(ListTest, EraseBegin) {
  s21::list<int> l = {1, 2, 3};
  auto it = l.erase(l.begin());
  EXPECT_EQ(*it, 2);
  EXPECT_EQ(l.size(), 2);
  EXPECT_EQ(l.front(), 2);
}

TEST(ListTest, EraseLast) {
  s21::list<int> l = {1, 2, 3};
  auto it = l.end();
  --it;  // Указывает на 3
  it = l.erase(it);
  EXPECT_TRUE(it == l.end());  // Должен вернуть end()
  EXPECT_EQ(l.size(), 2);
  EXPECT_EQ(l.back(), 2);
}

TEST(ListTest, EraseUntilEmpty) {
  s21::list<int> l = {1, 2, 3};
  while (!l.empty()) {
    l.erase(l.begin());
  }
  EXPECT_TRUE(l.empty());
}

// --- Тесты для merge ---
TEST(ListTest, MergeIntoEmpty) {
  s21::list<int> l1;
  s21::list<int> l2 = {1, 2, 3};
  l1.merge(l2);
  EXPECT_EQ(l1.size(), 3);
  EXPECT_TRUE(l2.empty());
  EXPECT_EQ(l1.front(), 1);
}

TEST(ListTest, MergeFromEmpty) {
  s21::list<int> l1 = {1, 2, 3};
  s21::list<int> l2;
  l1.merge(l2);
  EXPECT_EQ(l1.size(), 3);
  EXPECT_TRUE(l2.empty());
}

TEST(ListTest, MergeWithOverlap) {
  s21::list<int> l1 = {1, 5, 6};
  s21::list<int> l2 = {1, 3, 5, 7};
  l1.merge(l2);
  // Ожидаемый результат: {1, 1, 3, 5, 5, 6, 7}
  EXPECT_EQ(l1.size(), 7);
  int expected[] = {1, 1, 3, 5, 5, 6, 7};
  int i = 0;
  for (const auto& val : l1) {
    EXPECT_EQ(val, expected[i++]);
  }
}

// --- Тесты для unique ---
TEST(ListTest, UniqueOnEmptyAndSingle) {
  s21::list<int> l1;
  l1.unique();
  EXPECT_TRUE(l1.empty());

  s21::list<int> l2 = {5};
  l2.unique();
  EXPECT_EQ(l2.size(), 1);
}

TEST(ListTest, UniqueAllSame) {
  s21::list<int> l = {7, 7, 7, 7, 7};
  l.unique();
  EXPECT_EQ(l.size(), 1);
  EXPECT_EQ(l.front(), 7);
}

// --- Тесты для sort ---
TEST(ListTest, SortAlreadySorted) {
  s21::list<int> l = {1, 2, 3, 4, 5};
  l.sort();
  int i = 1;
  for (const auto& val : l) {
    EXPECT_EQ(val, i++);
  }
}

TEST(ListTest, SortReversed) {
  s21::list<int> l = {5, 4, 3, 2, 1};
  l.sort();
  int i = 1;
  for (const auto& val : l) {
    EXPECT_EQ(val, i++);
  }
}

TEST(ListTest, SortWithDuplicates) {
  s21::list<int> l = {5, 1, 5, 2, 1, 2};
  l.sort();
  int expected[] = {1, 1, 2, 2, 5, 5};
  int i = 0;
  for (const auto& val : l) {
    EXPECT_EQ(val, expected[i++]);
  }
}