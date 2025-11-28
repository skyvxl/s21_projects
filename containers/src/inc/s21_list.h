#ifndef __S21_LIST_H__
#define __S21_LIST_H__

#include <limits>
#include <stdexcept>
#include <utility>

namespace s21 {

template <class T>
class list {
 private:
  struct Node {
    T data;
    Node* next;
    Node* prev;

    explicit Node(const T& value = T{})
        : data(value), next(nullptr), prev(nullptr) {}
    explicit Node(T&& value)
        : data(std::move(value)), next(nullptr), prev(nullptr) {}
  };

  Node* head_;
  Node* tail_;
  size_t size_;

 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  class ListIterator {
   private:
    Node* node_;
    friend class list<T>;
    friend class ListConstIterator;

   public:
    explicit ListIterator(Node* node = nullptr) : node_(node) {}

    reference operator*() const {
      if (!node_ || !node_->next || !node_->prev) {
        throw std::invalid_argument("Iterator cannot be dereferenced");
      }
      return node_->data;
    }

    ListIterator& operator++() {
      if (node_) {
        node_ = node_->next;
      }
      return *this;
    }

    ListIterator operator++(int) {
      ListIterator temp = *this;
      ++(*this);
      return temp;
    }

    ListIterator operator--() {
      if (node_) {
        node_ = node_->prev;
      }
      return *this;
    }

    ListIterator operator--(int) {
      ListIterator temp = *this;
      --(*this);
      return temp;
    }

    bool operator==(const ListIterator& other) const {
      return node_ == other.node_;
    }

    bool operator!=(const ListIterator& other) const {
      return node_ != other.node_;
    }
  };

  class ListConstIterator {
   private:
    Node* node_;
    friend class list<T>;

   public:
    explicit ListConstIterator(Node* node = nullptr) : node_(node) {}

    ListConstIterator(const ListIterator& other) : node_(other.node_) {}

    const_reference operator*() const {
      if (!node_ || !node_->next || !node_->prev) {
        throw std::invalid_argument("Iterator cannot be dereferenced");
      }
      return node_->data;
    }

    ListConstIterator& operator++() {
      if (node_) {
        node_ = node_->next;
      }
      return *this;
    }

    ListConstIterator operator++(int) {
      ListConstIterator temp = *this;
      ++(*this);
      return temp;
    }

    ListConstIterator operator--() {
      if (node_) {
        node_ = node_->prev;
      }
      return *this;
    }

    ListConstIterator operator--(int) {
      ListConstIterator temp = *this;
      --(*this);
      return temp;
    }

    bool operator==(const ListConstIterator& other) const {
      return node_ == other.node_;
    }

    bool operator!=(const ListConstIterator& other) const {
      return node_ != other.node_;
    }
  };

  using iterator = ListIterator;
  using const_iterator = ListConstIterator;

  list();
  explicit list(size_type n);
  list(std::initializer_list<value_type> const& items);
  list(const list& l);
  list(list&& l) noexcept;
  ~list();
  list& operator=(list&& l) noexcept;
  list& operator=(const list& l);

  reference front();
  reference back();
  const_reference front() const;
  const_reference back() const;

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

  bool empty() const;
  size_type size() const;
  size_type max_size() const;

  void push_front(const_reference value);
  void push_back(const_reference value);
  void pop_front();
  void pop_back();

  iterator insert(iterator pos, const_reference value);
  iterator erase(iterator pos);
  void swap(list& other);
  void merge(list& other);
  void splice(const_iterator pos, list& other);
  void reverse();
  void unique();
  void sort();
  void clear();

 private:
  void init_empty_list();
  void free_list();
  void swap_elements(iterator pos1, iterator pos2);
};

};  // namespace s21

#include "../s21_containers/s21_list.tpp"

#endif  // __S21_LIST_H__