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

#ifndef MATH_TRIGONOMETRY_H
#define MATH_TRIGONOMETRY_H
#pragma once

#include <concepts>

namespace CTF::Math {
	template <std::floating_point T>
	CTF_CONSTEXPR T Sin(T x) {
		const T pi = std::numbers::pi_v<T>;
		const T twoPi = T(2) * pi;

		x = std::fmod(x, twoPi);

		if (x > pi)
			x -= twoPi;
		else if (x < -pi)
			x += twoPi;

		if (x > pi / 2)
			x = pi - x;
		else if (x < -pi / 2)
			x = -pi - x;

		T term = x;
		T sum = x;
		T sq = x * x;

		for (int i = 1; i <= 20; i++) {
			term *= -sq / ((2 * i) * (2 * i + 1));
			sum += term;
		}

		return sum;
	}

	template <std::floating_point T>
	CTF_CONSTEXPR T Cos(T x) {
		const T pi = std::numbers::pi_v<T>;
		while (x < 0) x += 2 * pi;
		x = std::fmod(x, T(2) * T(pi));

		T term = 1;
		T sum = 1;

		for (int i = 1; i <= 20; i++) {
			term *= -x * x / ((2 * i) * (2 * i - 1));
			sum += term;

			if (std::abs(term) < std::numeric_limits<T>::epsilon()) break;
		}

		return sum;
	}

	template <std::floating_point T>
	CTF_CONSTEXPR T Tan(T x) {
		return Sin(x) / Cos(x);
	}
}

#endif // !MATH_TRIGONOMETRY_H
