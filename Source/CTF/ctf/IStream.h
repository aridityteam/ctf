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

#ifndef ISTREAM_H
#define ISTREAM_H
#pragma once

#include "Stream.h"
#include "BasicString.h"
#include <stdexcept>

namespace CTF {

    class IStream {
    public:
        explicit IStream(Stream& stream) noexcept
            : s_(stream) {}

        IStream() = delete;
        IStream(IStream&&) = default;
        IStream& operator=(IStream&&) = delete;

        ~IStream() = default;

        IStream& operator>>(String& out) {
            char buffer[512];

            if (!s_.readline(buffer, sizeof(buffer))) {
                throw std::runtime_error("Failed to read line");
            }

            out = buffer;
            return *this;
        }

        IStream& operator>>(char& ch) {
            if (s_.read(&ch, 1) != 1) {
                throw std::runtime_error("Failed to read character");
            }
            return *this;
        }

        void flush() {
            s_.flush();
        }

        Stream& underlying() noexcept { return s_; }

    private:
        Stream& s_;
    };

}

#endif //ISTREAM_H