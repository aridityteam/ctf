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

#ifndef CTF_MEMORY_H
#define CTF_MEMORY_H
#pragma once

#include "CTF.h"

namespace CTF {
    class CTF_API Memory
    {
    public:
        static void* Alloc(size_t size);
        static void* Calloc(size_t count, size_t size);
        static void Free(void* ptr);

        static void* Realloc(void* ptr, size_t newSize);

        static void* AllocAligned(size_t size, size_t alignment);
        static void FreeAligned(void* ptr);
    };

    class CTF_API LinearArena
    {
    public:
        LinearArena(size_t size);
        ~LinearArena();

        void* Alloc(size_t size);
        void Reset();

    private:
        char* m_buffer;
        size_t m_capacity;
        size_t m_offset;
    };
}

#endif // !CTF_MEMORY_H