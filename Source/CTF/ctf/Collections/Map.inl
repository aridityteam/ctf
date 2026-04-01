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
#include "Map.h"

template <typename K, typename V>
CTF::Collections::Map<K, V>::Map(int initialCapacity, float maxLoadFactor)
    : currentSize_(0), capacity_(initialCapacity),
      maxLoadFactor_(maxLoadFactor) {
  table_.resize(capacity_);
}

template <typename K, typename V>
void CTF::Collections::Map<K, V>::insert(const K &key, const V &value) {
  int index = Hash(key);
  for (auto &pair : table_[index]) {
    if (pair.first == key) {
      pair.second = value;
      return;
    }
  }
  table_[index].emplace_back(key, value);
  currentSize_++;
  if (currentSize_ > capacity_ * maxLoadFactor_)
    resize();
}

template <typename K, typename V>
void CTF::Collections::Map<K, V>::insert(K &&key, V &&value) {
  int index = Hash(key);
  for (auto &pair : table_[index]) {
    if (pair.first == key) {
      pair.second = std::move(value);
      return;
    }
  }
  table_[index].emplace_back(std::move(key), std::move(value));
  currentSize_++;
  if (currentSize_ > capacity_ * maxLoadFactor_)
    resize();
}

template <typename K, typename V>
V &CTF::Collections::Map<K, V>::at(const K &key) {
  int index = Hash(key);
  for (auto &pair : table_[index]) {
    if (pair.first == key)
      return pair.second;
  }
  throw OutOfRangeException();
}

template <typename K, typename V>
const V &CTF::Collections::Map<K, V>::at(const K &key) const {
  int index = Hash(key);
  for (const auto &pair : table_[index]) {
    if (pair.first == key)
      return pair.second;
  }
  throw OutOfRangeException();
}

template <typename K, typename V>
V &CTF::Collections::Map<K, V>::operator[](const K &key) {
  int index = Hash(key);
  for (auto &pair : table_[index]) {
    if (pair.first == key)
      return pair.second;
  }
  table_[index].emplace_back(key, V());
  currentSize_++;
  if (currentSize_ > capacity_ * maxLoadFactor_)
    resize();
  return table_[index].back().second;
}

template <typename K, typename V>
void CTF::Collections::Map<K, V>::erase(const K &key) {
  int index = Hash(key);
  auto &chain = table_[index];
  for (auto it = chain.begin(); it != chain.end(); ++it) {
    if (it->first == key) {
      chain.erase(it);
      currentSize_--;
      return;
    }
  }
}

template <typename K, typename V> void CTF::Collections::Map<K, V>::clear() {
  for (auto &chain : table_)
    chain.clear();
  currentSize_ = 0;
}

template <typename K, typename V>
bool CTF::Collections::Map<K, V>::contains(const K &key) const {
  int index = Hash(key);
  for (const auto &pair : table_[index]) {
    if (pair.first == key)
      return true;
  }
  return false;
}

template <typename K, typename V> void CTF::Collections::Map<K, V>::resize() {
  int newCapacity = capacity_ * 2;
  Vector<List<PairM>> newTable(newCapacity);

  for (auto &chain : table_) {
    for (auto &pair : chain) {
      int newIndex = CTF::Collections::Hash<K>()(pair.first) % newCapacity;
      newTable[newIndex].emplace_back(std::move(pair));
    }
  }

  table_.swap(newTable);
  capacity_ = newCapacity;
}

template <typename K, typename V>
CTF::Collections::Map<K, V>::Iterator::Iterator(
    Map *map, int bucket, typename List<Pair<K, V>>::Iterator chainIt)
    : map_(map), bucket_(bucket), chainIt_(chainIt) {
  if (bucket_ < map_->capacity_ && chainIt_ == map_->table_[bucket_].end())
    advance();
}

template <typename K, typename V>
void CTF::Collections::Map<K, V>::Iterator::advance() {
  while (bucket_ < map_->capacity_ && chainIt_ == map_->table_[bucket_].end()) {
    bucket_++;
    if (bucket_ < map_->capacity_)
      chainIt_ = map_->table_[bucket_].begin();
  }
}

template <typename K, typename V>
auto CTF::Collections::Map<K, V>::Iterator::operator*() const -> reference {
  return *chainIt_;
}

