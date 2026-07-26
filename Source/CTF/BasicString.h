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
#include <memory>
#include <stdexcept>
#include <string>

namespace CTF {

/**
 * @brief Just a simple basic string class.
 */
template <
	typename CharT,
	typename Traits = std::char_traits<CharT>,
	typename Allocator = std::allocator<CharT>>
class CTF_API BasicString {
public:
	using ValueType = CharT;
	using TraitsType = Traits;
	using AllocatorType = Allocator;
	using AllocTraits = std::allocator_traits<Allocator>;
	using SizeType = typename AllocTraits::size_type;
	using Pointer = typename AllocTraits::pointer;
	using ConstPointer = typename AllocTraits::const_pointer;

public:
    /**
     * @brief Initializes an empty string.
     */
    explicit BasicString(const Allocator& alloc = Allocator())
	: size_(0)
	, capacity_(0)
	, allocator_(alloc) {	
		buffer_ = AllocTraits::allocate(allocator_, 1);
		buffer_[0] = '\0';
	}

    /**
     * @brief Initializes a string from a character buffer.
     * @param buf The character buffer to initialize the string with.
     */
    BasicString(CharT c, const Allocator& alloc = Allocator()) noexcept
	: size_(1)
	, capacity_(size_)
	, allocator_(alloc) {
        buffer_ = AllocTraits::allocate(allocator_, size_ + 1);
        buffer_[0] = c;
        buffer_[1] = '\0';
    }

    /**
     * @brief Initializes a string from a character buffer.
     * @param buf The character buffer to initialize the string with.
     */
    BasicString(ConstPointer buf, const Allocator& alloc = Allocator()) noexcept
	    : allocator_(alloc) {
        if (!buf) {
            buffer_ = nullptr;
            size_ = 0;
            capacity_ = 0;
            return;
        }

        size_ = Traits::length(buf);
        capacity_ = size_;
        buffer_ = AllocTraits::allocate(allocator_, size_ + 1);
        Traits::copy(buffer_, buf, size_ + 1);
    }

    /**
     * @brief Initializes a string from another string.
     * @param other The string to copy.
     */
    BasicString(const BasicString& other) noexcept
	    : allocator_(AllocTraits::select_on_container_copy_construction(
				    other.allocator_)) {
        size_ = other.size_;
        capacity_ = other.capacity_;

        if (!other.buffer_) {
            buffer_ = nullptr;
            size_ = 0;
            capacity_ = 0;
            return;
        }

        buffer_ = AllocTraits::allocate(allocator_, size_ + 1);
        Traits::copy(buffer_, other.buffer_, size_ + 1);
    }

    /**
     * @brief Initializes a string from another string.
     * @param other The string to copy.
     */
    BasicString(BasicString&& other) noexcept
	    : allocator_(std::move(other.allocator_)) {
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
	    if (buffer_) {
		AllocTraits::deallocate(allocator_, buffer_, capacity_ + 1);
	    }
    }

    /**
     * @brief Appends a string to the another one
     * @param other The string to append.
     */
    BasicString& Append(const BasicString& other) {
	if (other.size_ > std::numeric_limits<SizeType>::max() - size_)
		throw std::length_error("BasicString is too large.");
    
	SizeType newSize = size_ + other.size_;
	SizeType newCapacity = capacity_ == 0 ? 8 : capacity_;
        while (newCapacity < newSize) {
        	if (newCapacity > std::numeric_limits<SizeType>::max() / 2)
			newCapacity = newSize;
		else
			newCapacity *= 2;
	}

        Reserve(newCapacity);

        if (other.size_ > 0 && other.buffer_) {
            if (this == &other)
		Traits::move(buffer_ + size_, buffer_, size_);
	    else
		Traits::copy(buffer_ + size_, other.buffer_, other.size_);
        }

        size_ = newSize;
        buffer_[size_] = '\0';

        return *this;
    }

