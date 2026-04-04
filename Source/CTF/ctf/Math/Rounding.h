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

#ifndef MATH_ROUNDING_H
#define MATH_ROUNDING_H
#pragma once

#include <concepts>

namespace CTF::Math {
	template <std::floating_point T>
	CTF_CONSTEXPR T Ceil(T x) {
		long long truncated = static_cast<long long>(x);
		if (static_cast<T>(truncated) == x) return x;
		if (x > 0) return static_cast<T>(truncated + 1);
		return static_cast<T>(truncated);
	}

	template <std::floating_point T>
	CTF_CONSTEXPR T Floor(T x) {
		long long integer = static_cast<long long>(x);
		T result = static_cast<T>(truncated);
		if (x > 0 && x != result) return result - 1;
		return result;
	}
}

#endif // !MATH_ROUNDING_H
