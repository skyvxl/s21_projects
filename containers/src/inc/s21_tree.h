#ifndef __S21_TREE__
#define __S21_TREE__

#include <iostream>
#include <limits>
#include <utility>

namespace s21 {

template <typename T, bool AllowDuplicates = false>
class tree {
 private:
  struct node;

 public:
  using tree_data_type = T;
  using size_type = size_t;

  tree();
  tree(const tree &other);
  tree(tree &&other) noexcept;
  tree(std::initializer_list<tree_data_type> const &items);
  tree &operator=(const tree &other);
  tree &operator=(tree &&other) noexcept;
  tree &operator=(std::initializer_list<tree_data_type> const &items);
  ~tree();
  void insert(tree_data_type data);

  class TreeIterator {
   private:
    node *current;
    const tree *tree_ptr;
    friend class TreeConstIterator;

   public:
    TreeIterator(node *node, const tree *tree_p = nullptr)
        : current(node), tree_ptr(tree_p){};
    TreeIterator() : current(nullptr), tree_ptr(nullptr) {}
    tree_data_type &operator*() { return current->data; };
    TreeIterator &operator++() {
      if (!current) return *this;
      if (current->right) {
        current = current->right;
        while (current->left) current = current->left;
      } else {
        node *p = current->parent;
        while (p && current == p->right) {
          current = p;
          p = p->parent;
        }
        current = p;
      }
      return *this;
    }

    TreeIterator &operator--() {
      if (!current) {
        if (tree_ptr && tree_ptr->parent_) {
          current = tree_ptr->parent_;
          while (current->right) current = current->right;
        }
        return *this;
      }
      if (current->left) {
        current = current->left;
        while (current->right) current = current->right;
      } else {
        node *p = current->parent;
        while (p && current == p->left) {
          current = p;
          p = p->parent;
        }
        current = p;
      }
      return *this;
    }
    TreeIterator &go_left() {
      current = current->left;
      return *this;
    };
    TreeIterator &go_right() {
      current = current->right;
      return *this;
    }

    bool operator==(const TreeIterator &other) const {
      return current == other.current;
    }
    bool operator!=(const TreeIterator &other) const {
      return current != other.current;
    }

    bool operator==(const std::nullptr_t &) const { return current == nullptr; }
    bool operator!=(const std::nullptr_t &) const { return current != nullptr; }

    const node *get_node() const { return current; }
  };

  class TreeConstIterator {
   private:
    const node *current;

   public:
    TreeConstIterator(const node *node) : current(node){};
    TreeConstIterator(const TreeIterator &other) : current(other.get_node()){};
    const tree_data_type &operator*() const { return current->data; };
    TreeConstIterator &operator++() {
      if (!current) return *this;
      if (current->right) {
        current = current->right;
        while (current->left) current = current->left;
      } else {
        const node *p = current->parent;
        while (p && current == p->right) {
          current = p;
          p = p->parent;
        }
        current = p;
      }
      return *this;
    }

    TreeConstIterator &operator--() {
      if (!current) return *this;
      if (current->left) {
        current = current->left;
        while (current->right) current = current->right;
      } else {
        const node *p = current->parent;
        while (p && current == p->left) {
          current = p;
          p = p->parent;
        }
        current = p;
      }
      return *this;
    }

    bool operator==(const TreeConstIterator &other) const {
      return current == other.current;
    }
    bool operator!=(const TreeConstIterator &other) const {
      return current != other.current;
    }

    bool operator==(const std::nullptr_t &) const { return current == nullptr; }
    bool operator!=(const std::nullptr_t &) const { return current != nullptr; }
  };

  using iterator = TreeIterator;
  using const_iterator = TreeConstIterator;

  TreeIterator begin();
  TreeConstIterator begin() const;
  TreeIterator end();
  TreeConstIterator end() const;
  TreeIterator find(tree_data_type data);
  TreeConstIterator find(tree_data_type data) const;
  void erase(TreeIterator pos);
  void clear();
  size_type size() const;
  bool empty() const;
  size_type max_size() const;
  void merge(tree &other);
  void swap(tree &other);

 private:
  struct node {
    tree_data_type data;
    node *left;
    node *right;
    node *parent;
    node(tree_data_type d, node *l = nullptr, node *r = nullptr,
         node *p = nullptr)
        : data(d), left(l), right(r), parent(p){};
  };
  node *parent_;
  size_type size_;

  node *insertHelper(tree_data_type data, node *current, node *parent,
                     bool &inserted);
  node *findHelper(tree_data_type data, node *current);
  node *findHelper(tree_data_type data, node *current) const;
  node *eraseHelper(node *current, tree_data_type data);
  node *copy(node *current, node *parent = nullptr);
  void clear(node *current);
};

}  // namespace s21

#include "../s21_containers/s21_tree.tpp"
#endif /* __S21_TREE__ */
