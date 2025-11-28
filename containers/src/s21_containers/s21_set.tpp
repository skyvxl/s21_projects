#include "../inc/s21_set.h"

using namespace s21;

// ? Constructors
template <typename T>
set<T>::set() : tree_() {}

template <typename T>
set<T>::set(std::initializer_list<value_type> const& items) : tree_() {
  for (const auto& item : items) {
    insert(item);
  }
}

template <typename T>
set<T>::set(const set& other) : tree_(other.tree_) {}

template <typename T>
set<T>::set(set&& other) noexcept : tree_(std::move(other.tree_)) {}

template <typename T>
set<T>& set<T>::operator=(const set<T>& other) {
  if (this != &other) {
    tree_ = other.tree_;
  }
  return *this;
}

template <typename T>
set<T>& set<T>::operator=(set<T>&& other) noexcept {
  if (this != &other) {
    tree_ = std::move(other.tree_);
  }
  return *this;
}

// ? Iterators

template <typename T>
typename set<T>::iterator set<T>::begin() {
  return iterator(tree_.begin());
}

template <typename T>
typename set<T>::const_iterator set<T>::begin() const {
  return const_iterator(tree_.begin());
}

template <typename T>
typename set<T>::iterator set<T>::end() {
  return iterator(tree_.end());
}

template <typename T>
typename set<T>::const_iterator set<T>::end() const {
  return const_iterator(tree_.end());
}

// ? Capacity

template <typename T>
bool set<T>::empty() const {
  return tree_.empty();
}

template <typename T>
typename set<T>::size_type set<T>::size() const {
  return tree_.size();
}

template <typename T>
typename set<T>::size_type set<T>::max_size() const {
  return tree_.max_size();
}

// ? Modifiers

template <typename T>
void set<T>::clear() {
  tree_.clear();
}

template <typename T>
std::pair<typename set<T>::iterator, bool> set<T>::insert(
    const value_type& value) {
  node temp_node(value);
  auto tree_it = tree_.find(temp_node);

  if (tree_it != tree_.end()) {
    return {iterator(tree_it), false};
  }

  tree_.insert(temp_node);
  tree_it = tree_.find(temp_node);
  return {iterator(tree_it), true};
}

template <class T>
void set<T>::erase(iterator pos) {
  if (pos == end()) {
    return;
  }

  tree_.erase(pos.get_tree_iterator());
}

template <typename T>
void set<T>::swap(set& other) {
  tree_.swap(other.tree_);
}

template <typename T>
void set<T>::merge(set& other) {
  tree_.merge(other.tree_);
}

template <typename T>
bool set<T>::contains(const key_type& key) const {
  return tree_.find(node(key)) != tree_.end();
}

template <typename T>
typename set<T>::iterator set<T>::find(const key_type& key) {
  node temp_node(key);
  auto tree_it = tree_.find(temp_node);
  return iterator(tree_it);
}

template <typename T>
typename set<T>::const_iterator set<T>::find(const key_type& key) const {
  node temp_node(key);
  auto tree_it = tree_.find(temp_node);
  return const_iterator(tree_it);
}