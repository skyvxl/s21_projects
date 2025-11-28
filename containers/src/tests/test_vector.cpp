#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "s21_containers_tests.h"

// * default constructor

TEST(VectorTest, DefaultConstructor_Test1) {
  s21::vector<int> v;
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.size(), 0);
}

TEST(VectorTest, DefaultConstructor_Test2) {
  s21::vector<double> v;
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.size(), 0);
}

TEST(VectorTest, DefaultConstructor_Test3) {
  s21::vector<s21::vector<int>> v;
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.size(), 0);
}

TEST(VectorTest, DefaultConstructor_Test4) {
  s21::vector<vector<int>> v;
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.size(), 0);
}

// ? constructor with param

TEST(VectorTest, Constructor_Test1) {
  s21::vector<int> v;
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.size(), 0);
}

TEST(VectorTest, Constructor_Test2) {
  const size_t size = 5;
  s21::vector<int> v(size);
  EXPECT_FALSE(v.empty());
  EXPECT_EQ(v.size(), size);
  for (size_t i = 0; i < size; ++i) {
    EXPECT_EQ(v[i], 0);
  }
}

// * test vector<int>.max_size() == 4611686018427387903

TEST(VectorTest, max_size_Test1) {
  s21::vector<int> v;
  EXPECT_EQ(v.max_size(), 4611686018427387903);
}

// * test vector<double>.max_size() == 2305843009213693951

TEST(VectorTest, max_size_Test2) {
  s21::vector<double> v;
  EXPECT_EQ(v.max_size(), 2305843009213693951);
}

// * test vector<string>.max_size() == 576460752303423487

TEST(VectorTest, max_size_Test3) {
  s21::vector<std::string> v;
  EXPECT_EQ(v.max_size(), 576460752303423487);
}

// ?

TEST(VectorTest, Constructor_EmptyVector) {
  s21::vector<int> v(0);
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.size(), 0);
}

TEST(VectorTest, SizeConstructor_PositiveSize) {
  const size_t size = 5;
  s21::vector<int> v(size);
  EXPECT_FALSE(v.empty());
  EXPECT_EQ(v.size(), size);
  for (size_t i = 0; i < size; ++i) {
    EXPECT_EQ(v[i], 0);
  }
}

TEST(VectorTest, SizeConstructor_ZeroSize) {
  const size_t size = 0;
  s21::vector<int> v(size);
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.size(), size);
}

TEST(VectorTest, InitializerListConstructor_EmptyList) {
  s21::vector<int> v = {};
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.size(), 0);
}

TEST(VectorTest, InitializerListConstructor_NonEmptyList) {
  s21::vector<int> v = {1, 2, 3, 4, 5};
  EXPECT_FALSE(v.empty());
  EXPECT_EQ(v.size(), 5);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[4], 5);
}

TEST(VectorTest, CopyConstructor_EmptyVector) {
  s21::vector<int> v1;
  s21::vector<int> v2(v1);
  EXPECT_TRUE(v2.empty());
  EXPECT_EQ(v2.size(), 0);
}

TEST(VectorTest, CopyConstructor_NonEmptyVector) {
  s21::vector<int> v1 = {1, 2, 3};
  s21::vector<int> v2(v1);
  EXPECT_EQ(v2.size(), v1.size());
  for (size_t i = 0; i < v1.size(); ++i) {
    EXPECT_EQ(v2[i], v1[i]);
  }
}

TEST(VectorTest, MoveConstructor_EmptyVector) {
  s21::vector<int> v1;
  s21::vector<int> v2(std::move(v1));
  EXPECT_TRUE(v2.empty());
  EXPECT_EQ(v2.size(), 0);
}

TEST(VectorTest, MoveConstructor_NonEmptyVector) {
  s21::vector<int> v1 = {1, 2, 3};
  s21::vector<int> v2(std::move(v1));
  EXPECT_EQ(v2.size(), 3);
  EXPECT_TRUE(v1.empty());  // Source vector should be empty after move
}

TEST(VectorTest, Destructor_EmptyVector) {
  s21::vector<int>* v = new s21::vector<int>();
  delete v;  // Destructor should be called here
  // No specific assertions, but this ensures the destructor is working
}

TEST(VectorTest, Destructor_NonEmptyVector) {
  s21::vector<int>* v = new s21::vector<int>{1, 2, 3};
  delete v;  // Destructor should be called here
  // No specific assertions, but this ensures the destructor is working
}

TEST(VectorTest, CopyAssignmentOperator_EmptyToEmpty) {
  s21::vector<int> v1;
  s21::vector<int> v2;
  v2 = v1;  // копирующее присваивание
  EXPECT_TRUE(v2.empty());
  EXPECT_EQ(v2.size(), 0);
}

TEST(VectorTest, CopyAssignmentOperator_NonEmptyToEmpty) {
  s21::vector<int> v1 = {1, 2, 3};
  s21::vector<int> v2;
  v2 = v1;  // копирующее присваивание
  EXPECT_EQ(v2.size(), 3);
  EXPECT_EQ(v1.size(), 3);  // исходный вектор не изменился
  for (size_t i = 0; i < v1.size(); ++i) {
    EXPECT_EQ(v2[i], v1[i]);
  }
}

TEST(VectorTest, CopyAssignmentOperator_EmptyToNonEmpty) {
  s21::vector<int> v1;
  s21::vector<int> v2 = {4, 5, 6};
  v2 = v1;  // копирующее присваивание
  EXPECT_TRUE(v2.empty());
  EXPECT_EQ(v2.size(), 0);
}

TEST(VectorTest, CopyAssignmentOperator_NonEmptyToNonEmpty) {
  s21::vector<int> v1 = {1, 2, 3};
  s21::vector<int> v2 = {4, 5, 6, 7};
  v2 = v1;  // копирующее присваивание
  EXPECT_EQ(v2.size(), 3);
  for (size_t i = 0; i < v1.size(); ++i) {
    EXPECT_EQ(v2[i], v1[i]);
  }
}

TEST(VectorTest, CopyAssignmentOperator_SelfAssignment) {
  s21::vector<int> v = {1, 2, 3};
  v = v;  // самоприсваивание
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
}

