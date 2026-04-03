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

#ifndef OSTREAM_H
#define OSTREAM_H
#pragma once

#include "Stream.h"
#include "BasicString.h"

namespace CTF {

    class OStream;

    using Manipulator = OStream& (*)(OStream&);

    class OStream {
    public:
        explicit OStream(Stream& stream) noexcept
            : s_(stream) {}

        OStream(OStream&&) = default;
        OStream& operator=(OStream&&) = delete;

        OStream() = delete;

        ~OStream() = default;

        OStream& operator<<(const String& text) {
            s_.operator<<(text);
            return *this;
        }

        OStream& operator<<(const char* text) {
            s_.operator<<(text);
            return *this;
        }

        OStream& operator<<(char c) {
            s_.operator<<(c);
            return *this;
        }

        OStream& operator<<(Manipulator m) {
            return m(*this);
        }

        void flush() {
            s_.flush();
        }

        Stream& underlying() noexcept { return s_; }

    private:
        Stream& s_;
    };

}

#endif //OSTREAM_H