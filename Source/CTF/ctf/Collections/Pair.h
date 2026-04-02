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

#ifndef PAIR_H
#define PAIR_H
#pragma once

#include <utility>

namespace CTF::Collections {

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
    template<typename First, typename Second>
    struct Pair {
        First first;
        Second second;

        Pair(const First& f, const Second& s)
            : first(f), second(s) {}

        Pair(First&& f, Second&& s)
            : first(std::move(f)), second(std::move(s)) {}

        template<typename F = First, typename S = Second>
        Pair(F&& f, S&& s)
            : first(std::forward<F>(f)), second(std::forward<S>(s)) {}
    };
#if defined(__clang__)
#	pragma clang diagnostic pop
#elif defined(__GNUC__)
#	pragma GCC diagnostic pop
#else
#	pragma warning(pop)
#endif

} // namespace CTF::Collections

#endif // PAIR_H