TEST(VectorTest, MoveAssignmentOperator_EmptyToEmpty) {
  s21::vector<int> v1;
  s21::vector<int> v2;
  v2 = std::move(v1);
  EXPECT_TRUE(v2.empty());
  EXPECT_EQ(v2.size(), 0);
}

TEST(VectorTest, MoveAssignmentOperator_NonEmptyToEmpty) {
  s21::vector<int> v1 = {1, 2, 3};
  s21::vector<int> v2;
  v2 = std::move(v1);
  EXPECT_EQ(v2.size(), 3);
  EXPECT_TRUE(v1.empty());  // Source vector should be empty after move
}

TEST(VectorTest, MoveAssignmentOperator_EmptyToNonEmpty) {
  s21::vector<int> v1;
  s21::vector<int> v2 = {4, 5, 6};
  v2 = std::move(v1);
  EXPECT_TRUE(v2.empty());
  EXPECT_EQ(v2.size(), 0);
}

TEST(VectorTest, MoveAssignmentOperator_NonEmptyToNonEmpty) {
  s21::vector<int> v1 = {1, 2, 3};
  s21::vector<int> v2 = {4, 5, 6};
  v2 = std::move(v1);
  EXPECT_EQ(v2.size(), 3);
  EXPECT_TRUE(v1.empty());  // Source vector should be empty after move
}

TEST(VectorTest, At_ValidIndex) {
  s21::vector<int> v = {1, 2, 3, 4, 5};
  EXPECT_EQ(v.at(0), 1);
  EXPECT_EQ(v.at(2), 3);
  EXPECT_EQ(v.at(4), 5);
}

TEST(VectorTest, At_InvalidIndex) {
  s21::vector<int> v = {1, 2, 3, 4, 5};
  EXPECT_THROW(v.at(5), std::out_of_range);
  EXPECT_THROW(v.at(10), std::out_of_range);
}

TEST(VectorTest, IndexOperator_ValidIndex) {
  s21::vector<int> v = {1, 2, 3, 4, 5};
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(v[4], 5);
}

TEST(VectorTest, IndexOperator_InvalidIndex) {
  s21::vector<int> v = {1, 2, 3, 4, 5};
  EXPECT_NO_THROW(v[5]);
  EXPECT_NO_THROW(v[10]);
}

TEST(VectorTest, ConstIndexOperator_ValidIndex) {
  const s21::vector<int> v = {1, 2, 3, 4, 5};
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(v[4], 5);
}

TEST(VectorTest, ConstIndexOperator_InvalidIndex) {
  const s21::vector<int> v = {1, 2, 3, 4, 5};
  EXPECT_NO_THROW(v[5]);
  EXPECT_NO_THROW(v[10]);
}

TEST(VectorTest, FrontConst_ValidVector) {
  const s21::vector<int> v = {1, 2, 3, 4, 5};
  EXPECT_EQ(v.front(), 1);
}

TEST(VectorTest, FrontConst_EmptyVector) {
  const s21::vector<int> v;
  EXPECT_NO_THROW(v.front());
}

TEST(VectorTest, BackConst_ValidVector) {
  const s21::vector<int> v = {1, 2, 3, 4, 5};
  EXPECT_EQ(v.back(), 5);
}

TEST(VectorTest, BackConst_EmptyVector) {
  const s21::vector<int> v;
  EXPECT_NO_THROW(v.back());
}

TEST(VectorTest, Data_ValidVector) {
  s21::vector<int> v = {1, 2, 3, 4, 5};
  int* dataPtr = v.data();
  EXPECT_EQ(*dataPtr, 1);
  // Modify the data through the pointer and check if the vector is updated
  *dataPtr = 10;
  EXPECT_EQ(v[0], 10);
}

TEST(VectorTest, Data_EmptyVector) {
  s21::vector<int> v;
  int* dataPtr = v.data();
  // Data pointer of an empty vector can be anything
  EXPECT_EQ(dataPtr, nullptr);
}

// --- front() ---

TEST(VectorTest, Front_NonConst_ValidVector) {
  s21::vector<int> v = {10, 20, 30};
  EXPECT_EQ(v.front(), 10);
  v.front() = 99;
  EXPECT_EQ(v[0], 99);
}

TEST(VectorTest, Front_NonConst_EmptyVector) {
  s21::vector<int> v;
  // Поведение не определено, но проверим, что не падает (можно ожидать UB)
  EXPECT_NO_THROW(v.front());
}

TEST(VectorTest, Front_NonConst_Modify) {
  s21::vector<std::string> v = {"abc", "def"};
  v.front() = "xyz";
  EXPECT_EQ(v[0], "xyz");
}

// --- back() ---

TEST(VectorTest, Back_NonConst_ValidVector) {
  s21::vector<int> v = {1, 2, 3, 4};
  EXPECT_EQ(v.back(), 4);
  v.back() = 42;
  EXPECT_EQ(v[v.size() - 1], 42);
}

TEST(VectorTest, Back_NonConst_EmptyVector) {
  s21::vector<int> v;
  EXPECT_NO_THROW(v.back());
}

TEST(VectorTest, Back_NonConst_Modify) {
  s21::vector<std::string> v = {"a", "b", "c"};
  v.back() = "zzz";
  EXPECT_EQ(v[2], "zzz");
}

// --- data() ---

TEST(VectorTest, Data_NonConst_ValidVector) {
  s21::vector<int> v = {5, 6, 7};
  int* ptr = v.data();
  ASSERT_NE(ptr, nullptr);
  ptr[1] = 66;
  EXPECT_EQ(v[1], 66);
}

TEST(VectorTest, Data_NonConst_EmptyVector) {
  s21::vector<int> v;
  int* ptr = v.data();
  EXPECT_EQ(ptr, nullptr);
}

TEST(VectorTest, Data_NonConst_ModifyAll) {
  s21::vector<int> v = {1, 2, 3, 4, 5};
  int* ptr = v.data();
  for (size_t i = 0; i < v.size(); ++i) {
    ptr[i] = static_cast<int>(i * 10);
  }
  EXPECT_EQ(v[0], 0);
  EXPECT_EQ(v[4], 40);
}

