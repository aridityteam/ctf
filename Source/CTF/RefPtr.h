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

#ifndef CTF_REFPTR_H
#define CTF_REFPTR_H
#pragma once

namespace CTF {
    template <typename T>
    class RefPtr {
    public:
        RefPtr(T *p = nullptr) : ptr_(p) {
            if (ptr_) ptr_->addRef();
        }

        RefPtr(const RefPtr &other) : ptr_(other.ptr_) {
            if (ptr_) ptr_->addRef();
        }

        RefPtr& operator=(const RefPtr& other) {
            if (ptr_ != other.ptr_) {
                if (ptr_) ptr_->release();
                ptr_ = other.ptr_;
                if (ptr_) ptr_->addRef();
            }
            return *this;
        }

        ~RefPtr() {
            if (ptr_) ptr_->release();
        }

        T *operator->() const { return ptr_; }
        T &operator*() const { return *ptr_; }

    private:
        T *ptr_;
    };
}

#endif // !CTF_REFPTR_H
