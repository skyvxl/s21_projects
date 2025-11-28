#include <memory>

#include "../inc/s21_vector.h"

// * правильный подход ерализации .tpp файлов
using namespace s21;

#pragma region CONSTRUCTORS

// uninitialized_value_construct_n
template <typename T>
vector<T>::vector(size_t n) : data_(n) {
  std::uninitialized_value_construct_n(data_.buf, n);
  sz_ = n;
}

// * initializer list constructor
template <typename T>
vector<T>::vector(std::initializer_list<value_type> const &items)
    : data_(items.size()) {
  std::uninitialized_move_n(items.begin(), items.size(), data_.buf);
  sz_ = items.size();
};

// * copy constructor
template <typename T>
vector<T>::vector(const vector &other) : data_(other.sz_) {
  std::uninitialized_copy_n(other.data_.buf, other.sz_, data_.buf);
  sz_ = other.sz_;
}

// * конструктор из диапазона
template <typename T>
vector<T>::vector(typename vector<T>::iterator first,
                  typename vector<T>::iterator last)
    : data_(std::distance(first, last)) {
  size_type n = std::distance(first, last);
  std::uninitialized_copy_n(first, n, data_.buf);

  sz_ = n;
}

template <typename T>
void vector<T>::swap(vector &other) noexcept {
  data_.swap(other.data_);
  std::swap(sz_, other.sz_);
}

// * operator=
// базовая гарантия безопасности в угоду
// производительности (в соответсвии с STL)
template <typename T>
vector<T> &vector<T>::operator=(const vector &other) {
  // используем идиому copy and swap, если вектор стоящий справа от оператора =
  // заведомо больше чем наш, т.к. нам в любом случае понадобится реаллокация
  // памяти
  if (other.sz_ > data_.cp) {
    vector tmp(other);
    swap(tmp);
  } else {
    for (size_t i = 0; i < sz_ && i < other.sz_; ++i) {
      data_[i] = other.data_[i];
    }

    // случай 1 если размер левого вектора < размер правого: добавить новые
    // элементы в сырую память после скопированых
    if (sz_ < other.sz_) {
      std::uninitialized_copy_n(other.data_.buf + sz_, other.sz_ - sz_,
                                data_.buf + sz_);
    } else if (sz_ > other.sz_) {
      std::destroy_n(other.data_.buf + sz_, sz_ - other.sz_);
    }

    sz_ = other.sz_;
  }

  return *this;
}

// * move constructor
template <typename T>
vector<T>::vector(vector &&other) noexcept {
  swap(other);
}

template <typename T>
vector<T>::~vector() {
  std::destroy_n(data_.buf, sz_);
}

// * оператор перемещающего присваивания (move assignment operator)
template <typename T>
vector<T> &vector<T>::operator=(vector &&other) noexcept {
  if (this != &other) {
    vector<T> temp(std::move(other));
    this->swap(temp);
  }

  return *this;
}

template <typename T>
bool vector<T>::operator==(const vector &other) const {
  if (this == &other) return true;

  bool is_equal = true;

  if (sz_ != other.sz_) is_equal = false;

  return is_equal && std::equal(data_.buf, data_.buf + sz_, other.data_.buf);
}

#pragma endregion

#pragma region ELEMENT ACESS
// ? ELEMENT ACESS

template <typename T>
typename vector<T>::size_type vector<T>::size() const noexcept {
  return sz_;
}

template <typename T>
typename vector<T>::size_type vector<T>::max_size() const {
  return std::numeric_limits<size_type>::max() / sizeof(value_type);
}

template <typename T>
typename vector<T>::reference vector<T>::at(size_type pos) {
  if (pos >= sz_) throw std::out_of_range("vector::at: index out of range");

  return data_.buf[pos];
}

template <typename T>
typename vector<T>::const_reference vector<T>::at(size_type pos) const {
  if (pos >= sz_) throw std::out_of_range("vector::at: index out of range");

  return data_.buf[pos];
}

template <typename T>
typename vector<T>::reference vector<T>::operator[](size_type pos) {
  return data_.buf[pos];
}

template <typename T>
typename vector<T>::const_reference vector<T>::operator[](size_type pos) const {
  return data_.buf[pos];
}

template <typename T>
typename vector<T>::reference vector<T>::front() {
  return data_.buf[0];
}