TEST(VectorTest, Begin_ValidVector) {
  s21::vector<int> v = {1, 2, 3, 4, 5};
  auto it = v.begin();
  EXPECT_EQ(*it, 1);
  // Modify the data through the iterator and check if the vector is updated
  *it = 10;
  EXPECT_EQ(v[0], 10);
}

TEST(VectorTest, Begin_EmptyVector) {
  s21::vector<int> v;
  auto it = v.begin();
  // The begin() of an empty vector should be the end() too, so iterator
  // should be equal to end()
  EXPECT_EQ(it, v.end());
}

TEST(VectorTest, End_ValidVector) {
  s21::vector<int> v = {1, 2, 3, 4, 5};
  auto it = v.end();
  // end() iterator should not be dereferenced; comparing it to another
  // iterator should work
  auto beginIt = v.begin();
  EXPECT_NE(it, beginIt);
}

TEST(VectorTest, End_EmptyVector) {
  s21::vector<int> v;
  auto it = v.end();
  // The end() of an empty vector should be the begin() too, so iterator
  // should be equal to begin()
  EXPECT_EQ(it, v.begin());
}

TEST(VectorTest, CBegin_ValidVector) {
  const s21::vector<int> v = {1, 2, 3, 4, 5};
  auto it = v.cbegin();
  EXPECT_EQ(*it, 1);
  // Modify the data through the iterator and check if the vector is not
  // updated (should be const_iterator) Uncommenting the following line should
  // result in a compilation error. *it = 10;
}

TEST(VectorTest, CBegin_EmptyVector) {
  const s21::vector<int> v;
  auto it = v.cbegin();
  // The cbegin() of an empty vector should be the cend() too, so iterator
  // should be equal to cend()
  EXPECT_EQ(it, v.cend());
}

TEST(VectorTest, CEnd_ValidVector) {
  const s21::vector<int> v = {1, 2, 3, 4, 5};
  auto it = v.cend();
  // cend() iterator should not be dereferenced; comparing it to another
  // iterator should work
  auto beginIt = v.cbegin();
  EXPECT_EQ(it, beginIt + 5);
}

TEST(VectorTest, CEnd_EmptyVector) {
  const s21::vector<int> v;
  auto it = v.cend();
  // The cend() of an empty vector should be the cbegin() too, so iterator
  // should be equal to cbegin()
  EXPECT_EQ(it, v.cbegin());
}

// --- rbegin/rend ---

TEST(VectorTest, RBegin_ValidVector) {
  s21::vector<int> v = {1, 2, 3, 4, 5};
  auto rit = v.rbegin();
  EXPECT_EQ(*rit, 5);
  *rit = 42;
  EXPECT_EQ(v[4], 42);
}

TEST(VectorTest, RBegin_EmptyVector) {
  s21::vector<int> v;
  auto rit = v.rbegin();
  // rbegin() == rend() для пустого вектора
  EXPECT_EQ(rit, v.rend());
}

TEST(VectorTest, REnd_ValidVector) {
  s21::vector<int> v = {1, 2, 3, 4, 5};
  auto rit = v.rend();
  auto rbeginIt = v.rbegin();
  EXPECT_NE(rit, rbeginIt);
  // rit не должен разыменовываться, но можно сравнивать
}

TEST(VectorTest, REnd_EmptyVector) {
  s21::vector<int> v;
  auto rit = v.rend();
  EXPECT_EQ(rit, v.rbegin());
}

// --- crbegin/crend ---

TEST(VectorTest, CRBegin_ValidVector) {
  const s21::vector<int> v = {1, 2, 3, 4, 5};
  auto crit = v.crbegin();
  EXPECT_EQ(*crit, 5);
  // *crit = 10; // должно не компилироваться (const)
}

TEST(VectorTest, CRBegin_EmptyVector) {
  const s21::vector<int> v;
  auto crit = v.crbegin();
  EXPECT_EQ(crit, v.crend());
}

TEST(VectorTest, CREnd_ValidVector) {
  const s21::vector<int> v = {1, 2, 3, 4, 5};
  auto crit = v.crend();
  auto crbeginIt = v.crbegin();
  EXPECT_NE(crit, crbeginIt);
}

TEST(VectorTest, CREnd_EmptyVector) {
  const s21::vector<int> v;
  auto crit = v.crend();
  EXPECT_EQ(crit, v.crbegin());
}

TEST(VectorTest, ReverseIteration) {
  s21::vector<int> v = {1, 2, 3, 4, 5};
  int sum = 0;
  for (auto it = v.rbegin(); it != v.rend(); ++it) {
    sum += *it;
  }
  EXPECT_EQ(sum, 15);  // 5+4+3+2+1
}

TEST(VectorTest, ConstReverseIteration) {
  const s21::vector<int> v = {1, 2, 3};
  std::vector<int> result;
  for (auto it = v.crbegin(); it != v.crend(); ++it) {
    result.push_back(*it);
  }
  ASSERT_EQ(result.size(), 3u);
  EXPECT_EQ(result[0], 3);
  EXPECT_EQ(result[1], 2);
  EXPECT_EQ(result[2], 1);
}

TEST(VectorTest, Empty_EmptyVector) {
  const s21::vector<int> v;
  EXPECT_TRUE(v.empty());
}

TEST(VectorTest, Empty_NonEmptyVector) {
  const s21::vector<int> v = {1, 2, 3};
  EXPECT_FALSE(v.empty());
}

TEST(VectorTest, Size_EmptyVector) {
  const s21::vector<int> v;
  EXPECT_EQ(v.size(), 0);
}

TEST(VectorTest, Size_NonEmptyVector) {
  const s21::vector<int> v = {1, 2, 3};
  EXPECT_EQ(v.size(), 3);
}

TEST(VectorTest, MaxSize) {
  const s21::vector<int> v;
  // You can't predict the exact max size, but it should be a very large
  // value
  EXPECT_GT(v.max_size(), 0);
}

