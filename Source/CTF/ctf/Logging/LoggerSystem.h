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

#ifndef CTF_LOGGERSYSTEM_H
#define CTF_LOGGERSYSTEM_H
#pragma once

#include "LogListener.h"
#include "Threading/Mutex.h"
#include "Collections/Vector.h"

namespace CTF::Logging {
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
    class CTF_API LoggerSystem {
    public:
        static LoggerSystem* instance();

        void add_listener(ILogListener* listener);
        void remove_listener(ILogListener* listener);

        void dispatch(
            LogLevel level,
            const char* category,
            const char* timestamp,
            const char* message
        );

    private:
        Collections::Vector<ILogListener*> listeners_;
        Threading::Mutex mutex_;
    };
#if defined(__clang__)
#	pragma clang diagnostic pop
#elif defined(__GNUC__)
#	pragma GCC diagnostic pop
#else
#	pragma warning(pop)
#endif
}

#endif // !CTF_LOGGERSYSTEM_H
