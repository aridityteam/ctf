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

#ifndef CTF_LOGGER_H
#define CTF_LOGGER_H
#pragma once

#include <cstdarg>

namespace CTF::Logging {

    enum class LogLevel {
        none = 0,
        debug,
        info,
        warn,
        err
    };

    class CTF_API Logger {
    public:
        explicit Logger(const char *category);
        ~Logger();

        void info(const char *format, ...) const;
        void warn(const char *format, ...) const;
        void err(const char *format, ...) const;

        void log(LogLevel level, const char* format, ...) const;
        void vlog(LogLevel level, const char* format, va_list args) const;

        static const char *level_to_str(LogLevel level);

    private:
        static const char *get_timestamp();

        const char *m_pszCategory;
    };

}

#endif // CTF_LOGGER_H