TEST(VectorTest, Reserve_IncreaseCapacity) {
  s21::vector<int> v;
  size_t initialCapacity = v.capacity();
  size_t newCapacity = initialCapacity + 10;
  v.reserve(newCapacity);
  EXPECT_GE(v.capacity(), newCapacity);
}

TEST(VectorTest, Reserve_IncreaseCapacityToERROR) {
  s21::vector<int> v;
  size_t newCapacity = v.max_size() + 1;
  EXPECT_ANY_THROW(v.reserve(newCapacity));
  EXPECT_EQ(v.capacity(), 0);
}

TEST(VectorTest, Reserve_DecreaseCapacity) {
  s21::vector<int> v = {1, 2, 3, 4, 5};
  size_t initialCapacity = v.capacity();
  size_t newCapacity = initialCapacity - 2;
  v.reserve(newCapacity);
  // Capacity may not decrease immediately, but it shouldn't exceed the
  // requested value
  EXPECT_LE(v.capacity(), initialCapacity);
  EXPECT_GE(v.capacity(), newCapacity);
}

TEST(VectorTest, Capacity_EmptyVector) {
  const s21::vector<int> v;
  // The capacity of an empty vector should be at least 0
  EXPECT_GE(v.capacity(), 0);
}

TEST(VectorTest, Capacity_NonEmptyVector) {
  const s21::vector<int> v = {1, 2, 3};
  // The capacity of a non-empty vector might vary, but it should be at least
  // the size of the vector
  EXPECT_GE(v.capacity(), v.size());
}

TEST(VectorTest, ShrinkToFit_EmptyVector) {
  s21::vector<int> v;
  v.shrink_to_fit();
  // After shrink_to_fit() on an empty vector, the capacity should still be
  // at
  // least 0
  EXPECT_GE(v.capacity(), 0);
}

TEST(VectorTest, Reserve_DoesNotDecreaseCapacity) {
  s21::vector<int> v = {1, 2, 3, 4, 5};
  size_t initialCapacity = v.capacity();
  v.reserve(initialCapacity - 1);  // меньше текущей capacity
  EXPECT_EQ(v.capacity(), initialCapacity);  // capacity не должна уменьшиться
}

TEST(VectorTest, Reserve_ZeroOnEmptyVector) {
  s21::vector<int> v;
  v.reserve(0);
  EXPECT_EQ(v.capacity(), 0);
}

TEST(VectorTest, ShrinkToFit_NonEmptyVector) {
  s21::vector<int> v = {1, 2, 3, 4, 5};
  v.reserve(100);  // увеличиваем capacity
  size_t before = v.capacity();
  v.shrink_to_fit();
  EXPECT_EQ(v.capacity(), v.size());
  EXPECT_LT(v.capacity(), before);
  // Проверяем, что данные не потерялись
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[4], 5);
}

// TEST(VectorTest, ShrinkToFit_AfterPopBack) {
//   s21::vector<int> v = {1, 2, 3, 4, 5};
//   v.reserve(20);
//   v.pop_back();
//   v.pop_back();
//   v.shrink_to_fit();
//   EXPECT_EQ(v.capacity(), v.size());
//   EXPECT_EQ(v.size(), 3u);
//   EXPECT_EQ(v[2], 3);
// }

// TEST(VectorTest, ShrinkToFit_NonEmptyVector) {
//   s21::vector<int> v = {1, 2, 3, 4, 5};
//   size_t initialCapacity = v.capacity();
//   v.pop_back();  // Simulate a reduction in size
//   v.shrink_to_fit();
//   // After shrink_to_fit(), the capacity should not exceed the new size
//   EXPECT_LE(v.capacity(), v.size());
//   // The capacity may not necessarily decrease, but it should not be larger
//   than
//   // the initial capacity
//   EXPECT_LE(v.capacity(), initialCapacity);
// }

TEST(VectorTest, Clear_EmptyVector) {
  s21::vector<int> v;
  v.clear();
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.size(), 0);
  // Clearing an empty vector should not affect capacity
  EXPECT_GE(v.capacity(), 0);
}

TEST(VectorTest, Clear_NonEmptyVector) {
  s21::vector<int> v = {1, 2, 3, 4, 5};
  v.clear();
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.size(), 0);
  // Clearing a vector should not affect capacity immediately
  EXPECT_GE(v.capacity(), 0);
}

TEST(VectorTest, Insert_Beginning) {
  s21::vector<int> v = {2, 3, 4};
  auto it = v.insert(v.begin(), 1);
  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(*it, 1);
}

TEST(VectorTest, Insert_Middle) {
  s21::vector<int> v = {1, 3, 4};
  auto it = v.insert(v.begin() + 1, 2);
  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(*it, 2);
}

TEST(VectorTest, Insert_End) {
  s21::vector<int> v = {1, 2, 3};
  auto it = v.insert(v.end(), 4);
  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(v[3], 4);
  EXPECT_EQ(*it, 4);
}

TEST(VectorTest, Insert_EmptyVector) {
  s21::vector<int> v;
  auto it = v.insert(v.begin(), 42);
  EXPECT_EQ(v.size(), 1);
  EXPECT_EQ(v[0], 42);
  EXPECT_EQ(*it, 42);
}

TEST(VectorTest, Insert_MultipleInserts) {
  s21::vector<int> v = {1, 4};
  auto it1 = v.insert(v.begin() + 1, 2);
  auto it2 = v.insert(v.begin() + 2, 3);
  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(*it1, 2);
  EXPECT_EQ(*it2, 3);
}

TEST(VectorTest, Insert_ElementFromVector) {
  s21::vector<int> v = {1, 2, 3};
  auto it = v.insert(v.begin() + 1, v[2]);  // Вставляем последний элемент
  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(v[1], 3);
  EXPECT_EQ(v[2], 2);  // Проверяем, что старый элемент сдвинулся
  EXPECT_EQ(*it, 3);
}

TEST(VectorTest, Insert_WithReallocation) {
  s21::vector<int> v = {1, 2, 3};
  v.reserve(3);  // Устанавливаем capacity = 3
  auto it = v.insert(v.begin() + 1, 42);  // Должна произойти реаллокация
  EXPECT_EQ(v.size(), 4);
  EXPECT_GE(v.capacity(), 4);  // Проверяем, что capacity увеличилось
  EXPECT_EQ(v[1], 42);
  EXPECT_EQ(*it, 42);
}

