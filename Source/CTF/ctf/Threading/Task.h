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

#include "Thread.h"
#include "Mutex.h"
#include <functional>
#include <memory>

namespace CTF::Threading {

	template <typename T>
	class Task {
	public:
		explicit Task(std::function<T()> func) : done_(false) {
			thread_ = Thread([this, func]() {
				result_ = func();
			});
		}

		~Task() {
			if (thread_.joinable()) thread_.join();
		}

		T get() {
			if (!done_) {
				thread_.join();
				done_ = true;
			}
			return result_;
		}

	private:
		Thread thread_;
		T result_{};
		bool done_;
		mutable Mutex mutex_;
	};

	template <>
	class Task<void> {
	public:
		explicit Task(std::function<void()> func) : done_(false) {
			thread_ = Thread([this, func]() {
				func();
			});
		}

		~Task() {
			if (thread_.joinable()) thread_.join();
		}

		void get() {
			if (!done_) {
				thread_.join();
				done_ = true;
			}
		}

	private:
		Thread thread_;
		bool done_;
		mutable Mutex mutex_;
	};
}

namespace CTF::Threading::Internal {
	template <typename Func>
	auto make_task_async(Func&& func) {
		using result_t = decltype(func());
		return Task<result_t>(std::forward<Func>(func));
	}
}

#define async(func) CTF::Threading::make_task_async([&]() { return func; })
#define async_function(return_type, name, args, body) \
	CTF::Threading::Task<return_type> name args { \
		return CTF::Threading::Internal::make_task_async([&]() -> return_type body); \
	}
#define await(task) (task).get()

#endif // TASK_H