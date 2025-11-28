#include "../inc/s21_tree.h"

namespace s21 {

template <typename T, bool AllowDuplicates>
tree<T, AllowDuplicates>::tree() : parent_(nullptr), size_(0) {}

template <typename T, bool AllowDuplicates>
tree<T, AllowDuplicates>::tree(const tree &other)
    : parent_(copy(other.parent_)), size_(other.size_) {}

template <typename T, bool AllowDuplicates>
tree<T, AllowDuplicates>::tree(tree &&other) noexcept
    : parent_(other.parent_), size_(other.size_) {
  other.parent_ = nullptr;
  other.size_ = 0;
}

template <typename T, bool AllowDuplicates>
tree<T, AllowDuplicates>::tree(
    std::initializer_list<tree_data_type> const &items)
    : parent_(nullptr), size_(0) {
  for (const auto &item : items) {
    insert(item);
  }
}

template <typename T, bool AllowDuplicates>
tree<T, AllowDuplicates> &tree<T, AllowDuplicates>::operator=(
    const tree &other) {
  if (this != &other) {
    tree<T, AllowDuplicates> temp(other);
    std::swap(parent_, temp.parent_);
    std::swap(size_, temp.size_);
  }
  return *this;
}

template <typename T, bool AllowDuplicates>
tree<T, AllowDuplicates> &tree<T, AllowDuplicates>::operator=(
    tree &&other) noexcept {
  if (this != &other) {
    clear();
    parent_ = other.parent_;
    size_ = other.size_;
    other.parent_ = nullptr;
    other.size_ = 0;
  }
  return *this;
}

template <typename T, bool AllowDuplicates>
tree<T, AllowDuplicates> &tree<T, AllowDuplicates>::operator=(
    std::initializer_list<tree_data_type> const &items) {
  clear(parent_);
  parent_ = nullptr;
  size_ = 0;
  for (const auto &item : items) {
    insert(item);
  }
  return *this;
}

template <typename T, bool AllowDuplicates>
tree<T, AllowDuplicates>::~tree() {
  clear(parent_);
}

template <typename T, bool AllowDuplicates>
void tree<T, AllowDuplicates>::insert(tree_data_type data) {
  bool inserted = false;
  parent_ = insertHelper(data, parent_, nullptr, inserted);
  if (inserted) size_++;
}

template <typename T, bool AllowDuplicates>
typename tree<T, AllowDuplicates>::TreeIterator
tree<T, AllowDuplicates>::begin() {
  node *cur = parent_;
  if (!cur) return TreeIterator(nullptr, this);
  while (cur->left) cur = cur->left;
  return TreeIterator(cur, this);
}

template <typename T, bool AllowDuplicates>
typename tree<T, AllowDuplicates>::TreeConstIterator
tree<T, AllowDuplicates>::begin() const {
  const node *cur = parent_;
  if (!cur) return TreeConstIterator(nullptr);
  while (cur->left) cur = cur->left;
  return TreeConstIterator(cur);
}

template <typename T, bool AllowDuplicates>
typename tree<T, AllowDuplicates>::TreeIterator
tree<T, AllowDuplicates>::end() {
  return TreeIterator(nullptr, this);
}

template <typename T, bool AllowDuplicates>
typename tree<T, AllowDuplicates>::TreeConstIterator
tree<T, AllowDuplicates>::end() const {
  return TreeConstIterator(nullptr);
}

template <typename T, bool AllowDuplicates>
typename tree<T, AllowDuplicates>::TreeIterator tree<T, AllowDuplicates>::find(
    tree_data_type data) {
  return TreeIterator(findHelper(data, parent_), this);
}

template <typename T, bool AllowDuplicates>
typename tree<T, AllowDuplicates>::TreeConstIterator
tree<T, AllowDuplicates>::find(tree_data_type data) const {
  return findHelper(data, parent_);
}

template <typename T, bool AllowDuplicates>
void tree<T, AllowDuplicates>::erase(TreeIterator pos) {
  parent_ = eraseHelper(parent_, *pos);
  --size_;
}

template <typename T, bool AllowDuplicates>
void tree<T, AllowDuplicates>::clear() {
  clear(parent_);
  parent_ = nullptr;
  size_ = 0;
}

template <typename T, bool AllowDuplicates>
typename tree<T, AllowDuplicates>::size_type tree<T, AllowDuplicates>::size()
    const {
  return size_;
}

template <typename T, bool AllowDuplicates>
bool tree<T, AllowDuplicates>::empty() const {
  return size_ == 0;
}

template <typename T, bool AllowDuplicates>
typename tree<T, AllowDuplicates>::size_type
tree<T, AllowDuplicates>::max_size() const {
  return std::numeric_limits<size_type>::max() / sizeof(node);
}

template <typename T, bool AllowDuplicates>
void tree<T, AllowDuplicates>::merge(tree &other) {
  if (this == &other || other.empty()) return;
  for (const auto &item : other) {
    this->insert(item);
  }
  other.clear();
}

template <typename T, bool AllowDuplicates>
void tree<T, AllowDuplicates>::swap(tree &other) {
  std::swap(parent_, other.parent_);
  std::swap(size_, other.size_);
}

// ----------------- private -----------------

template <typename T, bool AllowDuplicates>
typename tree<T, AllowDuplicates>::node *tree<T, AllowDuplicates>::insertHelper(
    tree_data_type data, node *current, node *parent, bool &inserted) {
  if (current == nullptr) {
    inserted = true;
    return new node(data, nullptr, nullptr, parent);
  }
  if (data < current->data) {
    current->left = insertHelper(data, current->left, current, inserted);
  } else if (data > current->data) {
    current->right = insertHelper(data, current->right, current, inserted);
  } else if constexpr (AllowDuplicates) {
    current->right = insertHelper(data, current->right, current, inserted);
  }
  return current;
}

template <typename T, bool AllowDuplicates>
typename tree<T, AllowDuplicates>::node *tree<T, AllowDuplicates>::findHelper(
    tree_data_type data, node *current) {
  if (!current) return nullptr;
  if (data < current->data) {
    return findHelper(data, current->left);
  } else if (data > current->data) {
    return findHelper(data, current->right);
  } else {
    return current;
  }
}

template <typename T, bool AllowDuplicates>
typename tree<T, AllowDuplicates>::node *tree<T, AllowDuplicates>::findHelper(
    tree_data_type data, node *current) const {
  if (!current) return nullptr;
  if (data < current->data) {
    return findHelper(data, current->left);
  } else if (data > current->data) {
    return findHelper(data, current->right);
  } else {
    return current;
  }
}

template <typename T, bool AllowDuplicates>
typename tree<T, AllowDuplicates>::node *tree<T, AllowDuplicates>::eraseHelper(
    node *current, tree_data_type data) {
  if (!current) return nullptr;

  if (data < current->data) {
    current->left = eraseHelper(current->left, data);
    if (current->left) current->left->parent = current;
  } else if (data > current->data) {
    current->right = eraseHelper(current->right, data);
    if (current->right) current->right->parent = current;
  } else {
    if (!current->left) {
      node *right_child = current->right;
      if (right_child) right_child->parent = current->parent;
      delete current;
      return right_child;
    } else if (!current->right) {
      node *left_child = current->left;
      if (left_child) left_child->parent = current->parent;
      delete current;
      return left_child;
    } else {
      node *succ = current->right;
      while (succ->left) {
        succ = succ->left;
      }
      current->data.~tree_data_type();
      new (&current->data) tree_data_type(succ->data);
      current->right = eraseHelper(current->right, succ->data);
      if (current->right) current->right->parent = current;
    }
  }
  return current;
}

template <typename T, bool AllowDuplicates>
typename tree<T, AllowDuplicates>::node *tree<T, AllowDuplicates>::copy(
    node *current, node *parent) {
  if (!current) return nullptr;
  node *new_node = new node(current->data, nullptr, nullptr, parent);
  new_node->left = copy(current->left, new_node);
  new_node->right = copy(current->right, new_node);
  return new_node;
}

template <typename T, bool AllowDuplicates>
void tree<T, AllowDuplicates>::clear(node *current) {
  if (current) {
    clear(current->left);
    clear(current->right);
    delete current;
    current = nullptr;
  }
}

}  // namespace s21
