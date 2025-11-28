#ifndef __S21_MAP__
#define __S21_MAP__

#include "s21_tree.h"

namespace s21 {

template <class Key, class T>
class map {
 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = std::size_t;

 private:
  struct node {
    value_type data;
    node(const key_type& k, const mapped_type& v) : data(k, v) {}
    node(const key_type& k) : data(k, mapped_type{}) {}

    bool operator<(const node& other) const {
      return data.first < other.data.first;
    }
    bool operator>(const node& other) const {
      return data.first > other.data.first;
    }
    bool operator==(const node& other) const {
      return data.first == other.data.first;
    }
  };

  tree<node, false> tree_;

 public:
  class MapIterator;
  class MapConstIterator;

  using iterator = MapIterator;
  using const_iterator = MapConstIterator;

  map();
  map(std::initializer_list<value_type> const& items);
  map(const map& m);
  map(map&& m) noexcept;
  ~map() = default;
  map& operator=(const map& m);
  map& operator=(map&& m) noexcept;

  T& at(const Key& key);
  T& operator[](const Key& key);

  iterator find(const key_type& key);
  const_iterator find(const key_type& key) const;

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

  bool empty() const;
  size_type size() const;
  size_type max_size() const;

  void clear();
  std::pair<iterator, bool> insert(const value_type& value);
  std::pair<iterator, bool> insert(const Key& key, const T& obj);
  std::pair<iterator, bool> insert_or_assign(const Key& key, const T& obj);
  void erase(iterator pos);
  void swap(map& other);
  void merge(map& other);

  bool contains(const key_type& key) const;

  class MapIterator {
   private:
    typename tree<node, false>::iterator it_;
    friend class MapConstIterator;

   public:
    MapIterator() : it_() {}
    MapIterator(typename tree<node, false>::iterator it) : it_(it) {}

    reference operator*() { return (*it_).data; }
    value_type* operator->() { return &((*it_).data); }

    MapIterator& operator++() {
      ++it_;
      return *this;
    }
    MapIterator operator++(int) {
      MapIterator temp = *this;
      ++it_;
      return temp;
    }

    MapIterator& operator--() {
      --it_;
      return *this;
    }
    MapIterator operator--(int) {
      MapIterator temp = *this;
      --it_;
      return temp;
    }

    bool operator!=(const MapIterator& other) const { return it_ != other.it_; }
    bool operator==(const MapIterator& other) const { return it_ == other.it_; }

    typename tree<node, false>::iterator get_tree_iterator() const {
      return it_;
    }
  };

  class MapConstIterator {
   private:
    typename tree<node, false>::const_iterator it_;

   public:
    MapConstIterator() : it_() {}
    MapConstIterator(typename tree<node, false>::const_iterator it) : it_(it) {}
    MapConstIterator(const MapIterator& other)
        : it_(other.get_tree_iterator()) {}

    const_reference operator*() const { return (*it_).data; }
    const value_type* operator->() const { return &((*it_).data); }

    MapConstIterator& operator++() {
      ++it_;
      return *this;
    }
    MapConstIterator operator++(int) {
      MapConstIterator temp = *this;
      ++it_;
      return temp;
    }

    MapConstIterator& operator--() {
      --it_;
      return *this;
    }
    MapConstIterator operator--(int) {
      MapConstIterator temp = *this;
      --it_;
      return temp;
    }

    bool operator!=(const MapConstIterator& other) const {
      return it_ != other.it_;
    }
    bool operator==(const MapConstIterator& other) const {
      return it_ == other.it_;
    }
  };
};

}  // namespace s21

#include "../s21_containers/s21_map.tpp"

#endif /* __S21_MAP__ */
