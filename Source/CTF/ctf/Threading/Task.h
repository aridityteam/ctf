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

#ifndef TASK_H
#define TASK_H
#pragma once

#include <CTF.h>
#include "CancellationToken.h"
#include "Thread.h"
#include "Mutex.h"
#include <functional>
#include <memory>

namespace CTF::Threading {

    template<typename T>
    class Task
    {
    public:
        explicit Task(
            std::function<T( CancellationToken )> func,
            CancellationToken token = {} )
            : cancellationToken_( std::move( token ) )
        {
            thread_ = Thread( [ this, func ]()
                              {
                                  try
                                  {
                                      cancellationToken_.ThrowIfCancellationRequested();

                                      result_ = func( cancellationToken_ );

                                      canceled_ = cancellationToken_.IsCancellationRequested();
                                  }
                                  catch ( ... )
                                  {
                                      exception_ = std::current_exception();
                                  }

                                  done_ = true;
                              } );
        }

        ~Task()
        {
            if ( thread_.joinable() )
                thread_.join();
        }

        T get()
        {
            if ( thread_.joinable() )
                thread_.join();

            done_ = true;

            if ( exception_ )
                std::rethrow_exception( exception_ );

            return result_;
        }

        bool IsCompleted() const
        {
            return done_;
        }

        bool IsCanceled() const
        {
            return canceled_;
        }

    private:
        Thread thread_;

        CancellationToken cancellationToken_;

        T result_ {};

        std::atomic<bool> done_ { false };
        std::atomic<bool> canceled_ { false };

        std::exception_ptr exception_;
    };

    template<>
    class Task<void>
    {
    public:
        explicit Task(
            std::function<void( CancellationToken )> func,
            CancellationToken token = {} )
            : cancellationToken_( std::move( token ) )
        {
            thread_ = Thread( [ this, func ]()
                              {
                                  try
                                  {
                                      cancellationToken_.ThrowIfCancellationRequested();

                                      func( cancellationToken_ );

                                      canceled_ = cancellationToken_.IsCancellationRequested();
                                  }
                                  catch ( ... )
                                  {
                                      exception_ = std::current_exception();
                                  }

                                  done_ = true;
                              } );
        }

        ~Task()
        {
            if ( thread_.joinable() )
                thread_.join();
        }

        void get()
        {
            if ( thread_.joinable() )
                thread_.join();

            done_ = true;

            if ( exception_ )
                std::rethrow_exception( exception_ );
        }

        bool IsCompleted() const
        {
            return done_;
        }

        bool IsCanceled() const
        {
            return canceled_;
        }

    private:
        Thread thread_;

        CancellationToken cancellationToken_;

        std::atomic<bool> done_ { false };
        std::atomic<bool> canceled_ { false };

        std::exception_ptr exception_;
    };
}

namespace CTF::Threading::Internal
{
    template <typename Func>
    auto make_task_async( Func &&func, CancellationToken token = {} )
    {
        using result_t = decltype( func( token ) );
        return Task<result_t>( std::forward<Func>( func ), token );
    }
}

#define async(func) CTF::Threading::make_task_async([&]() { return func; })
#define async_function(return_type, name, args, body) \
	CTF::Threading::Task<return_type> name args { \
		return CTF::Threading::Internal::make_task_async([&]() -> return_type body); \
	}
#define await(task) (task).get()

#endif // TASK_H
