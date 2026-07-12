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

#ifndef STREAM_H
#define STREAM_H
#pragma once


#include "BasicString.h"

namespace CTF {

    class Stream;
    using StreamManipulator = Stream& (*)(Stream&);

    enum class StreamState {
        Good,
        EOFReached,
        Error
    };

    class CTF_API Stream {
    public:
        virtual ~Stream() = default;

        virtual CTF::size_t read(void* buffer, CTF::size_t size) = 0;
        virtual CTF::size_t write(const void* buffer, CTF::size_t size) = 0;

        virtual bool readline(char* buffer, size_t size) = 0;

        virtual Stream& writeString(const CTF::String& text) = 0;
        virtual Stream& writeChar(char character) = 0;

        virtual Stream& readString(CTF::String& out) = 0;
        virtual Stream& readChar(char& ch) = 0;

        virtual Stream& operator<<(const CTF::String& text) {
            return writeString(text);
        }

        virtual Stream& operator<<(const char* text) {
            if (!text) {
                return *this;
            }

            return writeString(CTF::String(text));
        }

        virtual Stream& operator<<(char ch) {
            return writeChar(ch);
        }

        virtual Stream& operator<<(StreamManipulator m) {
            return m(*this);
        }

        virtual Stream& operator>>(CTF::String& out) {
            return readString(out);
        }

        virtual Stream& operator>>(char& ch) {
            return readChar(ch);
        }

        virtual StreamState state() const = 0;
        virtual bool good() const { return state() == StreamState::Good; }
        virtual bool eof() const { return state() == StreamState::EOFReached; }

        virtual bool seek(CTF::size_t position) = 0;
        virtual CTF::size_t tell() const = 0;

        virtual void flush() = 0;
        virtual void close() = 0;
    };

}

#endif //STREAM_H
