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

#ifndef VECTOR_H
#define VECTOR_H
#pragma once

#include <cstring>
#include <utility>
#include <stdexcept>

namespace CTF::Collections {

    template <typename T>
    class Vector {
    public:
        using Iterator = T*;
        using ConstIterator = const T*;

        Vector();
        explicit Vector(int n);
        ~Vector();

        void push_back(const T& value);
        void push_back(T&& value);
        void pop_back();

        T& operator[](int index);
        const T& operator[](int index) const;
        T& at(int index);

        int size() const;
        int capacity() const;

        void swap(Vector<T>& other) noexcept;

        Iterator begin();
        Iterator end();
        ConstIterator begin() const;
        ConstIterator end() const;

        void clear();
        void resize(int new_capacity);

        Iterator erase(Iterator pos);
        Iterator erase(Iterator first, Iterator last);

    private:
        T* data_;
        int size_;
        int capacity_;

        template<typename U>
        void insert_impl(U&& value);
    };

#   include "Vector.inl"

} // namespace CTF::Collections

#endif // VECTOR_H