    /**
     * @brief Initializes a string from a character buffer.
     * @param other The character buffer to initialize the string with.
     */
    BasicString& operator=(const BasicString& other) noexcept {
        if (this != &other) {
            CleanUp();

            if (!other.buffer_) {
                capacity_ = 0;
                return *this;
            }

            size_ = other.size_;
            capacity_ = other.capacity_;
            buffer_ = AllocTraits::allocate(allocator_, size_ + 1);
            Traits::copy(buffer_, other.buffer_, size_ + 1);
        }
        return *this;
    }

    /**
     * @brief Initializes a string from a character buffer.
     * @param other The character buffer to initialize the string with.
     */
    BasicString& operator=(BasicString&& other) noexcept {
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
    BasicString operator+(const BasicString& other) {
	BasicString result(*this);
	result.Append(other);
	return result;
    }

    /**
     * @brief Appends a string to the another one
     * @param other The string to append.
     */
    BasicString& operator+=(const BasicString& other) {
        return Append(other);
    }

    /**
     * @brief Determines whether the current BasicString is equals to the other one.
     * @param other The BasicString to append.
     */
    bool Equals(ConstPointer other) const {
        if (Traits::length(other) != size_) return false;
        return !Traits::compare(buffer_, other, size_);
    }

    /**
     * @brief Determines whether the current BasicString is equals to the other one.
     * @param other The BasicString to append.
     */
    bool Equals(const BasicString& other) const {
        if (other.size_ != size_) return false;
        return !Traits::compare(buffer_, other.buffer_, size_);
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
    SizeType Capacity() const {
        return capacity_;
    }

    /**
     * @brief Reserves a new number of capacity for the current BasicString.
     */
    void Reserve(SizeType newCapacity) {
        if (newCapacity <= capacity_)
            return;

        Pointer newBuffer = AllocTraits::allocate(allocator_, newCapacity + 1);

        if (buffer_) {
            Traits::copy(newBuffer, buffer_, size_);
	    AllocTraits::deallocate(
			    allocator_,
			    buffer_,
			    capacity_ + 1
			);
        }

        newBuffer[size_] = CharT();

        buffer_ = newBuffer;
        capacity_ = newCapacity;
    }

    /**
     * @brief Determines whether the current BasicString is equals to the other one.
     * @param other The BasicString to append.
     */
    bool operator==(ConstPointer other) const {
        return Equals(other);
    }

    /**
     * @brief Determines whether the current BasicString is equals to the other one.
     * @param other The BasicString to append.
     */
    bool operator==(const BasicString& other) const {
        return Equals(other);
    }

    /**
     * @brief Determines whether the current BasicString is not equal to the other one.
     * @param other The BasicString to append.
     */
    bool operator!=(ConstPointer other) const {
        return !Equals(other);
    }

    /**
     * @brief Determines whether the current BasicString is not equal to the other one.
     * @param other The BasicString to append.
     */
    bool operator!=(const BasicString& other) const {
        return !Equals(other);
    }

	/**
	 * @brief Gets a single character from a specific index.
	 */
	CharT At(SizeType index) const {
		if (index >= size_)
			throw std::out_of_range(
				"BasicString::At got an out-of-bounds index."
			);

		return buffer_[index];
	}

	/**
	 * @brief Gets a single character from a specific index.
	 */
	CharT& operator [](SizeType index) noexcept { return buffer_[index]; }
	
	/**
	 * @brief Gets a single character from a specific index.
	 */
	const CharT& operator [](SizeType index) const noexcept { return buffer_[index]; }

    /**
     * @brief Gets the current string length/size.
     */
    SizeType Length() const noexcept { return size_; }

    /**
     * @brief Converts the string to a raw string.
     */
    operator ConstPointer () const noexcept { return buffer_; }

    /**
     * @brief Converts the string to a raw string.
     */
    ConstPointer CStr() const noexcept { return buffer_; }

private:
    /**
     * @brief Releases BasicString resources.
     */
    void CleanUp() {
	    AllocTraits::deallocate(allocator_, buffer_, capacity_ + 1);
	    size_ = 0;
    }

    Pointer buffer_;
    SizeType size_;
    SizeType capacity_;
    [[no_unique_address]] Allocator allocator_;
};

typedef BasicString<char> String;
typedef BasicString<wchar_t> WString;

} // namespace CTF

#endif // STRING_H
