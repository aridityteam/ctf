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
#include "Logger.h"
#include "LoggerSystem.h"
#include "ColorUtil.h"
#include "IOStream.h"
#include <cstdarg>
#include <chrono>
#include <ctime>
#include <time.h>

namespace CTF::Logging {
    Logger::Logger(const char* category) :
        m_pszCategory(category) {
        // warm up the logger system
        LoggerSystem::instance();
    }

    Logger::~Logger() = default;

    void Logger::log(const LogLevel level, const char* format, ...) const {
        std::va_list args;
        va_start(args, format);
        vlog(level, format, args);
        va_end(args);
    }

    void Logger::vlog(const LogLevel level, const char* format, va_list args) const {
        char message[4096];
        vsnprintf(message, sizeof(message), format, args);

        const char* timestamp = get_timestamp();
        LoggerSystem::instance()->dispatch(
            level,
            m_pszCategory,
            timestamp,
            message
        );
    }

    const char* Logger::get_timestamp() {
        static char buffer[20]; // "YYYY-MM-DD HH:MM:SS" = 19 + null terminator

        const auto now = std::chrono::system_clock::now();
        std::time_t in_time_t = std::chrono::system_clock::to_time_t(now);

        std::tm time_info{};

#ifdef _WIN32
        localtime_s(&time_info, &in_time_t);
#else
        localtime_r(&in_time_t, &time_info);
#endif

        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &time_info);
        return buffer;
    }

    const char* Logger::level_to_str(const LogLevel level) {
        const char* final = nullptr;

        switch (level)
        {
        case LogLevel::info:
            final = "INFO";
            break;
        case LogLevel::warn:
            final = "WARN";
            break;
        case LogLevel::err:
            final = "ERROR";
            break;

        default:
            final = "UNKNOWN";
            break;
        }

        return final;
    }

    void Logger::info(const char* format, ...) const {
        std::va_list args;
        va_start(args, format);
        vlog(LogLevel::info, format, args);
        va_end(args);
    }

    void Logger::warn(const char* format, ...) const {
        std::va_list args;
        va_start(args, format);
        vlog(LogLevel::warn, format, args);
        va_end(args);
    }

    void Logger::err(const char* format, ...) const {
        std::va_list args;
        va_start(args, format);
        vlog(LogLevel::err, format, args);
        va_end(args);
    }
}
