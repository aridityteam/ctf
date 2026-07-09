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

#ifndef MATH_POWER_H
#define MATH_POWER_H
#pragma once

#include <concepts>
#include "Algorithm.h"

namespace CTF::Math {
	template <std::floating_point T>
	CTF_CONSTEXPR T Pow(T b, T ex) {
		T result = 1.0;
		T absEx = Abs(ex);
		for (int i = 0; i < absEx; i++) result *= b;
		return (ex < 0) ? 1.0 / result : result;
	}
}

#endif // !MATH_POWER_H