TEST(VectorTest, Insert_StringType) {
  s21::vector<std::string> v = {"a", "c"};
  auto it = v.insert(v.begin() + 1, "b");
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v[1], "b");
  EXPECT_EQ(*it, "b");
}

TEST(VectorTest, Insert_vectorType) {
  s21::vector<std::string> v1 = {"a", "c"};
  s21::vector<std::string> v2 = {"a", "c"};
  s21::vector<std::string> v3 = {"b", "c"};
  s21::vector<std::string> v3_copy = v3;  // Сохраняем копию

  s21::vector<s21::vector<std::string>> v = {v1, v2};
  auto it = v.insert(v.begin() + 1, v3);
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ((v[0] == v1), true);
  EXPECT_EQ(v[1] == v3, true);
  EXPECT_EQ(v[2] == v2, true);

  EXPECT_EQ(*it, v3_copy);  // Сравниваем с копией
}

TEST(VectorTest, InsertN_Beginning) {
  s21::vector<int> v = {2, 3, 4};
  auto it = v.insert(v.begin(), 2, 1);  // Вставляем два элемента 1 в начало
  EXPECT_EQ(v.size(), 5);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 1);
  EXPECT_EQ(v[2], 2);
  EXPECT_EQ(*it, 1);  // Итератор указывает на первый вставленный элемент
}

TEST(VectorTest, InsertN_Middle) {
  s21::vector<int> v = {1, 4, 5};
  auto it =
      v.insert(v.begin() + 1, 2, 2);  // Вставляем два элемента 2 в середину
  EXPECT_EQ(v.size(), 5);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 2);
  EXPECT_EQ(v[3], 4);
  EXPECT_EQ(*it, 2);
}

TEST(VectorTest, InsertN_End) {
  s21::vector<int> v = {1, 2, 3};
  auto it = v.insert(v.end(), 3, 4);  // Вставляем три элемента 4 в конец
  EXPECT_EQ(v.size(), 6);
  EXPECT_EQ(v[3], 4);
  EXPECT_EQ(v[4], 4);
  EXPECT_EQ(v[5], 4);
  EXPECT_EQ(*it, 4);
}

TEST(VectorTest, InsertN_EmptyVector) {
  s21::vector<int> v;
  auto it =
      v.insert(v.begin(), 3, 42);  // Вставляем три элемента 42 в пустой вектор
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v[0], 42);
  EXPECT_EQ(v[1], 42);
  EXPECT_EQ(v[2], 42);
  EXPECT_EQ(*it, 42);
}

TEST(VectorTest, InsertN_ZeroElements) {
  s21::vector<int> v = {1, 2, 3};
  auto it = v.insert(v.begin() + 1, 0, 5);  // Вставляем 0 элементов
  EXPECT_EQ(v.size(), 3);  // Размер не должен измениться
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(it, v.begin() + 1);  // Итератор должен указывать на позицию вставки
}

TEST(VectorTest, InsertN_WithReallocation) {
  s21::vector<int> v = {1, 2, 3};
  v.reserve(3);  // Устанавливаем capacity = 3
  auto it = v.insert(v.begin() + 1, 3, 42);  // Должна произойти реаллокация
  EXPECT_EQ(v.size(), 6);
  EXPECT_GE(v.capacity(), 6);  // Проверяем, что capacity увеличилось
  EXPECT_EQ(v[1], 42);
  EXPECT_EQ(v[2], 42);
  EXPECT_EQ(v[3], 42);
  EXPECT_EQ(*it, 42);
}

TEST(VectorTest, InsertN_StringType) {
  s21::vector<std::string> v = {"a", "d"};
  auto it = v.insert(v.begin() + 1, 2, "b");  // Вставляем две строки "b"
  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(v[1], "b");
  EXPECT_EQ(v[2], "b");
  EXPECT_EQ(*it, "b");
}

TEST(VectorTest, InsertN_VectorType) {
  s21::vector<std::string> v1 = {"a", "b"};
  s21::vector<std::string> v2 = {"c", "d"};
  s21::vector<std::string> pattern = {"x", "y"};

  s21::vector<s21::vector<std::string>> v = {v1, v2};
  auto it =
      v.insert(v.begin() + 1, 2, pattern);  // Вставляем два вектора pattern

  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(v[0], v1);
  EXPECT_EQ(v[1], pattern);
  EXPECT_EQ(v[2], pattern);
  EXPECT_EQ(v[3], v2);
  EXPECT_EQ(*it, pattern);
}

TEST(VectorTest, InsertN_SelfReference) {
  s21::vector<int> v = {1, 2, 3};
  auto it =
      v.insert(v.begin() + 1, 2,
               v[2]);  // Вставляем два элемента, равных последнему элементу

  EXPECT_EQ(v.size(), 5);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 3);  // Первый вставленный элемент
  EXPECT_EQ(v[2], 3);  // Второй вставленный элемент
  EXPECT_EQ(v[3], 2);  // Оригинальный второй элемент
  EXPECT_EQ(v[4], 3);  // Оригинальный третий элемент
  EXPECT_EQ(*it, 3);
}

TEST(VectorTest, InsertRange_Beginning) {
  s21::vector<int> v = {2, 3, 4};
  s21::vector<int> to_insert = {0, 1};
  auto it = v.insert(v.begin(), to_insert.begin(), to_insert.end());
  EXPECT_EQ(v.size(), 5);
  EXPECT_EQ(v[0], 0);
  EXPECT_EQ(v[1], 1);
  EXPECT_EQ(v[2], 2);
  EXPECT_EQ(*it, 0);
}

TEST(VectorTest, InsertRange_Middle) {
  s21::vector<int> v = {1, 4, 5};
  s21::vector<int> to_insert = {2, 3};
  auto it = v.insert(v.begin() + 1, to_insert.begin(), to_insert.end());
  EXPECT_EQ(v.size(), 5);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(v[3], 4);
  EXPECT_EQ(*it, 2);
}

