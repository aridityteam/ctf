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

namespace CTF {

/******************************************************************************
 * @brief Just a simple string class.
 ******************************************************************************/
template<typename T>
class CTF_API BasicString {
public:
    /******************************************************************************
     * @brief Initializes an empty string.
     ******************************************************************************/
    BasicString();

    /******************************************************************************
     * @brief Initializes a string from a character buffer.
     * @param buf The character buffer to initialize the string with.
     ******************************************************************************/
    BasicString(T c) noexcept;

    /******************************************************************************
     * @brief Initializes a string from a character buffer.
     * @param buf The character buffer to initialize the string with.
     ******************************************************************************/
    BasicString(const T *buf) noexcept;

    /******************************************************************************
     * @brief Initializes a string from another string.
     * @param other The string to copy.
     ******************************************************************************/
    BasicString(const BasicString<T> &other) noexcept;

    /******************************************************************************
     * @brief Initializes a string from another string.
     * @param other The string to copy.
     ******************************************************************************/
    BasicString(BasicString<T> &&other) noexcept;

    /******************************************************************************
     * @brief De-initializes a string.
     ******************************************************************************/
    ~BasicString();

    /******************************************************************************
     * @brief Appends a string to the another one
     * @param other The string to append.
     ******************************************************************************/
    BasicString<T> &append(const BasicString<T> &other) noexcept;

    /******************************************************************************
     * @brief Initializes a string from a character buffer.
     * @param other The character buffer to initialize the string with.
     ******************************************************************************/
    BasicString<T> &operator=(const BasicString<T> &other) noexcept;

    /******************************************************************************
     * @brief Initializes a string from a character buffer.
     * @param other The character buffer to initialize the string with.
     ******************************************************************************/
    BasicString<T> &operator=(BasicString<T> &&other) noexcept;

    /******************************************************************************
     * @brief Appends a string to the another one
     * @param other The string to append.
     ******************************************************************************/
    BasicString<T> &operator+(BasicString<T> &&other);

    /******************************************************************************
     * @brief Appends a string to the another one
     * @param other The string to append.
     ******************************************************************************/
    BasicString<T> &operator+=(const BasicString<T> &other);

    /******************************************************************************
     * @brief Determines whether the current BasicString is equals to the other one.
     * @param other The BasicString to append.
     ******************************************************************************/
    bool equals(const T *other) const;

    /******************************************************************************
     * @brief Determines whether the current BasicString is equals to the other one.
     * @param other The BasicString to append.
     ******************************************************************************/
    bool equals(const BasicString &other) const;

    /******************************************************************************
     * @brief Determines whether the current BasicString is empty.
     * @returns true if it the current buffer is empty; otherwise false.
     ******************************************************************************/
    bool empty() const;

    /******************************************************************************
     * @brief Clears out the current BasicString's buffer.
     ******************************************************************************/
    void clear();

    /******************************************************************************
     * @brief Gets the current BasicString's capacity.
     ******************************************************************************/
    size_t capacity() const;

    /******************************************************************************
     * @brief Reserves a new number of capacity for the current BasicString.
     ******************************************************************************/
    void reserve(size_t newCapacity);

    /******************************************************************************
     * @brief Determines whether the current BasicString is equals to the other one.
     * @param other The BasicString to append.
     ******************************************************************************/
    bool operator==(const T *other) const;

    /******************************************************************************
     * @brief Determines whether the current BasicString is equals to the other one.
     * @param other The BasicString to append.
     ******************************************************************************/
    bool operator==(BasicString<T> &other) const;

    /******************************************************************************
     * @brief Determines whether the current BasicString is not equal to the other one.
     * @param other The BasicString to append.
     ******************************************************************************/
    bool operator!=(const T *other) const;

    /******************************************************************************
     * @brief Determines whether the current BasicString is not equal to the other one.
     * @param other The BasicString to append.
     ******************************************************************************/
    bool operator!=(BasicString<T> &other) const;

    /******************************************************************************
     * @brief Gets the current string length/size.
     ******************************************************************************/
    size_t length() const noexcept { return size_; }

    /******************************************************************************
     * @brief Converts the string to a raw string.
     ******************************************************************************/
    operator const T *() const noexcept { return buffer_; }

    /******************************************************************************
     * @brief Converts the string to a raw string.
     ******************************************************************************/
    const T *c_str() const noexcept { return buffer_; }

private:
    /******************************************************************************
     * @brief Releases BasicString resources.
     ******************************************************************************/
    void cleanup();

    T *buffer_;
    size_t size_;
    size_t capacity_;
};

typedef BasicString<char> String;
typedef BasicString<wchar_t> WString;

} // namespace CTF

#include "String.inl"

#endif // STRING_H
