/*
 * Cereon Template Framework - Task
 * Type definitions
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