TEST(VectorTest, InsertRange_End) {
  s21::vector<int> v = {1, 2, 3};
  s21::vector<int> to_insert = {4, 5, 6};
  auto it = v.insert(v.end(), to_insert.begin(), to_insert.end());
  EXPECT_EQ(v.size(), 6);
  EXPECT_EQ(v[3], 4);
  EXPECT_EQ(v[4], 5);
  EXPECT_EQ(v[5], 6);
  EXPECT_EQ(*it, 4);
}

TEST(VectorTest, InsertRange_EmptyVector) {
  s21::vector<int> v;
  s21::vector<int> to_insert = {1, 2, 3};
  auto it = v.insert(v.begin(), to_insert.begin(), to_insert.end());
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(*it, 1);
}

TEST(VectorTest, InsertRange_EmptyRange) {
  s21::vector<int> v = {1, 2, 3};
  s21::vector<int> to_insert;
  auto it = v.insert(v.begin() + 1, to_insert.begin(), to_insert.end());
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(it, v.begin() + 1);
}

TEST(VectorTest, InsertRange_WithReallocation) {
  s21::vector<int> v = {1, 2, 3};
  v.reserve(3);
  s21::vector<int> to_insert = {4, 5, 6};
  auto it = v.insert(v.begin() + 1, to_insert.begin(), to_insert.end());
  EXPECT_EQ(v.size(), 6);
  EXPECT_GE(v.capacity(), 6);
  EXPECT_EQ(v[1], 4);
  EXPECT_EQ(v[2], 5);
  EXPECT_EQ(v[3], 6);
  EXPECT_EQ(*it, 4);
}

TEST(VectorTest, InsertRange_StringType) {
  s21::vector<std::string> v = {"a", "d"};
  s21::vector<std::string> to_insert = {"b", "c"};
  auto it = v.insert(v.begin() + 1, to_insert.begin(), to_insert.end());
  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(v[1], "b");
  EXPECT_EQ(v[2], "c");
  EXPECT_EQ(*it, "b");
}

TEST(VectorTest, InsertRange_VectorType) {
  s21::vector<std::string> v1 = {"a", "b"};
  s21::vector<std::string> v2 = {"c", "d"};
  s21::vector<std::string> pattern1 = {"x", "y"};
  s21::vector<std::string> pattern2 = {"z", "w"};

  s21::vector<s21::vector<std::string>> v = {v1, v2};
  s21::vector<s21::vector<std::string>> to_insert = {pattern1, pattern2};
  auto it = v.insert(v.begin() + 1, to_insert.begin(), to_insert.end());

  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(v[0], v1);
  EXPECT_EQ(v[1], pattern1);
  EXPECT_EQ(v[2], pattern2);
  EXPECT_EQ(v[3], v2);
  EXPECT_EQ(*it, pattern1);
}

TEST(VectorTest, InsertRange_SelfReference) {
  s21::vector<int> v = {1, 2, 3, 4};
  auto it = v.insert(v.begin() + 1, v.begin() + 2, v.end());

  EXPECT_EQ(v.size(), 6);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 3);  // Первый вставленный элемент
  EXPECT_EQ(v[2], 4);  // Второй вставленный элемент
  EXPECT_EQ(v[3], 2);  // Оригинальный второй элемент
  EXPECT_EQ(v[4], 3);  // Оригинальный третий элемент
  EXPECT_EQ(v[5], 4);  // Оригинальный четвертый элемент
  EXPECT_EQ(*it, 3);
}

TEST(VectorTest, InsertRange_PartialSelfReference) {
  s21::vector<int> v = {1, 2, 3, 4, 5};
  auto it = v.insert(v.begin() + 2, v.begin(), v.begin() + 2);

  EXPECT_EQ(v.size(), 7);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 1);  // Первый вставленный элемент
  EXPECT_EQ(v[3], 2);  // Второй вставленный элемент
  EXPECT_EQ(v[4], 3);  // Оригинальный третий элемент
  EXPECT_EQ(v[5], 4);  // Оригинальный четвертый элемент
  EXPECT_EQ(v[6], 5);  // Оригинальный пятый элемент
  EXPECT_EQ(*it, 1);
}

TEST(VectorTest, Erase_Beginning) {
  s21::vector<int> v = {1, 2, 3, 4};
  v.erase(v.begin());  // Erase the first element
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v[0], 2);
}

TEST(VectorTest, Erase_Middle) {
  s21::vector<int> v = {1, 2, 3, 4};
  v.erase(v.begin() + 1);
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v[1], 3);
}

TEST(VectorTest, Erase_End) {
  s21::vector<int> v = {1, 2, 3, 4};
  v.erase(v.end() - 1);  // Erase the last element
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v[2], 3);
}

// Тесты для вектора векторов (сложный тип данных)
TEST(VectorVectorTest, Erase_Beginning) {
  s21::vector<s21::vector<int>> v = {{1, 2}, {3, 4}, {5, 6}};
  v.erase(v.begin());  // Удаление первого элемента
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], (s21::vector<int>{3, 4}));
}

TEST(VectorVectorTest, Erase_Middle) {
  s21::vector<s21::vector<int>> v = {{1, 2}, {3, 4}, {5, 6}};
  v.erase(v.begin() + 1);  // Удаление среднего элемента
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[1], (s21::vector<int>{5, 6}));
}

TEST(VectorVectorTest, Erase_End) {
  s21::vector<s21::vector<int>> v = {{1, 2}, {3, 4}, {5, 6}};
  v.erase(v.end() - 1);  // Удаление последнего элемента
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v.back(), (s21::vector<int>{3, 4}));
}

TEST(VectorTest, Erase_AllElements) {
  s21::vector<std::string> v = {"one", "two", "three"};
  v.erase(v.begin());
  v.erase(v.begin());
  v.erase(v.begin());
  EXPECT_TRUE(
      v.empty());  // Проверка, что вектор пуст после удаления всех элементов
}

TEST(VectorVectorTest, Erase_NestedEmptyVector) {
  s21::vector<s21::vector<int>> v = {{}, {1, 2}, {3, 4}};
  v.erase(v.begin());  // Удаление пустого вложенного вектора
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], (s21::vector<int>{1, 2}));
}

