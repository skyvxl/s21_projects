#ifndef __S21_VECTOR_H___
#define __S21_VECTOR_H___

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <utility>
/*

HEADER FILE

*/

namespace s21 {

template <typename T>
struct RawMemory {
  T *buf = nullptr;
  size_t cp = 0;

  static T *Allocate(size_t n) {
    return static_cast<T *>(operator new(n * sizeof(T)));
  }

  static void Deallocate(T *buf) { operator delete(buf); }

  void swap(RawMemory &other) noexcept {
    std::swap(cp, other.cp);
    std::swap(buf, other.buf);
  }

  RawMemory() = default;

  RawMemory(size_t n) {
    buf = Allocate(n);
    cp = n;
  }

  RawMemory(const RawMemory &other) = delete;
  RawMemory &operator=(const RawMemory &other) = delete;

  RawMemory(RawMemory &&other) noexcept { Swap(other); }
  RawMemory &operator=(RawMemory &&other) noexcept {
    swap(other);
    return *this;
  }

  ~RawMemory() { Deallocate(buf); }

  const T *operator+(size_t n) const { return buf + n; }
  T *operator+(size_t n) { return buf + n; }

  const T &operator[](size_t n) const { return buf[n]; }
  T &operator[](size_t n) { return buf[n]; }
};

template <class T>
class vector {
  // * private свойства вектора
 private:
  RawMemory<T> data_;
  size_t sz_ = 0;

 public:
  // * Vector Member type
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using size_type = size_t;

  // * Vector public methods
 public:
#pragma region CONTRUCTORS
  // ? CONSTRUCTORS
  // * Vector default constructor
  vector() = default;

  // * Vector parametrized constructor for fixed size vector (explicit was used
  // * in order to avoid automatic type conversion)
  //  explicit чтобы чел не ошибся, думая что в таком коде:
  //  s21::vector<int> v = 5;
  //  он кладет в вектор 5, а не создает вектор из 5 элементов
  // *
  explicit vector(size_type n);

  // initializer list constructor (allows creating lists with initializer lists,
  // see main.cpp)
  //  поддерживает инициализацию вектора через {}
  //  Создание вектора с начальными значениями
  //  s21::vector<int> v1 = {1, 2, 3, 4, 5};
  //  s21::vector<int> v2{10, 20, 30};
  //  s21::vector<std::string> v3 = {"hello", "world", "!"};
  //  Или даже пустой список
  //  s21::vector<int> v4 = {};  // создает пустой вектор
  // *
  vector(std::initializer_list<value_type> const &items);

  // * конструктор копирования
  // *
  vector(const vector &v);

  vector(iterator first, iterator last);

  // * оператор присваивания
  vector &operator=(const vector &other);

  // * оператор перемещающего присваивания (move assignment operator)
  // s21::vector<int> a = {1, 2, 3};
  // s21::vector<int> b;
  // b = std::move(a);  // Быстрое перемещение, без копирования элементов!
  // *
  vector &operator=(vector &&v) noexcept;

  bool operator==(const vector &other) const;

  // move constructor
  // вызывается когда объект инициализируется из временного объекта
  // и при возврате из функции по значению (если компилятор может применить
  // move)
  // *
  vector(vector &&v) noexcept;

  // * destructor
  ~vector();
  // ? CONSTRUCTORS END
#pragma endregion

#pragma region ELEMENT_ACCESS
  // ? ELEMENT ACCESS
  // * operator at - element accessor with bounds checking
  reference at(size_type pos);              // *
  const_reference at(size_type pos) const;  // *
  // * operator [] - access specified element
  reference operator[](size_type pos);              // *
  const_reference operator[](size_type pos) const;  // *
  reference front();                                // *
  const_reference front() const;                    // *
  reference back();                                 // *
  const_reference back() const;                     // *
  T *data() noexcept;                               // *
  const T *data() const noexcept;                   // *
  // ? ELEMENT ACCESS END
#pragma endregion

#pragma region ITERATORS

  // ? ITERATORS

  iterator begin() noexcept;                        // *
  iterator end() noexcept;                          // *
  reverse_iterator rbegin() noexcept;               // *
  reverse_iterator rend() noexcept;                 // *
  const_iterator cbegin() const noexcept;           // *
  const_iterator cend() const noexcept;             // *
  const_reverse_iterator crbegin() const noexcept;  // *
  const_reverse_iterator crend() const noexcept;    // *

  // ? ITERATORS END
#pragma endregion

#pragma region CAPACITY
  // ? CAPACITY

  bool empty() const noexcept;          // *
  size_type size() const noexcept;      // *
  size_type max_size() const;           // *
  void reserve(size_type size);         // *
  void resize(size_t n);                // *
  size_type capacity() const noexcept;  // *
  void shrink_to_fit();                 // *

  // ? CAPACITY END
#pragma endregion

#pragma region MODIFIERS
  // ? MODIFIERS

  // очищает содержимое контейнера, но не высвобождает память
  // Все элементы контейнера удаляются
  // Размер(size()) становится 0.
  // Ёмкость(capacity()) у std::vector не изменяется
  // Контейнер остаётся валидным и его можно использовать дальше
  // Итераторы к элементам становятся недействительными.
  void clear() noexcept;  // *

  // вставляет элемент в позицию текущего итератора и возвращает итератор,
  // указывающий на новый элемент
  iterator insert(iterator pos, const_reference value);
  // перегрузка для вставки n копий элемента
  iterator insert(iterator position, size_type n, const value_type &val);
  // перегрузка для вставки диапазона итераторов
  iterator insert(iterator pos, iterator start, iterator end);
  // удаляет элемент по заданной позиции, возвращает итератор на элемент,
  // следующий за удаленным
  iterator erase(iterator pos);
  // удаляет элементы в интервале НЕ включая последний элемент last, возвращает
  // итератор на элемент, следующий за удаленным
  iterator erase(iterator first, iterator last);
  // добавляет передаваемый элемент в конец контейнера
  void push_back(const_reference value);
  void push_back(T &&value);
  // удаляет последний элемент контейнера
  void pop_back();
  // меняет содержимое текущего контейнера и контейнера other
  void swap(vector &other) noexcept;

  // ? MODIFIERS END
#pragma endregion

  // Простые и понятные функции
  size_t Size() { return sz_; }
  size_t Capasity() { return data_.cp; }

  const T *operator+(size_t n) const { return data_.buf + n; }
  T *operator+(size_t n) { return data_.buf + n; }

  const T *operator-(size_t n) const { return data_.buf - n; }
  T *operator-(size_t n) { return data_.buf - n; }
  // * vector private method
 private:
  void reserve_more_capacity(size_type size);
};

}  // namespace s21

#include "../s21_containers/s21_vector.tpp"

#endif