template <typename T>
typename vector<T>::const_reference vector<T>::front() const {
  return data_.buf[0];
}

template <typename T>
typename vector<T>::reference vector<T>::back() {
  return data_.buf[sz_ - 1];
}

template <typename T>
typename vector<T>::const_reference vector<T>::back() const {
  return data_.buf[sz_ - 1];
}

template <typename T>
typename vector<T>::iterator vector<T>::data() noexcept {
  return data_.buf;
}

template <typename T>
typename vector<T>::const_iterator vector<T>::data() const noexcept {
  return data_.buf;
}

// ? ELEMENT ACESS END
#pragma endregion

#pragma region ITERATORS

// ? ITERATORS

template <typename T>
typename vector<T>::iterator vector<T>::begin() noexcept {
  return data_.buf;
}

template <typename T>
typename vector<T>::iterator vector<T>::end() noexcept {
  return data_.buf + sz_;
}

template <typename T>
typename vector<T>::reverse_iterator vector<T>::rbegin() noexcept {
  return reverse_iterator(end());
}

template <typename T>
typename vector<T>::reverse_iterator vector<T>::rend() noexcept {
  return reverse_iterator(begin());
}

template <typename T>
typename vector<T>::const_iterator vector<T>::cbegin() const noexcept {
  return data_.buf;
}

template <typename T>
typename vector<T>::const_iterator vector<T>::cend() const noexcept {
  return data_.buf + sz_;
}

template <typename T>
typename vector<T>::const_reverse_iterator vector<T>::crbegin() const noexcept {
  return const_reverse_iterator(cend());
}

template <typename T>
typename vector<T>::const_reverse_iterator vector<T>::crend() const noexcept {
  return const_reverse_iterator(cbegin());
}

// ? ITERATORS END
#pragma endregion

#pragma region CAPACITY
// ? CAPACITY

template <typename T>
void vector<T>::reserve(size_type size) {
  if (size > max_size()) {
    throw std::length_error("vector::reserve: size exceeds max_size()");
  }

  reserve_more_capacity(size);
}

// сторгая гарантия безопасности, в случае ошибки исходынй вектор не будет
// изменён
template <typename T>
void vector<T>::reserve_more_capacity(size_t n) {
  if (n > data_.cp) {
    RawMemory<T> data_2(n);
    std::uninitialized_move_n(data_.buf, sz_, data_2.buf);
    std::destroy_n(data_.buf, sz_);
    data_ = std::move(data_2);
  }
}

// сторгая гарантия безопасности, в случае ошибки исходынй вектор не будет
// изменен
template <typename T>
void vector<T>::resize(size_t n) {
  reserve(n);

  if (sz_ < n) {
    std::uninitialized_value_construct_n(data_.buf + sz_, n - sz_);
  } else if (sz_ > n) {
    std::destroy_n(data_.buf + sz_, sz_ - n);
  }

  sz_ = n;
}

template <typename T>
bool vector<T>::empty() const noexcept {
  return sz_ == 0;
}

template <typename T>
typename vector<T>::size_type vector<T>::capacity() const noexcept {
  return data_.cp;
}

template <typename T>
void vector<T>::shrink_to_fit() {
  if (data_.cp > sz_) {
    RawMemory<T> data_2(sz_);
    std::uninitialized_move_n(data_.buf, sz_, data_2.buf);
    data_.swap(data_2);
  }
}

// ? CAPACITY END
#pragma endregion

#pragma region MODIFIERS
// ? MODIFIERS

template <typename T>
void vector<T>::clear() noexcept {
  // условие времени компиляции
  if constexpr (!std::is_trivially_destructible_v<value_type>) {
    for (auto it = begin(); it != end(); it++) it->~value_type();
  }

  sz_ = 0;
}