// Тесты для простого типа данных (int)
TEST(VectorTest, EraseRange_Beginning) {
  s21::vector<int> v = {1, 2, 3, 4, 5};
  auto it = v.erase(v.begin(), v.begin() + 2);  // Удаляем первые 2 элемента
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v[0], 3);
  EXPECT_EQ(*it, 3);  // Проверяем, что возвращен правильный итератор
}

TEST(VectorTest, EraseRange_Middle) {
  s21::vector<int> v = {1, 2, 3, 4, 5};
  auto it = v.erase(v.begin() + 1, v.begin() + 3);  // Удаляем элементы 2 и 3
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v[1], 4);
  EXPECT_EQ(*it, 4);
}

TEST(VectorTest, EraseRange_End) {
  s21::vector<int> v = {1, 2, 3, 4, 5};
  auto it = v.erase(v.end() - 2, v.end());  // Удаляем последние 2 элемента
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v.back(), 3);
  EXPECT_EQ(it, v.end());
}

// Тесты для сложного типа данных (vector<vector<int>>)
TEST(VectorVectorTest, EraseRange_Beginning) {
  s21::vector<s21::vector<int>> v = {{1, 2}, {3, 4}, {5, 6}, {7, 8}};
  auto it = v.erase(v.begin(), v.begin() + 2);  // Удаляем первые 2 вектора
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], (s21::vector<int>{5, 6}));
  EXPECT_EQ(*it, (s21::vector<int>{5, 6}));
}

TEST(VectorVectorTest, EraseRange_Middle) {
  s21::vector<s21::vector<int>> v = {{1}, {2, 2}, {3, 3, 3}, {4, 4}};
  auto it = v.erase(v.begin() + 1, v.begin() + 3);  // Удаляем 2 и 3 элементы
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[1], (s21::vector<int>{4, 4}));
  EXPECT_EQ(*it, (s21::vector<int>{4, 4}));
}

TEST(VectorVectorTest, EraseRange_End) {
  s21::vector<s21::vector<int>> v = {{1}, {2}, {3}, {4}};
  auto it = v.erase(v.end() - 3, v.end());  // Удаляем последние 3 элемента
  EXPECT_EQ(v.size(), 1);
  EXPECT_EQ(v[0], (s21::vector<int>{1}));
  EXPECT_EQ(it, v.end());
}

// Тесты для критических ситуаций
TEST(VectorTest, EraseRange_EmptyRange) {
  s21::vector<int> v = {1, 2, 3, 4};
  auto it = v.erase(v.begin() + 1, v.begin() + 1);  // Пустой диапазон
  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(*it, 2);
  // Итератор должен указывать на первый элемент
  // диапазона
}

TEST(VectorTest, EraseRange_AllElements) {
  s21::vector<std::string> v = {"one", "two", "three"};
  auto it = v.erase(v.begin(), v.end());  // Удаляем все элементы
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(it, v.end());
}

// Тест на добавление в пустой вектор
TEST(VectorTest, PushBack_EmptyVector) {
  s21::vector<int> v;
  v.push_back(1);
  EXPECT_EQ(v.size(), 1);
  EXPECT_EQ(v[0], 1);
}

// Тест на добавление в непустой вектор
TEST(VectorTest, PushBack_NonEmptyVector) {
  s21::vector<int> v = {1, 2, 3};
  v.push_back(4);
  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(v[3], 4);
}

// Тест на добавление при необходимости реаллокации
TEST(VectorTest, PushBack_WithReallocation) {
  s21::vector<int> v;
  v.reserve(2);
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);  // Должно вызвать реаллокацию
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v.capacity(), 4);  // Проверяем, что capacity увеличилось
  EXPECT_EQ(v[2], 3);
}

// Тест на добавление большого количества элементов
TEST(VectorTest, PushBack_MultipleElements) {
  s21::vector<int> v;
  for (int i = 0; i < 1000; ++i) {
    v.push_back(i);
    EXPECT_EQ(v.size(), i + 1);
    EXPECT_EQ(v[i], i);
  }
}

// Тест на добавление строк (проверка работы с нетривиальными типами)
TEST(VectorTest, PushBack_Strings) {
  s21::vector<std::string> v;
  v.push_back("first");
  v.push_back("second");
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], "first");
  EXPECT_EQ(v[1], "second");
}

// Тест на перемещающую семантику (если реализован push_back(T&&))
TEST(VectorTest, PushBack_MoveSemantics) {
  s21::vector<std::string> v;
  std::string str = "test";
  v.push_back(std::move(str));
  EXPECT_EQ(v.size(), 1);
  EXPECT_EQ(v[0], "test");
  EXPECT_TRUE(str.empty());  // Проверяем, что исходная строка была перемещена
}

// Тест на добавление в вектор векторов (сложный тип)
TEST(VectorTest, PushBack_VectorOfVectors) {
  s21::vector<s21::vector<int>> v;
  v.push_back({1, 2, 3});
  v.push_back({4, 5});
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0].size(), 3);
  EXPECT_EQ(v[1].size(), 2);
}

// Тест на добавление элементов разных типов (если вектор шаблонный)
TEST(VectorTest, PushBack_DifferentTypes) {
  s21::vector<double> v;
  v.push_back(1);     // int
  v.push_back(2.5);   // double
  v.push_back(3.0f);  // float
  EXPECT_EQ(v.size(), 3);
  EXPECT_DOUBLE_EQ(v[0], 1.0);
  EXPECT_DOUBLE_EQ(v[1], 2.5);
  EXPECT_DOUBLE_EQ(v[2], 3.0);
}

TEST(VectorTest, PushBack_InvalidatesIteratorsOnReallocation) {
  s21::vector<int> v = {1, 2, 3};
  v.reserve(3);  // Убедимся, что следующий push_back вызовет реаллокацию
  auto it = v.begin();
  v.push_back(4);  // Реаллокация гарантирована

  // Нельзя разыменовывать it - можно только проверить, что он не равен новому
  // begin()
  EXPECT_NE(it, v.begin());  // Итератор указывает на старую память
}

