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
#include "Vector.h"

template <typename T>
CTF::Collections::Vector<T>::Vector()
    : data_(nullptr), size_(0), capacity_(0) {}

template <typename T>
CTF::Collections::Vector<T>::Vector(int n)
    : data_(nullptr), size_(0), capacity_(0) {
  resize(n);
}

template <typename T> CTF::Collections::Vector<T>::Vector::~Vector() { clear(); }

template <typename T>
void CTF::Collections::Vector<T>::push_back(const T &value) {
  insert_impl(value);
}

template <typename T> void CTF::Collections::Vector<T>::push_back(T &&value) {
  insert_impl(std::move(value));
}

template <typename T> void CTF::Collections::Vector<T>::pop_back() {
  if (size_ > 0)
    --size_;
}

template <typename T> T &CTF::Collections::Vector<T>::operator[](int index) {
  return data_[index];
}

template <typename T>
const T &CTF::Collections::Vector<T>::operator[](int index) const {
  return data_[index];
}

template <typename T> T &CTF::Collections::Vector<T>::at(int index) {
  if (index < 0 || index >= size_)
    throw std::out_of_range("Vector::at out of range");
  return data_[index];
}

template <typename T> int CTF::Collections::Vector<T>::size() const {
  return size_;
}

template <typename T> int CTF::Collections::Vector<T>::capacity() const {
  return capacity_;
}

template <typename T>
void CTF::Collections::Vector<T>::swap(Vector<T> &other) noexcept {
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

template <typename T>
CTF::Collections::Vector<T>::Iterator CTF::Collections::Vector<T>::begin() {
  return data_;
}

template <typename T>
CTF::Collections::Vector<T>::Iterator CTF::Collections::Vector<T>::end() {
  return data_ + size_;
}

template <typename T>
CTF::Collections::Vector<T>::ConstIterator
CTF::Collections::Vector<T>::begin() const {
  return data_;
}

template <typename T>
CTF::Collections::Vector<T>::ConstIterator
CTF::Collections::Vector<T>::end() const {
  return data_ + size_;
}

template <typename T> void CTF::Collections::Vector<T>::clear() {
  for (int i = 0; i < size_; ++i)
    data_[i].~T();
  ::operator delete[](data_);
  data_ = nullptr;
  size_ = capacity_ = 0;
}

template <typename T>
CTF::Collections::Vector<T>::Iterator
CTF::Collections::Vector<T>::erase(Iterator pos) {
  if (pos < data_ || pos >= data_ + size_)
    return end();
  for (T *it = pos; it < data_ + size_ - 1; ++it) {
    *it = std::move(*(it + 1));
  }
  --size_;
  return pos;
}

template <typename T>
CTF::Collections::Vector<T>::Iterator
CTF::Collections::Vector<T>::erase(Iterator first, Iterator last) {
  int n = static_cast<int>(last - first);
  for (T *it = first; it < data_ + size_ - n; ++it) {
    *it = std::move(*(it + n));
  }
  size_ -= n;
  return first;
}

template <typename T>
void CTF::Collections::Vector<T>::resize(int new_capacity) {
  T *new_data = static_cast<T *>(::operator new[](new_capacity * sizeof(T)));
  for (int i = 0; i < size_; ++i) {
    new (new_data + i) T(std::move(data_[i]));
    data_[i].~T();
  }
  ::operator delete[](data_);
  data_ = new_data;
  capacity_ = new_capacity;
}

template <typename T>
template <typename U>
void CTF::Collections::Vector<T>::insert_impl(U &&value) {
  if (size_ == capacity_)
    resize(capacity_ == 0 ? 1 : capacity_ * 2);
  new (data_ + size_) T(std::forward<U>(value));
  ++size_;
}
