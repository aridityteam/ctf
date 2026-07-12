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

#ifndef OSTRINGSTREAM_H
#define OSTRINGSTREAM_H
#pragma once

#include "Stream.h"
#include "BasicString.h"

namespace CTF {
    
    class OStringStream : public Stream {
    public:
        OStringStream() = default;
        ~OStringStream() override = default;

        size_t read(void*, size_t) override {
            state_ = StreamState::Error;
            return 0;
        }

        size_t write(const void* buffer, size_t size) override {
            if (!buffer || size == 0) {
                state_ = StreamState::Error;
                return 0;
            }

            const char* ptr = static_cast<const char*>(buffer);
            for (size_t i = 0; i < size; ++i) {
                buffer_ += String(ptr[i]);
            }

            state_ = StreamState::Good;
            return size;
        }

        bool readline(char*, size_t) override {
            state_ = StreamState::Error;
            return false;
        }

        Stream& writeString(const String& text) override {
            buffer_ += text;
            state_ = StreamState::Good;
            return *this;
        }

        Stream& writeChar(char c) override {
            buffer_ += String(c);
            state_ = StreamState::Good;
            return *this;
        }

        Stream& readString(String& out) override {
            out.Clear();
            state_ = StreamState::Error;
            return *this;
        }

        Stream& readChar(char& ch) override {
            ch = '\0';
            state_ = StreamState::Error;
            return *this;
        }

        StreamState state() const override { return state_; }

        bool seek(CTF::size_t position) override {
            if (position > buffer_.Length()) {
                state_ = StreamState::Error;
                return false;
            }

            state_ = StreamState::Good;
            return true;
        }

        CTF::size_t tell() const override {
            return buffer_.Length();
        }

        void flush() override {
            state_ = StreamState::Good;
        }

        void close() override {
            state_ = StreamState::Good;
        }

        const String& str() const noexcept { return buffer_; }

    private:
        String buffer_;
        StreamState state_{StreamState::Good};
    };

}

#endif //OSTRINGSTREAM_H
