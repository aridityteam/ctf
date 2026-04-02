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
#include "String.h"

template<typename T>
CTF::BasicString<T>::BasicString() {
    buffer_ = nullptr;
    size_ = 0;
    capacity_ = 0;
}

template<typename T>
CTF::BasicString<T>::BasicString(T c) noexcept {
    size_ = 1;
    capacity_ = size_;
    buffer_ = new T[2];
    buffer_[0] = c;
    buffer_[1] = '\0';
}

template<typename T>
CTF::BasicString<T>::BasicString(const T *buf) noexcept {
    if (!buf)
    {
        buffer_ = nullptr;
        size_ = 0;
        capacity_ = 0;
        return;
    }

    size_ = Q_strlen(buf);
    capacity_ = size_;
    buffer_ = new char[size_ + 1];
    Q_strcpy(buffer_, buf);
}

template<typename T>
CTF::BasicString<T>::BasicString(const BasicString<T>& other) noexcept {
    size_ = other.size_;
    capacity_ = other.capacity_;

    if (!other.buffer_) {
        buffer_ = nullptr;
        size_ = 0;
        capacity_ = 0;
        return;
    }

    buffer_ = new char[size_ + 1];
    Q_memcpy(buffer_, other.buffer_, size_ + 1);
}

template<typename T>
CTF::BasicString<T>::BasicString(BasicString<T>&& other) noexcept {
    buffer_ = other.buffer_;
    size_ = other.size_;
    capacity_ = other.capacity_;

    other.buffer_ = nullptr;
    other.size_ = 0;
}

template<typename T>
CTF::BasicString<T>::BasicString::~BasicString() {
    delete[] buffer_;
}

template<typename T>
CTF::BasicString<T> &CTF::BasicString<T>::append(const CTF::BasicString<T> &other) noexcept {
    size_t newSize = size_ + other.size_;
    size_t newCapacity = capacity_ == 0 ? 8 : capacity_;
    while (newCapacity < newSize) {
        newCapacity *= 2;
    }

    reserve(newCapacity);

    if (other.size_ > 0 && other.buffer_) {
        Q_memcpy(buffer_ + size_, other.buffer_, other.size_);
    }

    size_ = newSize;
    buffer_[size_] = '\0';

    return *this;
}

template<typename T>
CTF::BasicString<T> &CTF::BasicString<T>::operator=(const CTF::BasicString<T> &other) noexcept {
    if (this != &other) {
        cleanup();

        if (!other.buffer_) {
            capacity_ = 0;
            return *this;
        }

        size_ = other.size_;
        capacity_ = other.capacity_;
        buffer_ = new char[size_ + 1];
        Q_strcpy(buffer_, other.buffer_);
    }
    return *this;
}

template<typename T>
CTF::BasicString<T> &CTF::BasicString<T>::operator=(CTF::BasicString<T> &&other) noexcept {
    if (this != &other) {
        cleanup();
        buffer_ = other.buffer_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.buffer_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

template<typename T>
CTF::BasicString<T> &CTF::BasicString<T>::operator+(CTF::BasicString<T> &&other) {
    if (this != &other)
    {
        delete[] buffer_;

        buffer_ = other.buffer_;
        size_ = other.size_;

        other.buffer_ = nullptr;
        other.size_ = 0;
    }

    return *this;
}

template<typename T>
CTF::BasicString<T> &CTF::BasicString<T>::operator+=(const CTF::BasicString<T> &other) {
    return append(other);
}

template<typename T>
bool CTF::BasicString<T>::equals(const T *other) const {
    return !Q_strcmp(buffer_, other);
}

template<typename T>
bool CTF::BasicString<T>::equals(const CTF::BasicString<T> &other) const {
    return !Q_strcmp(buffer_, other.buffer_);
}

template<typename T>
bool CTF::BasicString<T>::empty() const {
    return (!buffer_ || buffer_[0] == '\0') && size_ == 0;
}

template<typename T>
size_t CTF::BasicString<T>::capacity() const {
    return capacity_;
}

template<typename T>
void CTF::BasicString<T>::clear() {
    size_ = 0;

    if (buffer_)
        buffer_[0] = '\0';
}

template<typename T>
void CTF::BasicString<T>::reserve(size_t newCapacity) {
    if (newCapacity <= capacity_)
        return;

    char* newBuffer = new char[newCapacity + 1];

    if (buffer_)
    {
        Q_memcpy(newBuffer, buffer_, size_);
        delete[] buffer_;
    }

    newBuffer[size_] = '\0';

    buffer_ = newBuffer;
    capacity_ = newCapacity;
}

template<typename T>
bool CTF::BasicString<T>::operator==(const T *other) const {
    return equals(other);
}

template<typename T>
bool CTF::BasicString<T>::operator==(CTF::BasicString<T> &other) const {
    return equals(other);
}

template<typename T>
bool CTF::BasicString<T>::operator!=(const T *other) const {
    return !equals(other);
}

template<typename T>
bool CTF::BasicString<T>::operator!=(CTF::BasicString<T> &other) const {
    return !equals(other);
}

template<typename T>
void CTF::BasicString<T>::cleanup() {
    delete[] buffer_;
    buffer_ = nullptr;
    size_ = 0;
}
