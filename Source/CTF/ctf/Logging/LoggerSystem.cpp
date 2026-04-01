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
#include "LoggerSystem.h"
#include <algorithm>

namespace CTF::Logging {
    static LoggerSystem* instance_ = nullptr;
    LoggerSystem* LoggerSystem::instance() {
        if (!instance_)
            instance_ = new LoggerSystem();

        return instance_;
    }

    void LoggerSystem::add_listener(ILogListener* listener) {
        Threading::LockGuard lock(mutex_);
        listeners_.push_back(listener);
    }

    void LoggerSystem::remove_listener(ILogListener* listener) {
        Threading::LockGuard lock(mutex_);

        listeners_.erase(
            std::remove(listeners_.begin(), listeners_.end(), listener),
            listeners_.end()
        );
    }

    void LoggerSystem::dispatch(
        LogLevel level,
        const char* category,
        const char* timestamp,
        const char* message
    ) {
        Threading::LockGuard lock(mutex_);

        for (auto* listener : listeners_) {
            if (!listener) continue;
            listener->on_log(level, category, timestamp, message);
        }
    }
}
