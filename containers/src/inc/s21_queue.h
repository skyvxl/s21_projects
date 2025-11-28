#ifndef __S21_QUEUE_H___
#define __S21_QUEUE_H___

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <utility>

#include "s21_list.h"

/*

HEADER FILE

*/

namespace s21 {

template <typename T, typename Container = s21::list<T>>
class queue {
 private:
  Container c;

 public:
  // ? Queue Member type
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;

  // ? CONSTURCTORS
  queue() = default;
  queue(std::initializer_list<value_type> const &items) : c(items) {}
  queue(const queue &other) : c(other.c) {}
  queue(const Container &cont) : c(cont) {}
  queue(queue &&other) noexcept : c(std::move(other.c)) {}
  queue(Container &&cont) : c(std::move(cont)) {}
  ~queue() = default;
  queue &operator=(queue &&other) {
    if (this != &other) {
      c = std::move(other.c);
    }
    return *this;
  }

  queue &operator=(const queue &other) noexcept {
    if (this != &other) {
      c = other.c;
    }
    return *this;
  }

  // ? Element Access
  const_reference front() const { return c.front(); }
  const_reference back() const { return c.back(); };

  // ? Capacity
  bool empty() const { return c.empty(); }
  size_type size() const { return c.size(); }

  // ? Modifiers
  void push(const_reference value) { c.push_back(value); }
  void pop() { c.pop_front(); }
  void swap(queue &other) noexcept { std::swap(c, other.c); };

  bool operator==(const queue &other) const {
    if (size() != other.size()) return false;

    queue tmp1(*this);
    queue tmp2(other);

    while (!tmp1.empty()) {
      if (tmp1.front() != tmp2.front()) return false;
      tmp1.pop();
      tmp2.pop();
    }
    return true;
  }

  bool operator!=(const queue &other) const { return !(*this == other); }

  bool operator<(const queue &other) const {
    queue tmp1(*this);
    queue tmp2(other);

    while (!tmp1.empty() && !tmp2.empty()) {
      if (tmp1.front() < tmp2.front()) return true;
      if (tmp2.front() < tmp1.front()) return false;
      tmp1.pop();
      tmp2.pop();
    }
    return tmp1.empty() && !tmp2.empty();
  }

  bool operator<=(const queue &other) const { return !(other < *this); }

  bool operator>(const queue &other) const { return other < *this; }

  bool operator>=(const queue &other) const { return !(*this < other); }
};

}  // namespace s21

#endif