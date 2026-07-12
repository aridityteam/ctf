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

#ifndef LIST_H
#define LIST_H
#pragma once



namespace CTF::Collections {

    template<typename T>
    class List {
    private:
        struct Node {
            T value;
            Node* next = nullptr;
            Node* prev = nullptr;

            Node(const T& val) : value(val) {}
            Node(T&& val) : value(std::move(val)) {}

            Node(const T& val, Node* n) : value(val), next(n) {}
            Node(T&& val, Node* n) : value(std::move(val)), next(n) {}

            template<typename... Args>
            Node(Args&&... args) : value(std::forward<Args>(args)...) {}

            template<typename... Args>
            Node(Node* n, Args&&... args) : value(std::forward<Args>(args)...), next(n) {}
        };

        Node* head_;
        Node* tail_;
        size_t size_;

    public:
        class Iterator {
            Node* node_;
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;

            Iterator(Node* n = nullptr) : node_(n) {}
            reference operator*() const { return node_->value; }
            pointer operator->() const { return &node_->value; }
            Iterator& operator++() { node_ = node_->next; return *this; }
            Iterator operator++(int) { Iterator tmp(*this); ++(*this); return tmp; }
            Iterator& operator--() { node_ = node_->prev; return *this; }
            Iterator operator--(int) { Iterator tmp(*this); --(*this); return tmp; }

            bool operator==(const Iterator& other) const { return node_ == other.node_; }
            bool operator!=(const Iterator& other) const { return node_ != other.node_; }

            Node* raw() const { return node_; }
        };

        class ConstIterator {
            const Node* node_;
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = const T;
            using difference_type = std::ptrdiff_t;
            using pointer = const T*;
            using reference = const T&;

            ConstIterator(const Node* n = nullptr) : node_(n) {}
            reference operator*() const { return node_->value; }
            pointer operator->() const { return &node_->value; }
            ConstIterator& operator++() { node_ = node_->next; return *this; }
            ConstIterator operator++(int) { ConstIterator tmp(*this); ++(*this); return tmp; }
            ConstIterator& operator--() { node_ = node_->prev; return *this; }
            ConstIterator operator--(int) { ConstIterator tmp(*this); --(*this); return tmp; }

            bool operator==(const ConstIterator& other) const { return node_ == other.node_; }
            bool operator!=(const ConstIterator& other) const { return node_ != other.node_; }
        };

        List() : head_(nullptr), tail_(nullptr), size_(0) {}
        ~List() { clear(); }

        size_t size() const { return size_; }
        bool empty() const { return size_ == 0; }

        void push_back(const T& val);
        void push_back(T&& val);
        template<typename... Args>
        void emplace_back(Args&&... args);

        void push_front(const T& val);
        void push_front(T&& val);
        template<typename... Args>
        void emplace_front(Args&&... args);

        Iterator erase(Iterator it);
        Iterator erase(Iterator first, Iterator last);
        void clear();

        Iterator begin() { return Iterator(head_); }
        Iterator end() { return Iterator(nullptr); }
        ConstIterator begin() const { return ConstIterator(head_); }
        ConstIterator end() const { return ConstIterator(nullptr); }
        ConstIterator cbegin() const { return ConstIterator(head_); }
        ConstIterator cend() const { return ConstIterator(nullptr); }

        T& back();
        const T& back() const;
    };

#   include "List.inl"

} // namespace CTF::Collections

#endif // LIST_H