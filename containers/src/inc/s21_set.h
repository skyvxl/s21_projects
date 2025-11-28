#ifndef __S21_SET_H___
#define __S21_SET_H___

#include "s21_tree.h"

namespace s21 {
template <class Key>
class set {
 public:
  // ? set member type
  using key_type = Key;
  using value_type = Key;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = std::size_t;

 private:
  struct node {
    value_type data;
    node(const value_type& k) : data(k) {}

    bool operator<(const node& other) const { return data < other.data; }
    bool operator>(const node& other) const { return data > other.data; }
    bool operator==(const node& other) const { return data == other.data; }
  };

  tree<node, false> tree_;

 public:
  class SetIterator;
  class SetConstIterator;

  using iterator = SetIterator;
  using const_iterator = SetConstIterator;

  // ? CTORS
  set();                                                // *
  set(std::initializer_list<value_type> const& items);  // *
  set(const set& other);
  set(set&& other) noexcept;
  ~set() = default;
  set& operator=(const set& other);
  set& operator=(set&& other) noexcept;

  // ? ITERATORS
  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;

  // ? CAPACITY
  bool empty() const;
  size_type size() const;
  size_type max_size() const;

  // ? Modifiers
  void clear();
  std::pair<iterator, bool> insert(const value_type& value);  // *
  void erase(iterator pos);
  void swap(set& other);
  void merge(set& other);

  // ? Lookup
  iterator find(const key_type& key);
  const_iterator find(const key_type& key) const;
  bool contains(const key_type& key) const;

  class SetIterator {
   private:
    typename tree<node, false>::iterator it_;
    friend class SetConstIterator;

   public:
    SetIterator() : it_() {}
    SetIterator(typename tree<node, false>::iterator it) : it_(it) {}

    reference operator*() { return (*it_).data; }
    value_type* operator->() { return &((*it_).data); }

    SetIterator& operator++() {
      ++it_;
      return *this;
    }

    SetIterator operator++(int) {
      SetIterator temp = *this;
      ++it_;
      return temp;
    }

    SetIterator& operator--() {
      --it_;
      return *this;
    }

    SetIterator operator--(int) {
      SetIterator temp = *this;
      --it_;
      return temp;
    }

    bool operator!=(const SetIterator& other) const { return it_ != other.it_; }
    bool operator==(const SetIterator& other) const { return it_ == other.it_; }

    typename tree<node, false>::iterator get_tree_iterator() const {
      return it_;
    }
  };

  class SetConstIterator {
   private:
    typename tree<node, false>::const_iterator it_;

   public:
    SetConstIterator() : it_() {}
    SetConstIterator(typename tree<node, false>::const_iterator it) : it_(it) {}
    SetConstIterator(const SetIterator& other)
        : it_(other.get_tree_iterator()) {}

    const_reference operator*() const { return (*it_).data; }
    const value_type* operator->() const { return &((*it_).data); }

    SetConstIterator& operator++() {
      ++it_;
      return *this;
    }

    SetConstIterator operator++(int) {
      SetConstIterator temp = *this;
      ++it_;
      return temp;
    }

    SetConstIterator& operator--() {
      --it_;
      return *this;
    }

    SetConstIterator operator--(int) {
      SetConstIterator temp = *this;
      --it_;
      return temp;
    }

    bool operator!=(const SetConstIterator& other) const {
      return it_ != other.it_;
    }

    bool operator==(const SetConstIterator& other) const {
      return it_ == other.it_;
    }
  };
};

#include "../s21_containers/s21_set.tpp"

}  // namespace s21

#endif