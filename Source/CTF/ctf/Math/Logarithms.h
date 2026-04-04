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

#ifndef MATH_LOGARITHMS_H
#define MATH_LOGARITHMS_H
#pragma once

#include <concepts>

namespace CTF::Math {
	CTF_CONSTEXPR double Log2 = 0.693147180559945309417;

	template <std::floating_point T>
	CTF_CONSTEXPR T Exp(T x) {
		int k = static_cast<int>(x / Log2);
		T r = x - k * Log2;

		T term = 1;
		T sum = 1;

		for (int n = 1; n < 30; ++n) {
			term *= r / n;
			sum += term;
			if (std::abs(term) < 1e-15) break;
		}

		return std::ldexp(sum, k);
	}

	template <std::floating_point T>
	CTF_CONSTEXPR T Log(T x) {
		int k;
		T m = std::frexp(x, &k); // x = m * 2^k, m in [0.5, 1)

		if (m < T(0.70710678118)) { // ~1/sqrt(2)
			m *= 2;
			k -= 1;
		}

		T y = (m - 1) / (m + 1);
		T y2 = y * y;

		T sum = 0;
		T term = y;

		for (int i = 1; i < 50; i += 2) {
			sum += term / i;
			term *= y2;
		}

		return 2 * sum + k * Log2;
	}

	template <typename T>
	CTF_CONSTEXPR T Log(T x, T b) {
		return Log(x) / Log(b);
	}
}

#endif // !MATH_LOGARITHMS_H
