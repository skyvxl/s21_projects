#include "../inc/s21_list.h"

namespace s21 {

template <class T>
list<T>::list() : head_(nullptr), tail_(nullptr), size_(0) {
  init_empty_list();
}

template <class T>
list<T>::list(size_type n) : head_(nullptr), tail_(nullptr), size_(0) {
  init_empty_list();
  for (size_type i = 0; i < n; ++i) {
    push_back(T{});
  }
}

template <class T>
list<T>::list(std::initializer_list<value_type> const& items)
    : head_(nullptr), tail_(nullptr), size_(0) {
  init_empty_list();
  for (const auto& item : items) {
    push_back(item);
  }
}

template <class T>
list<T>::list(const list& l) : head_(nullptr), tail_(nullptr), size_(0) {
  init_empty_list();
  for (const auto& item : l) {
    push_back(item);
  }
}

template <class T>
list<T>::list(list&& l) noexcept
    : head_(l.head_), tail_(l.tail_), size_(l.size_) {
  l.head_ = nullptr;
  l.tail_ = nullptr;
  l.size_ = 0;
}

template <class T>
list<T>::~list() {
  free_list();
}

template <class T>
list<T>& list<T>::operator=(list&& other) noexcept {
  if (this != &other) {
    free_list();

    head_ = other.head_;
    tail_ = other.tail_;
    size_ = other.size_;
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }
  return *this;
}

template <class T>
list<T>& list<T>::operator=(const list& l) {
  if (this != &l) {
    list<T> temp(l);
    std::swap(head_, temp.head_);
    std::swap(tail_, temp.tail_);
    std::swap(size_, temp.size_);
  }
  return *this;
}

template <class T>
typename list<T>::reference list<T>::front() {
  if (empty()) throw std::out_of_range("List is empty");
  return head_->next->data;
}

template <class T>
typename list<T>::reference list<T>::back() {
  if (empty()) throw std::out_of_range("List is empty");
  return tail_->prev->data;
}

template <class T>
typename list<T>::const_reference list<T>::front() const {
  if (empty()) {
    throw std::out_of_range("List is empty");
  }
  return head_->next->data;
}

template <class T>
typename list<T>::const_reference list<T>::back() const {
  if (empty()) {
    throw std::out_of_range("List is empty");
  }
  return tail_->prev->data;
}

template <class T>
typename list<T>::iterator list<T>::begin() {
  return iterator(head_->next);
}

template <class T>
typename list<T>::iterator list<T>::end() {
  return iterator(tail_);
}

template <class T>
typename list<T>::const_iterator list<T>::begin() const {
  return const_iterator(head_->next);
}

template <class T>
typename list<T>::const_iterator list<T>::end() const {
  return const_iterator(tail_);
}

template <class T>
bool list<T>::empty() const {
  return size_ == 0;
}

template <class T>
typename list<T>::size_type list<T>::size() const {
  return size_;
}

template <class T>
typename list<T>::size_type list<T>::max_size() const {
  return std::numeric_limits<size_type>::max();
}

template <class T>
void list<T>::push_back(const_reference value) {
  Node* new_node = new Node(value);
  Node* last_node = tail_->prev;

  last_node->next = new_node;
  new_node->prev = last_node;
  new_node->next = tail_;
  tail_->prev = new_node;

  ++size_;
}

template <class T>
void list<T>::push_front(const_reference value) {
  Node* new_node = new Node(value);
  Node* first_node = head_->next;

  head_->next = new_node;
  new_node->prev = head_;
  new_node->next = first_node;
  first_node->prev = new_node;

  ++size_;
}

template <class T>
void list<T>::pop_front() {
  if (empty()) {
    throw std::out_of_range("List is empty");
  }
  Node* node_to_delete = head_->next;
  Node* new_first_node = node_to_delete->next;

  head_->next = new_first_node;
  new_first_node->prev = head_;

  delete node_to_delete;
  --size_;
}

template <class T>
void list<T>::pop_back() {
  if (empty()) {
    throw std::out_of_range("List is empty");
  }
  Node* node_to_delete = tail_->prev;
  Node* new_last_node = node_to_delete->prev;

  new_last_node->next = tail_;
  tail_->prev = new_last_node;

  delete node_to_delete;
  --size_;
}

template <class T>
typename list<T>::iterator list<T>::insert(iterator pos,
                                           const_reference value) {
  if (!pos.node_) {
    throw std::invalid_argument("Invalid iterator");
  }

  Node* new_node = new Node(value);
  Node* current = pos.node_;

  new_node->prev = current->prev;
  new_node->next = current;
  current->prev->next = new_node;
  current->prev = new_node;

  ++size_;
  return iterator(new_node);
}

template <class T>
typename list<T>::iterator list<T>::erase(iterator pos) {
  if (!pos.node_) {
    throw std::invalid_argument("Invalid iterator");
  }

  Node* current = pos.node_;
  Node* prev_node = current->prev;
  Node* next_node = current->next;

  prev_node->next = next_node;
  next_node->prev = prev_node;

  delete current;
  --size_;
  return iterator(next_node);
}

template <class T>
void list<T>::swap(list& other) {
  std::swap(head_, other.head_);
  std::swap(tail_, other.tail_);
  std::swap(size_, other.size_);
}

template <class T>
void list<T>::merge(list& other) {
  if (this == &other || other.empty()) {
    return;
  }
  Node* this_pos = head_->next;
  Node* other_pos = other.head_->next;

  while (other_pos != other.tail_) {
    while (this_pos != tail_ && this_pos->data <= other_pos->data) {
      this_pos = this_pos->next;
    }

    Node* node_to_move = other_pos;
    other_pos = other_pos->next;

    node_to_move->prev->next = node_to_move->next;
    node_to_move->next->prev = node_to_move->prev;

    Node* prev_this_pos = this_pos->prev;
    prev_this_pos->next = node_to_move;
    node_to_move->prev = prev_this_pos;
    node_to_move->next = this_pos;
    this_pos->prev = node_to_move;
  }

  size_ += other.size_;
  other.size_ = 0;
  other.head_->next = other.tail_;
  other.tail_->prev = other.head_;
}

template <class T>
void list<T>::splice(const_iterator pos, list& other) {
  if (this == &other || other.empty()) return;

  Node* first = other.head_->next;
  Node* last = other.tail_->prev;

  other.head_->next = other.tail_;
  other.tail_->prev = other.head_;

  Node* pos_node = const_cast<Node*>(pos.node_);
  Node* prev_node = pos_node->prev;

  prev_node->next = first;
  first->prev = prev_node;

  last->next = pos_node;
  pos_node->prev = last;

  size_ += other.size_;
  other.size_ = 0;
}

template <class T>
void list<T>::reverse() {
  if (size_ <= 1) return;

  iterator forward_it = begin();
  iterator backward_it = --end();

  for (size_type i = 0; i < size_ / 2; ++i) {
    swap_elements(forward_it, backward_it);
    ++forward_it;
    --backward_it;
  }
}

template <class T>
void list<T>::unique() {
  if (size() < 2) return;

  iterator current = begin();
  iterator next = current;
  ++next;

  while (next != end()) {
    if (*current == *next) {
      next = erase(next);
    } else {
      current = next;
      ++next;
    }
  }
}

template <class T>
void list<T>::sort() {
  if (size_ <= 1) return;
  for (size_type i = 0; i < size_; ++i) {
    iterator it = begin();
    for (size_type j = 0; j < size_ - 1; ++j) {
      iterator next_it = it;
      ++next_it;
      if (*it > *next_it) {
        swap_elements(it, next_it);
      }
      ++it;
    }
  }
}

template <class T>
void list<T>::clear() {
  if (head_) {
    Node* current = head_->next;
    while (current != tail_) {
      Node* next = current->next;
      delete current;
      current = next;
    }
    head_->next = tail_;
    tail_->prev = head_;
    size_ = 0;
  }
}

// ----------------- private -----------------

template <class T>
void list<T>::init_empty_list() {
  head_ = new Node();
  tail_ = new Node();
  head_->next = tail_;
  tail_->prev = head_;
}

template <class T>
void list<T>::free_list() {
  if (head_) {
    clear();
    delete head_;
    delete tail_;
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
  }
}

template <class T>
void list<T>::swap_elements(iterator pos1, iterator pos2) {
  std::swap(pos1.node_->data, pos2.node_->data);
}

}  // namespace s21