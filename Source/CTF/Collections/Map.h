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

#ifndef MAP_H
#define MAP_H
#pragma once


#include "Vector.h"
#include "Pair.h"
#include "List.h"
#include "Hash.h"
#include <stdexcept>

class OutOfRangeException : public std::exception {
    const char* what() const noexcept override {
        return "Key not found";
    }
};

namespace CTF::Collections {

    template<typename K, typename V>
    class Map {
    public:
        Map(int initialCapacity = 16, float maxLoadFactor = 0.75f);

        void insert(const K& key, const V& value);
        void insert(K&& key, V&& value);

        V& at(const K& key);
        const V& at(const K& key) const;

        V& operator[](const K& key);

        void erase(const K& key);
        void clear();

        bool contains(const K& key) const;

        int size() const { return currentSize_; }
        int capacity() const { return capacity_; }

    protected:
        using PairM = Pair<K, V>;

        Vector<List<PairM>> table_;
        int currentSize_;
        int capacity_;
        float maxLoadFactor_;

        int Hash(const K& key) const {
            return CTF::Collections::Hash<K>()(key) % capacity_;
        }

        void resize();

    public:
        class Iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = Pair<K, V>;
            using difference_type = std::ptrdiff_t;
            using pointer = value_type*;
            using reference = value_type&;

            Iterator(Map* map, int bucket, typename List<PairM>::Iterator chainIt);

            reference operator*() const;
            pointer operator->() const;

            Iterator& operator++();
            Iterator operator++(int);
            bool operator==(const Iterator& other) const;
            bool operator!=(const Iterator& other) const;

        private:
            Map* map_;
            int bucket_;
            typename List<PairM>::Iterator chainIt_;

            void advance();
        };

        Iterator begin();
        Iterator end();

        class ConstIterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = const Pair<K, V>;
            using difference_type = std::ptrdiff_t;
            using pointer = const value_type*;
            using reference = const value_type&;

            ConstIterator(const Map* map, int bucket, typename List<PairM>::ConstIterator chainIt);

            reference operator*() const;
            pointer operator->() const;

            ConstIterator& operator++();
            ConstIterator operator++(int);
            bool operator==(const ConstIterator& other) const;
            bool operator!=(const ConstIterator& other) const;

        private:
            const Map* map_;
            int bucket_;
            typename List<PairM>::ConstIterator chainIt_;

            void advance();
        };

        ConstIterator begin() const;
        ConstIterator end() const;

        Iterator find(const K& key);
        ConstIterator find(const K& key) const;
    };

#   include "Map.inl"

} // namespace CTF::Collections

#endif // MAP_H