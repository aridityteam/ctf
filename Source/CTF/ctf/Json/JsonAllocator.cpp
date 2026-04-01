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
#include "JsonAllocator.h"

namespace CTF::Json {
	JsonAllocator::JsonAllocator(int size) {
		memory = new char[size];
		capacity = size;
		offset = 0;
	}

	JsonAllocator::~JsonAllocator() {
		delete[] memory;
	}

	void* JsonAllocator::Allocate(int size) {
		if (offset + size > capacity)
			return nullptr;

		void* ptr = memory + offset;
		offset += size;

		return ptr;
	}
}