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

#ifndef STRING_H
#define STRING_H
#pragma once

#include "CTF.h"
#include <string>

namespace CTF {

/**
 * @brief Just a simple basic string class.
 */
template <typename T>
class CTF_API BasicString {
public:
    /**
     * @brief Initializes an empty string.
     */
    BasicString() {
        buffer_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

    /**
     * @brief Initializes a string from a character buffer.
     * @param buf The character buffer to initialize the string with.
     */
    BasicString(T c) noexcept {
        size_ = 1;
        capacity_ = size_;
        buffer_ = new T[2];
        buffer_[0] = c;
        buffer_[1] = '\0';
    }

    /**
     * @brief Initializes a string from a character buffer.
     * @param buf The character buffer to initialize the string with.
     */
    BasicString(const T* buf) noexcept {
        if (!buf) {
            buffer_ = nullptr;
            size_ = 0;
            capacity_ = 0;
            return;
        }

        size_ = std::char_traits<T>::length(buf);
        capacity_ = size_;
        buffer_ = new T[size_ + 1];
        std::char_traits<T>::copy(buffer_, buf, size_ + 1);
    }

    /**
     * @brief Initializes a string from another string.
     * @param other The string to copy.
     */
    BasicString(const BasicString& other) noexcept {
        size_ = other.size_;
        capacity_ = other.capacity_;

        if (!other.buffer_) {
            buffer_ = nullptr;
            size_ = 0;
            capacity_ = 0;
            return;
        }

        buffer_ = new T[size_ + 1];
        std::char_traits<T>::copy(buffer_, other.buffer_, size_ + 1);
    }

    /**
     * @brief Initializes a string from another string.
     * @param other The string to copy.
     */
    BasicString(BasicString&& other) noexcept {
        buffer_ = other.buffer_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.buffer_ = nullptr;
        other.size_ = 0;
    }

    /**
     * @brief De-initializes a string.
     */
    ~BasicString() {
        delete[] buffer_;
    }

    /**
     * @brief Appends a string to the another one
     * @param other The string to append.
     */
    BasicString Append(const BasicString& other) noexcept {
        size_t newSize = size_ + other.size_;
        size_t newCapacity = capacity_ == 0 ? 8 : capacity_;
        while (newCapacity < newSize) {
            newCapacity *= 2;
        }

        Reserve(newCapacity);

        if (other.size_ > 0 && other.buffer_) {
            std::char_traits<T>::copy(buffer_ + size_, other.buffer_, other.size_);
        }

        size_ = newSize;
        buffer_[size_] = '\0';

        return *this;
    }

    /**
     * @brief Initializes a string from a character buffer.
     * @param other The character buffer to initialize the string with.
     */
    BasicString &operator=(const BasicString& other) noexcept {
        if (this != &other) {
            CleanUp();

            if (!other.buffer_) {
                capacity_ = 0;
                return *this;
            }

            size_ = other.size_;
            capacity_ = other.capacity_;
            buffer_ = new T[size_ + 1];
            std::char_traits<T>::copy(buffer_, other.buffer_, size_ + 1);
        }
        return *this;
    }

    /**
     * @brief Initializes a string from a character buffer.
     * @param other The character buffer to initialize the string with.
     */
    BasicString &operator=(BasicString&& other) noexcept {
        if (this != &other) {
            CleanUp();
            buffer_ = other.buffer_;
            size_ = other.size_;
            capacity_ = other.capacity_;

            other.buffer_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    /**
     * @brief Appends a string to the another one
     * @param other The string to append.
     */
    BasicString operator+(BasicString& other) {
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

    /**
     * @brief Appends a string to the another one
     * @param other The string to append.
     */
    BasicString operator+=(const BasicString& other) {
        return Append(other);
    }

    /**
     * @brief Determines whether the current BasicString is equals to the other one.
     * @param other The BasicString to append.
     */
    bool Equals(const T* other) const {
        if (std::char_traits<T>::length(other) != size_) return false;
        return !std::char_traits<T>::compare(buffer_, other, size_);
    }

    /**
     * @brief Determines whether the current BasicString is equals to the other one.
     * @param other The BasicString to append.
     */
    bool Equals(const BasicString& other) const {
        if (std::char_traits<T>::length(other.buffer_) != size_) return false;
        return !std::char_traits<T>::compare(buffer_, other.buffer_, size_);
    }

    /**
     * @brief Determines whether the current BasicString is empty.
     * @returns true if it the current buffer is empty; otherwise false.
     */
    bool Empty() const {
        return (!buffer_ || buffer_[0] == '\0') && size_ == 0;
    }

    /**
     * @brief Clears out the current BasicString's buffer.
     */
    void Clear() {
        size_ = 0;

        if (buffer_)
            buffer_[0] = '\0';
    }

    /**
     * @brief Gets the current BasicString's capacity.
     */
    size_t Capacity() const {
        return capacity_;
    }

    /**
     * @brief Reserves a new number of capacity for the current BasicString.
     */
    void Reserve(size_t newCapacity) {
        if (newCapacity <= capacity_)
            return;

        T* newBuffer = new T[newCapacity + 1];

        if (buffer_)
        {
            std::char_traits<T>::copy(newBuffer, buffer_, size_);
            delete[] buffer_;
        }

        newBuffer[size_] = '\0';

        buffer_ = newBuffer;
        capacity_ = newCapacity;
    }

    /**
     * @brief Determines whether the current BasicString is equals to the other one.
     * @param other The BasicString to append.
     */
    bool operator==(const T* other) const {
        return Equals(other);
    }

    /**
     * @brief Determines whether the current BasicString is equals to the other one.
     * @param other The BasicString to append.
     */
    bool operator==(BasicString& other) const {
        return Equals(other);
    }

    /**
     * @brief Determines whether the current BasicString is not equal to the other one.
     * @param other The BasicString to append.
     */
    bool operator!=(const T* other) const {
        return !Equals(other);
    }

    /**
     * @brief Determines whether the current BasicString is not equal to the other one.
     * @param other The BasicString to append.
     */
    bool operator!=(BasicString& other) const {
        return !Equals(other);
    }

    /**
     * @brief Gets the current string length/size.
     */
    size_t Length() const noexcept { return size_; }

    /**
     * @brief Converts the string to a raw string.
     */
    operator const T* () const noexcept { return buffer_; }

    /**
     * @brief Converts the string to a raw string.
     */
    const T* CStr() const noexcept { return buffer_; }

private:
    /**
     * @brief Releases BasicString resources.
     */
    void CleanUp() {
        delete[] buffer_;
        buffer_ = nullptr;
        size_ = 0;
    }

    T *buffer_;
    size_t size_;
    size_t capacity_;
};

typedef BasicString<char> String;
typedef BasicString<wchar_t> WString;

} // namespace CTF

#endif // STRING_H
