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

#ifndef FILESTREAM_H
#define FILESTREAM_H
#pragma once

#include "Stream.h"
#include "stdio.h"
#include <stdexcept>
#include <memory>

namespace CTF {

    class CTF_API FileStream : public Stream {
    public:
        explicit FileStream(FILE* fp, bool takeOwnership = false) noexcept
            : fp_(fp), owns_fp_(takeOwnership) {}

        ~FileStream() override {
            close();
        }

        FileStream(const FileStream&) = delete;
        FileStream& operator=(const FileStream&) = delete;

        FileStream(FileStream&& other) noexcept
            : fp_(other.fp_), owns_fp_(other.owns_fp_) {
            other.fp_ = nullptr;
            other.owns_fp_ = false;
        }

        static std::unique_ptr<FileStream> open(const char* filename, const char* mode) {
            FILE* f = fopen_crossplatform(filename, mode);
            if (!f) throw std::runtime_error("Failed to open file");
            return std::make_unique<FileStream>(f, true);
        }

        size_t read(void* buffer, size_t size) override;
        size_t write(const void* buffer, size_t size) override;

        bool readline(char* buffer, size_t size) override;

        Stream& writeString(const String& text) override;
        Stream& writeChar(char character) override;

        Stream& readString(String& out) override;
        Stream& readChar(char& ch) override;

        StreamState state() const override { return state_; }

        bool seek(CTF::size_t position) override;
        CTF::size_t tell() const override;

        void flush() override;
        void close() override;

    private:
        FILE* fp_{nullptr};
        bool owns_fp_{false};
        StreamState state_{StreamState::Good};

        static FILE* fopen_crossplatform(const char* filename, const char* mode) {
    #ifdef _WIN32
            FILE* file = nullptr;
            fopen_s(&file, filename, mode);
            return file;
    #else
            return fopen(filename, mode);
    #endif
        }
    };

}

#endif //FILESTREAM_H
