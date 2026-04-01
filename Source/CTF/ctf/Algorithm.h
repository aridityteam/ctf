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

#ifndef ALGORITHM_H
#define ALGORITHM_H
#pragma once



namespace CTF {

	template <typename T>
	CTF_API CTF_CONSTEXPR T Max(T a, T b);

	template <typename T>
	CTF_API CTF_CONSTEXPR const T& Max(const T& a, const T& b);

	template <typename T, typename Compare>
	CTF_API CTF_CONSTEXPR const T& Max(const T& a, const T& b, Compare compare);

	template <typename T>
	CTF_API CTF_CONSTEXPR T Min(T a, T b);

	template <typename T>
	CTF_API CTF_CONSTEXPR const T& Min(const T& a, const T& b);
    
	template <typename T, typename Compare>
	CTF_API CTF_CONSTEXPR const T& Min(const T& a, const T& b, Compare compare);

	template <typename T>
	CTF_API CTF_CONSTEXPR T Clamp(T val, T low, T high);

	template <typename T>
	CTF_API CTF_CONSTEXPR void Swap(T& x, T& y);

	template <typename InputIterator, typename Function>
	CTF_API CTF_CONSTEXPR Function ForEach(InputIterator first, InputIterator last, Function function);
}

#endif //ALGORITHM_H
