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
#include "Memory.h"

namespace CTF {
    void* Memory::Alloc(size_t size)
    {
        return std::malloc(size);
    }

    void* Memory::Calloc(size_t count, size_t size)
    {
        return std::calloc(count, size);
    }

    void* Memory::Realloc(void *ptr, size_t size)
    {
        return std::realloc(ptr, size);
    }

    void Memory::Free(void* ptr)
    {
        std::free(ptr);
    }

    void* Memory::AllocAligned(size_t size, size_t alignment)
    {
#ifdef _WIN32
        return _aligned_malloc(size, alignment);
#else
        void* ptr = nullptr;
        if (posix_memalign(&ptr, alignment, size) != 0)
            return nullptr;
        return ptr;
#endif
    }

    void Memory::FreeAligned(void* ptr)
    {
#ifdef _WIN32
        _aligned_free(ptr);
#else
        std::free(ptr);
#endif
    }

    LinearArena::LinearArena(size_t size)
    {
        m_buffer = (char*)Memory::Alloc(size);
        m_capacity = size;
        m_offset = 0;
    }

    LinearArena::~LinearArena()
    {
        Memory::Free(m_buffer);
    }

    void* LinearArena::Alloc(size_t size)
    {
        size_t alignedSize = (size + 7) & ~7ULL;

        if (m_offset + alignedSize > m_capacity)
            return nullptr;

        void* ptr = m_buffer + m_offset;
        m_offset += alignedSize;
        return ptr;
    }

    void LinearArena::Reset()
    {
        m_offset = 0;
    }
}
