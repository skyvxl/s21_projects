#include "../inc/s21_map.h"

namespace s21 {

template <typename Key, typename T>
map<Key, T>::map() : tree_() {}

template <typename Key, typename T>
map<Key, T>::map(std::initializer_list<value_type> const& items) : tree_() {
  for (const auto& item : items) {
    insert(item);
  }
}

template <typename Key, typename T>
map<Key, T>::map(const map& m) : tree_(m.tree_) {}

template <typename Key, typename T>
map<Key, T>::map(map&& m) noexcept : tree_(std::move(m.tree_)) {}

template <typename Key, typename T>
map<Key, T>& map<Key, T>::operator=(const map& m) {
  if (this != &m) {
    tree_ = m.tree_;
  }
  return *this;
}

template <typename Key, typename T>
map<Key, T>& map<Key, T>::operator=(map&& m) noexcept {
  if (this != &m) {
    tree_ = std::move(m.tree_);
  }
  return *this;
}

template <typename Key, typename T>
T& map<Key, T>::at(const Key& key) {
  node temp_node(key);
  auto tree_it = tree_.find(temp_node);

  if (tree_it == tree_.end()) {
    throw std::out_of_range("map::at: key not found");
  }

  return (*tree_it).data.second;
}

template <typename Key, typename T>
T& map<Key, T>::operator[](const Key& key) {
  node temp_node(key);
  auto tree_it = tree_.find(temp_node);

  if (tree_it == tree_.end()) {
    tree_.insert(temp_node);
    tree_it = tree_.find(temp_node);
  }

  return (*tree_it).data.second;
}

template <typename Key, typename T>
typename map<Key, T>::iterator map<Key, T>::begin() {
  return iterator(tree_.begin());
}

template <typename Key, typename T>
typename map<Key, T>::iterator map<Key, T>::end() {
  return iterator(tree_.end());
}

template <typename Key, typename T>
typename map<Key, T>::const_iterator map<Key, T>::begin() const {
  return const_iterator(tree_.begin());
}

template <typename Key, typename T>
typename map<Key, T>::const_iterator map<Key, T>::end() const {
  return const_iterator(tree_.end());
}

template <typename Key, typename T>
bool map<Key, T>::empty() const {
  return tree_.empty();
}

template <typename Key, typename T>
typename map<Key, T>::size_type map<Key, T>::size() const {
  return tree_.size();
}

template <typename Key, typename T>
typename map<Key, T>::size_type map<Key, T>::max_size() const {
  return tree_.max_size();
}

template <typename Key, typename T>
void map<Key, T>::clear() {
  tree_.clear();
}

template <typename Key, typename T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert(
    const value_type& value) {
  node temp_node(value.first, value.second);
  auto tree_it = tree_.find(temp_node);

  if (tree_it != tree_.end()) {
    return {iterator(tree_it), false};
  }

  tree_.insert(temp_node);
  tree_it = tree_.find(temp_node);
  return {iterator(tree_it), true};
}

template <typename Key, typename T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert(
    const Key& key, const T& obj) {
  return insert(value_type(key, obj));
}

template <typename Key, typename T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert_or_assign(
    const Key& key, const T& obj) {
  node temp_node(key);
  auto tree_it = tree_.find(temp_node);

  if (tree_it != tree_.end()) {
    (*tree_it).data.second = obj;
    return {iterator(tree_it), false};
  }

  tree_.insert(node(key, obj));
  tree_it = tree_.find(temp_node);
  return {iterator(tree_it), true};
}

template <typename Key, typename T>
void map<Key, T>::erase(iterator pos) {
  tree_.erase(pos.get_tree_iterator());
}

template <typename Key, typename T>
void map<Key, T>::swap(map& other) {
  tree_.swap(other.tree_);
}

template <typename Key, typename T>
void map<Key, T>::merge(map& other) {
  tree_.merge(other.tree_);
}

template <typename Key, typename T>
bool map<Key, T>::contains(const key_type& key) const {
  return tree_.find(node(key)) != tree_.end();
}

template <typename Key, typename T>
typename map<Key, T>::iterator map<Key, T>::find(const key_type& key) {
  node temp_node(key);
  auto tree_it = tree_.find(temp_node);
  return iterator(tree_it);
}

template <typename Key, typename T>
typename map<Key, T>::const_iterator map<Key, T>::find(
    const key_type& key) const {
  node temp_node(key);
  auto tree_it = tree_.find(temp_node);
  return const_iterator(tree_it);
}

}  // namespace s21
