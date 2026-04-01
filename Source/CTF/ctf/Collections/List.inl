/*
 * Cereon Template Framework, a C++ 23 standard template library.
 * Copyright (c) 2026 The Aridity Team, all rights reserved.
 *
 * This file is part of the Cereon Template Framework project.
 * 
 * Cereon Template Framework is free software: you can redistribute
 * it and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation, either
 * version 3 of the License, or any later version.
 * 
 * Cereon Template Framework is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Cereon Template Framework. If not, see <https://www.gnu.org/licenses/>. 
 */

#include "CTF.h"
#include "List.h"

template <typename T> void CTF::Collections::List<T>::push_back(const T &val) {
  emplace_back(val);
}

template <typename T> void CTF::Collections::List<T>::push_back(T &&val) {
  emplace_back(std::move(val));
}

template <typename T>
template <typename... Args>
void CTF::Collections::List<T>::emplace_back(Args &&...args) {
  Node *n = new Node(std::forward<Args>(args)...);
  if (!head_)
    head_ = tail_ = n;
  else {
    tail_->next = n;
    n->prev = tail_;
    tail_ = n;
  }
  ++size_;
}

template <typename T> void CTF::Collections::List<T>::push_front(const T &val) {
  emplace_front(val);
}

template <typename T> void CTF::Collections::List<T>::push_front(T &&val) {
  emplace_front(std::move(val));
}

template <typename T>
template <typename... Args>
void CTF::Collections::List<T>::emplace_front(Args &&...args) {
  Node *n = new Node(std::forward<Args>(args)...);
  if (!head_)
    head_ = tail_ = n;
  else {
    n->next = head_;
    head_->prev = n;
    head_ = n;
  }
  ++size_;
}

template <typename T>
typename CTF::Collections::List<T>::Iterator
CTF::Collections::List<T>::erase(Iterator it) {
  Node *n = it.raw();
  if (!n)
    return end();
  Node *next = n->next;

  if (n->prev)
    n->prev->next = n->next;
  if (n->next)
    n->next->prev = n->prev;
  if (n == head_)
    head_ = n->next;
  if (n == tail_)
    tail_ = n->prev;

  delete n;
  --size_;
  return Iterator(next);
}

template <typename T>
typename CTF::Collections::List<T>::Iterator
CTF::Collections::List<T>::erase(Iterator first, Iterator last) {
  Node *current = first.raw();
  Node *endNode = last.raw();
  while (current != endNode) {
    Node *tmp = current->next;
    erase(Iterator(current));
    current = tmp;
  }
  return Iterator(endNode);
}

template <typename T> void CTF::Collections::List<T>::clear() {
  Node *current = head_;
  while (current) {
    Node *tmp = current->next;
    delete current;
    current = tmp;
  }
  head_ = tail_ = nullptr;
  size_ = 0;
}

template <typename T> T &CTF::Collections::List<T>::back() {
  if (empty())
    throw std::out_of_range("List::back() called on empty list");
  return tail_->value; // or however your last element is stored
}

template <typename T> const T &CTF::Collections::List<T>::back() const {
  if (empty())
    throw std::out_of_range("List::back() called on empty list");
  return tail_->value;
}
