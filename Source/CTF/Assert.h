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

// @PracticeMedicine: 
// if you dont want the more hardcore approach then its better of using functions from validation.h

#ifndef ASSERT_H
#define ASSERT_H
#pragma once

#include "IOStream.h"
#include "Runtime.h"

#if !defined(NDEBUG) || defined(_DEBUG)
    #define ASSERT(condition, message)                             \
        do {                                                       \
            if (!(condition)) {                                    \
                CTF::CErr << "Assertion failed: " #condition       \
                          << ", at file " << __FILE__                 \
                          << ", at line " << __LINE__ << CTF::EndLine;   \
                CTF::CErr << "Message: " << message << CTF::EndLine;  \
                CTF::abort();                                      \
            }                                                      \
        } while (0)

    #define COMPILE_TIME_ASSERT(condition, message) static_assert(condition, message)
#else
	#define ASSERT(condition, message) ((void)0)
    #define COMPILE_TIME_ASSERT(condition, message) ((void)0)
#endif // NDEBUG,_DEBUG

#endif // ASSERT_H
