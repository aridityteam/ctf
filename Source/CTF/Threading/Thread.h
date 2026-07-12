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

#ifndef THREAD_H
#define THREAD_H
#pragma once

#include <memory>
#include <functional>
#include <chrono>

#ifdef _WIN32
#   include <Windows.h>
#else
#   include <pthread.h>
#endif

namespace CTF::Threading {
#   ifdef _WIN32
        typedef HANDLE native_handle_type;
#       define NATIVE_HANDLE_NULL nullptr
#   else
        typedef pthread_t native_handle_type;
#       define NATIVE_HANDLE_NULL {}
#   endif
        
    struct ThreadImpl {
        native_handle_type handle = NATIVE_HANDLE_NULL;
        bool joinable = false;
    };

    template <typename F>
    struct ThreadData {
        F func;
        explicit ThreadData(F&& f) noexcept(std::is_nothrow_move_constructible_v<F>)
            : func(std::forward<F>(f)) {}
    };

#ifdef _WIN32
    template <typename F>
    DWORD WINAPI thread_entry(LPVOID data)
#else
    template <typename F>
    void* thread_entry(void* data)
#endif // _WIN32
    {
        std::unique_ptr<ThreadData<F>> fn(static_cast<ThreadData<F>*>(data));
        fn->func();
        return 0;
    }

#if defined(__clang__)
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wattributes"
#elif defined(__GNUC__)
#	pragma GCC diagnostic push
#	pragma GCC diagnostic ignored "-Wattributes"
#else
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif
    class Thread {
    public:
        Thread() noexcept = default;

        template <typename F, typename... Args>
        explicit Thread(F&& f, Args&&... args) : pimpl_(std::make_unique<ThreadImpl>()) {
            auto bound = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
            using BoundType = decltype(bound);

            auto* data = new ThreadData<BoundType>(std::move(bound));

#ifdef _WIN32
            pimpl_->handle = CreateThread(nullptr, 0, thread_entry<BoundType>, data, 0, nullptr);
            if (!pimpl_->handle) {
                delete data;
                throw std::runtime_error("Failed to create thread");
            }
#else
            if (pthread_create(&pimpl_->handle, nullptr, thread_entry<BoundType>, data) != 0) {
                delete data;
                throw std::runtime_error("Failed to create pthread");
            }
#endif
            pimpl_->joinable = true;
        }

        ~Thread() {
            if (pimpl_ && pimpl_->joinable) {
                join();
            }
        }

        void join() {
            if (!pimpl_ || !pimpl_->joinable) return;
#ifdef _WIN32
            WaitForSingleObject(pimpl_->handle, INFINITE);
            CloseHandle(pimpl_->handle);
#else
            pthread_join(pimpl_->handle, nullptr);
#endif
            pimpl_->joinable = false;
        }

        bool joinable() const noexcept {
            return pimpl_ && pimpl_->joinable;
        }

        void detach() {
            if (!pimpl_ || !pimpl_->joinable) return;
#ifdef _WIN32
            CloseHandle(pimpl_->handle);
#else
            pthread_detach(pimpl_->handle);
#endif
            pimpl_->joinable = false;
        }

        Thread(const Thread&) = delete;
        Thread& operator=(const Thread&) = delete;

        Thread(Thread&& other) noexcept = default;
        Thread& operator=(Thread&& other) noexcept = default;

    private:
        std::unique_ptr<ThreadImpl> pimpl_;
    };
#if defined(__clang__)
#	pragma clang diagnostic pop
#elif defined(__GNUC__)
#	pragma GCC diagnostic pop
#else
#	pragma warning(pop)
#endif
}

#endif // THREAD_H
