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

#ifndef UTILITY_H
#define UTILITY_H
#pragma once



namespace CTF {
    // @ThePixelMoon: i don't like this. i don't want this.
    //				  i don't know why i'm doing this.
    //				  but it's for the fucking move function
    template<typename T> struct RemoveReference      { using Type = T; };
    template<typename T> struct RemoveReference<T&>  { using Type = T; };
    template<typename T> struct RemoveReference<T&&> { using Type = T; };

    template<typename T>
    CTF_CONSTEXPR RemoveReference<T>::Type&& Move(T&& t) {
        return static_cast<RemoveReference<T>::Type&&>(t);
    }
}

#endif //UTILITY_H
