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

#ifndef MUTEX_H
#define MUTEX_H
#pragma once

#ifdef _WIN32
    #include <windows.h>
#else
    #include <pthread.h>
#endif

namespace CTF::Threading {

    class Mutex {
    public:
        Mutex() noexcept {
#ifdef _WIN32
            InitializeCriticalSection(&handle_);
#else
            pthread_mutex_init(&handle_, nullptr);
#endif
        }
        ~Mutex() noexcept {
#ifdef _WIN32
            DeleteCriticalSection(&handle_);
#else
            pthread_mutex_destroy(&handle_);
#endif
        }

        Mutex(const Mutex&) = delete;
        Mutex& operator=(const Mutex&) = delete;

        void lock() noexcept {
#ifdef _WIN32
            EnterCriticalSection(&handle_);
#else
            pthread_mutex_lock(&handle_);
#endif
        }

        bool try_lock() noexcept {
#ifdef _WIN32
            return TryEnterCriticalSection(&handle_) != 0;
#else
            return pthread_mutex_trylock(&handle_) == 0;
#endif
        }

        void unlock() noexcept {
#ifdef _WIN32
            LeaveCriticalSection(&handle_);
#else
            pthread_mutex_unlock(&handle_);
#endif
        }

    private:
#ifdef _WIN32
        CRITICAL_SECTION handle_;
#else
        pthread_mutex_t handle_;
#endif
    };

    class LockGuard {
    public:
        explicit LockGuard(Mutex& m) noexcept : m_(m) { m_.lock(); }
        ~LockGuard() noexcept { m_.unlock(); }

        LockGuard(const LockGuard&) = delete;
        LockGuard& operator=(const LockGuard&) = delete;

    private:
        Mutex& m_;
    };

}

#endif // CTF_MUTEX_H
