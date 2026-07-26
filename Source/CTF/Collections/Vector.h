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

#include <algorithm>
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
        explicit Vector(size_t n);

	Vector(const Vector&) = delete;
	Vector& operator=(const Vector&) = delete;

	Vector(Vector&&) noexcept;
	Vector& operator=(Vector&&) = default;

        ~Vector();

        void push_back(const T& value);
        void push_back(T&& value);
        void pop_back();

        T& operator[](size_t index);
        const T& operator[](size_t index) const;
        T& at(size_t index);

	T& front();
	const T& front() const;

	T& back();
	const T& back() const;

        size_t size() const;
        size_t capacity() const;

        void swap(Vector<T>& other) noexcept;

        Iterator begin();
        Iterator end();
        ConstIterator begin() const;
        ConstIterator end() const;

        void clear();
        void resize(size_t new_capacity);

        Iterator erase(Iterator pos);
        Iterator erase(Iterator first, Iterator last);

	const T* data() const noexcept { return data_; }

    private:
        T* data_;
        size_t size_;
        size_t capacity_;

        template<typename U>
        void insert_impl(U&& value);
    };

#   include "Vector.inl"

} // namespace CTF::Collections

#endif // VECTOR_H
