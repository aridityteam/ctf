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

#ifndef COLOR_H
#define COLOR_H
#pragma once

namespace CTF {
    template<class T = float>
    class [[nodiscard]] Color {
    public:
        constexpr Color(T r, T g, T b, T a = static_cast<T>(1)) noexcept {
            rgba.r_ = r;
            rgba.g_ = g;
            rgba.b_ = b;
            rgba.a_ = a;
        }

        constexpr T r() const noexcept { return rgba.r_; }
        constexpr T g() const noexcept { return rgba.g_; }
        constexpr T b() const noexcept { return rgba.b_; }
        constexpr T a() const noexcept { return rgba.a_; }

        constexpr T& operator[](size_t index) noexcept {
            return data_[index];
        }

        constexpr const T& operator[](size_t index) const noexcept {
            return data_[index];
        }

    private:
        struct RGBA { T r_, g_, b_, a_; };

        union
        {
            RGBA rgba;
            T data_[4];
        };
    };
}

#endif // COLOR_H