template <typename K, typename V>
auto CTF::Collections::Map<K, V>::Iterator::operator->() const -> pointer {
  return &(*chainIt_);
}

template <typename K, typename V>
auto CTF::Collections::Map<K, V>::Iterator::operator++() -> Iterator & {
  ++chainIt_;
  advance();
  return *this;
}

template <typename K, typename V>
auto CTF::Collections::Map<K, V>::Iterator::operator++(int) -> Iterator {
  Iterator tmp = *this;
  ++(*this);
  return tmp;
}

template <typename K, typename V>
bool CTF::Collections::Map<K, V>::Iterator::operator==(
    const Iterator &other) const {
  return map_ == other.map_ && bucket_ == other.bucket_ &&
         chainIt_ == other.chainIt_;
}

template <typename K, typename V>
bool CTF::Collections::Map<K, V>::Iterator::operator!=(
    const Iterator &other) const {
  return !(*this == other);
}

template <typename K, typename V>
CTF::Collections::Map<K, V>::ConstIterator::ConstIterator(
    const Map *map, int bucket,
    typename List<Pair<K, V>>::ConstIterator chainIt)
    : map_(map), bucket_(bucket), chainIt_(chainIt) {
  if (bucket_ < map_->capacity_ && chainIt_ == map_->table_[bucket_].end())
    advance();
}

template <typename K, typename V>
void CTF::Collections::Map<K, V>::ConstIterator::advance() {
  while (bucket_ < map_->capacity_ && chainIt_ == map_->table_[bucket_].end()) {
    bucket_++;
    if (bucket_ < map_->capacity_)
      chainIt_ = map_->table_[bucket_].begin();
  }
}

template <typename K, typename V>
auto CTF::Collections::Map<K, V>::ConstIterator::operator*() const
    -> reference {
  return *chainIt_;
}

template <typename K, typename V>
auto CTF::Collections::Map<K, V>::ConstIterator::operator->() const -> pointer {
  return &(*chainIt_);
}

template <typename K, typename V>
auto CTF::Collections::Map<K, V>::ConstIterator::operator++()
    -> ConstIterator & {
  ++chainIt_;
  advance();
  return *this;
}

template <typename K, typename V>
auto CTF::Collections::Map<K, V>::ConstIterator::operator++(int)
    -> ConstIterator {
  ConstIterator tmp = *this;
  ++(*this);
  return tmp;
}

template <typename K, typename V>
bool CTF::Collections::Map<K, V>::ConstIterator::operator==(
    const ConstIterator &other) const {
  return map_ == other.map_ && bucket_ == other.bucket_ &&
         chainIt_ == other.chainIt_;
}

template <typename K, typename V>
bool CTF::Collections::Map<K, V>::ConstIterator::operator!=(
    const ConstIterator &other) const {
  return !(*this == other);
}

template <typename K, typename V>
auto CTF::Collections::Map<K, V>::begin() -> Iterator {
  return Iterator(this, 0, table_[0].begin());
}

template <typename K, typename V>
auto CTF::Collections::Map<K, V>::end() -> Iterator {
  return Iterator(this, capacity_, typename List<Pair<K, V>>::Iterator());
}

template <typename K, typename V>
auto CTF::Collections::Map<K, V>::begin() const -> ConstIterator {
  return ConstIterator(this, 0, table_[0].begin());
}

template <typename K, typename V>
auto CTF::Collections::Map<K, V>::end() const -> ConstIterator {
  return ConstIterator(this, capacity_,
                       typename List<Pair<K, V>>::ConstIterator());
}

template <typename K, typename V>
auto CTF::Collections::Map<K, V>::find(const K &key) -> Iterator {
  int index = Hash(key);
  for (auto it = table_[index].begin(); it != table_[index].end(); ++it) {
    if (it->first == key)
      return Iterator(this, index, it);
  }
  return end();
}

template <typename K, typename V>
auto CTF::Collections::Map<K, V>::find(const K &key) const -> ConstIterator {
  int index = Hash(key);
  for (auto it = table_[index].begin(); it != table_[index].end(); ++it) {
    if (it->first == key)
      return ConstIterator(this, index, it);
  }
  return end();
}