TEST(VectorTest, PushBack_KeepsIteratorsWhenNoReallocation) {
  s21::vector<int> v = {1, 2, 3};
  v.reserve(10);  // Достаточно места
  auto it = v.begin();
  v.push_back(4);  // Реаллокации нет

  EXPECT_EQ(*it, 1);  // Итератор остаётся валидным
  EXPECT_EQ(v.size(), 4);
}

TEST(VectorTest, PushBack_InvalidatesPointers) {
  s21::vector<int> v = {1, 2, 3};
  int* p = &v[0];
  v.push_back(4);  // Возможна реаллокация

  // Нельзя использовать p - только для демонстрации
  EXPECT_NE(p, &v[0]);  // Указатель изменился
}

TEST(VectorTest, PopBack_NonEmptyVector) {
  s21::vector<int> v = {1, 2, 3};
  v.pop_back();
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v.back(), 2);
}

TEST(VectorTest, PopBack_EmptyVector) {
  s21::vector<int> v;
  // Popping from an empty vector should not cause an error (undefined
  // behavior
  // for some std::vector implementations)
  EXPECT_ANY_THROW(v.pop_back());
  EXPECT_TRUE(v.empty());
}

TEST(VectorTest, Swap_Basic) {
  s21::vector<int> v1 = {1, 2, 3};
  s21::vector<int> v2 = {4, 5};

  v1.swap(v2);

  EXPECT_EQ(v1.size(), 2);
  EXPECT_EQ(v2.size(), 3);
  EXPECT_EQ(v1[0], 4);
  EXPECT_EQ(v2[0], 1);
}

TEST(VectorTest, Swap_Empty) {
  s21::vector<int> v1;
  s21::vector<int> v2 = {1, 2, 3};

  v1.swap(v2);

  EXPECT_TRUE(v2.empty());
  EXPECT_EQ(v1.size(), 3);
}

TEST(VectorTest, Swap_Self) {
  s21::vector<int> v = {1, 2, 3};
  v.swap(v);  // Swap с самим собой

  EXPECT_EQ(v.size(), 3);  // Должно остаться неизменным
}

TEST(VectorTest, Swap_NonEmptyVectors) {
  s21::vector<int> v1 = {1, 2, 3};
  s21::vector<int> v2 = {4, 5, 6};

  v1.swap(v2);

  EXPECT_EQ(v1.size(), 3);
  EXPECT_EQ(v2.size(), 3);

  EXPECT_EQ(v1[0], 4);
  EXPECT_EQ(v1[2], 6);

  EXPECT_EQ(v2[0], 1);
  EXPECT_EQ(v2[2], 3);
}

TEST(VectorTest, Swap_EmptyAndNonEmptyVectors) {
  s21::vector<int> v1;
  s21::vector<int> v2 = {1, 2, 3};

  v1.swap(v2);

  EXPECT_EQ(v1.size(), 3);
  EXPECT_FALSE(v1.empty());
  EXPECT_EQ(v2.size(), 0);
  EXPECT_TRUE(v2.empty());
}

// TEST(VectorTest, InsertMany) {
//   s21::vector<int> vec = {1, 2, 3, 7, 8};
//   s21::vector<int>::const_iterator pos = vec.cbegin() + 3;

//   vec.insert_many(pos, 4, 5, 6);

//   // Check the size of the vector after insertion
//   EXPECT_EQ(vec.size(), 8);

//   // Check the elements after insertion
//   EXPECT_EQ(vec[0], 1);
//   EXPECT_EQ(vec[1], 2);
//   EXPECT_EQ(vec[2], 3);
//   EXPECT_EQ(vec[3], 4);
//   EXPECT_EQ(vec[4], 5);
//   EXPECT_EQ(vec[5], 6);
//   EXPECT_EQ(vec[6], 7);
//   EXPECT_EQ(vec[7], 8);
// }

// TEST(VectorTest, InsertManyBack) {
//   s21::vector<int> vec = {1, 2, 3};

//   vec.insert_many_back(4, 5, 6);

//   // Check the size of the vector after insertion
//   EXPECT_EQ(vec.size(), 6);

//   // Check the elements after insertion
//   EXPECT_EQ(vec[0], 1);
//   EXPECT_EQ(vec[1], 2);
//   EXPECT_EQ(vec[2], 3);
//   EXPECT_EQ(vec[3], 4);
//   EXPECT_EQ(vec[4], 5);
//   EXPECT_EQ(vec[5], 6);
// }

TEST(VectorTest, IteratorEq) {
  s21::vector<int> v2 = {1, 2, 3};

  auto it1 = v2.begin();
  auto it2 = v2.end();

  EXPECT_EQ((it1 != it2), true);
}

// ЛУЧШИЕ ТЕСТЫ В МИРЕ
struct C {
  inline static size_t created = 0;
  inline static size_t destroyed = 0;

  C() { created++; }
  C(const C& other) { other.created++; }
  ~C() { destroyed++; }

  C& operator=(const C& other) {
    if (this != &other) {
      other.created++;
    }
    return *this;
  }
};

struct TooManyObjectsException {};

struct C2 {
  inline static size_t created = 0;
  inline static size_t destroyed = 0;

  C2() {
    created++;
    if (created >= 5) throw TooManyObjectsException();
  }

  C2(const C& other) {
    other.created++;
    if (created >= 5) throw TooManyObjectsException();
  }

  ~C2() { destroyed++; }

  C2& operator=(const C2& other) {
    if (this != &other) {
      other.created++;
      if (created >= 5) throw TooManyObjectsException();
    }
    return *this;
  }
};

TEST(VectorTest, operator_assigment) {
  C::created = 0;
  C::destroyed = 0;

  s21::vector<C> v1(3), v2(4);

  v1 = v2;

  EXPECT_EQ((C::created == 11), true);
}

TEST(VectorTest, operator_assigment_2) {
  EXPECT_EQ((C::destroyed == 11), true);

  C::created = 0;
  s21::vector<C> v1(3), v2(4);

  v2 = v1;

  EXPECT_EQ((C::created == 10), true);
}

TEST(VectorTest, memory_leak) {
  try {
    s21::vector<C2>(10);
  } catch (...) {
  }
}