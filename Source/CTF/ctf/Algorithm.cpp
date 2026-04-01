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
#include "Algorithm.h"

namespace CTF {
	template <typename T>
	CTF_CONSTEXPR T Min(T a, T b) {
		return b < a ? b : a;
	}

	template <typename T>
	CTF_CONSTEXPR const T &Min(const T &a, const T &b) {
		return b < a ? b : a;
	}

	template <typename T, typename Compare>
	CTF_CONSTEXPR const T& Min(const T& a, const T& b, Compare compare) {
		return compare(b, a) ? b : a;
	}

	template <typename T>
	CTF_CONSTEXPR T Max(T a, T b) {
		return a < b ? b : a;
	}

	template <typename T>
	CTF_CONSTEXPR const T &Max(const T &a, const T &b) {
		return a < b ? b : a;
	}

	template <typename T, typename Compare>
	CTF_CONSTEXPR const T& Max(const T& a, const T& b, Compare compare)
	{
		return compare(a, b) ? b : a;
	}

	template <typename T>
	CTF_CONSTEXPR T Clamp(T val, T low, T high) {
		return Min(Max(val, low), high);
	}

	template <typename T>
	CTF_CONSTEXPR void Swap(T& x, T& y) {
		T temp = x;
		x = y;
		y = temp;
	}

	template <typename InputIterator, typename Function>
	CTF_CONSTEXPR Function ForEach(InputIterator first, InputIterator last, Function function) {
		for(; first != last; ++first)
			function(*first);
		return function;
	}
}
