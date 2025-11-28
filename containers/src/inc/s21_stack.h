#ifndef __S21_STACK_H__
#define __S21_STACK_H__

#include "s21_list.h"

namespace s21 {

template <typename T>
class stack {
 private:
  s21::list<T> data_;

 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;

  stack() : data_() {}
  stack(std::initializer_list<value_type> const &items) : data_(items) {}
  stack(const stack &s) : data_(s.data_) {}
  stack(stack &&s) noexcept : data_(std::move(s.data_)) {}
  ~stack() = default;

  stack &operator=(const stack &other) {
    data_ = other.data_;
    return *this;
  }
  stack &operator=(stack &&other) noexcept {
    data_ = std::move(other.data_);
    return *this;
  }

  reference top() { return data_.back(); }
  const_reference top() const { return data_.back(); }

  bool empty() const { return data_.empty(); }
  size_type size() const { return data_.size(); }

  void push(const_reference value) { data_.push_back(value); }
  void pop() { data_.pop_back(); }
  void swap(stack &other) { data_.swap(other.data_); }
};

}  // namespace s21

#endif  // __S21_STACK_H__