template <typename T>
typename vector<T>::iterator vector<T>::insert(iterator pos,
                                               const_reference value) {
  const size_type index = pos - begin();
  const T *value_ptr = &value;

  // Проверяем, не находится ли value внутри этого вектора
  const bool need_copy =
      (value_ptr >= data_.buf && value_ptr < data_.buf + sz_);
  T value_copy;
  if (need_copy) {
    value_copy = value;  // Копируем ДО reserve!
  }

  // Реаллокация (если нужно)
  if (sz_ + 1 > data_.cp) {
    const size_type new_capacity = data_.cp ? data_.cp * 2 : 1;
    reserve(new_capacity);
    pos = begin() + index;  // Обновляем pos после реаллокации
  }

  const T &val_to_insert = need_copy ? value_copy : value;

  if (pos != end()) {
    // 1. Создаём новый элемент в конце (сдвигаем последний элемент)
    new (data_.buf + sz_) T(std::move_if_noexcept(data_.buf[sz_ - 1]));

    // 2. Сдвигаем остальные элементы
    std::move_backward(pos, end() - 1, end());

    // 3. Уничтожаем старый элемент на позиции pos (если нужно)
    if constexpr (!std::is_trivially_destructible_v<value_type>) pos->~T();

    // 4. Конструируем новый элемент на месте pos
    new (data_.buf + index) T(val_to_insert);
  } else {
    // Вставка в конец — просто конструируем новый элемент
    new (data_.buf + sz_) T(val_to_insert);
  }

  ++sz_;
  return begin() + index;
}

template <typename T>
typename vector<T>::iterator vector<T>::insert(iterator pos, size_type n,
                                               const value_type &val) {
  if (n == 0) return pos;

  // Создаем копию значения ДО любых операций с памятью!
  value_type val_copy = val;

  const size_type index = pos - begin();
  const size_type new_size = sz_ + n;

  if (new_size > data_.cp) {
    size_type new_capacity = std::max(data_.cp * 2, new_size);
    reserve(new_capacity);
    pos = begin() + index;
  }

  if (pos != end()) {
    std::uninitialized_move(pos, end(), pos + n);
    std::destroy(pos, end());
  }

  // Используем копию значения вместо оригинальной ссылки
  std::uninitialized_fill_n(pos, n, val_copy);

  sz_ = new_size;
  return begin() + index;
}

template <typename T>
typename vector<T>::iterator vector<T>::insert(iterator pos, iterator first,
                                               iterator last) {
  if (first == last) return pos;

  const size_type n = std::distance(first, last);
  const size_type index = pos - begin();
  const size_type new_size = sz_ + n;

  // Создаем временную копию ДО любых манипуляций с памятью
  vector<value_type> temp(first, last);

  if (new_size > data_.cp) {
    size_type new_capacity = std::max(data_.cp * 2, new_size);
    reserve(new_capacity);
    pos = begin() + index;
  }

  if (pos != end()) {
    // Сдвигаем элементы используя uninitialized_move
    std::uninitialized_move(pos, end(), pos + n);
    // Уничтожаем оригиналы (они уже перемещены)
    std::destroy(pos, end());
  }

  // Копируем элементы из временного буфера
  std::uninitialized_copy(temp.begin(), temp.end(), pos);

  sz_ = new_size;
  return begin() + index;
}

template <typename T>
typename vector<T>::iterator vector<T>::erase(iterator pos) {
  if (pos == end()) return pos;  // удалять нечего

  // Сдвигаем элементы влево
  std::move(pos + 1, end(), pos);

  // Разрушаем последний элемент (он уже перемещён)
  std::destroy_at(data_.buf + (sz_ - 1));

  --sz_;
  return pos;
}

template <typename T>
typename vector<T>::iterator vector<T>::erase(iterator first, iterator last) {
  if (first == last) return first;

  size_type n = std::distance(first, last);

  // Сдвигаем хвост влево
  iterator new_end = std::move(last, end(), first);

  // Уничтожаем "висячие" элементы
  std::destroy_n(new_end, n);

  sz_ -= n;
  return first;
}

// сторгая гарантия безопасности, в случае ошибки исходынй
// вектор не будет изменен
template <typename T>
void vector<T>::push_back(const T &value) {
  if (sz_ == data_.cp) reserve(sz_ == 0 ? 1 : data_.cp * 2);

  new (data_.buf + sz_) T(value);
  ++sz_;
}

template <typename T>
void vector<T>::push_back(T &&value) {
  if (sz_ == data_.cp) reserve(sz_ == 0 ? 1 : data_.cp * 2);

  new (data_.buf + sz_) T(std::move(value));
  ++sz_;
}

template <typename T>
void vector<T>::pop_back() {
  if (sz_ == 0)
    throw std::out_of_range(
        "Index out of bounds: can't pop_back from empty vector");

  std::destroy_at(data_.buf + sz_ - 1);
  --sz_;
}

// ? MODIFIERS END
#pragma endregion

/*





*/