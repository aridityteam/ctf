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
#include "FileStream.h"
#include <cstdio>

#ifdef _WIN32
#   ifdef fscanf
#       undef fscanf
#   endif // fscanf
#   define fscanf fscanf_s
#endif // _WIN32

namespace CTF {

    size_t FileStream::read(void* buffer, size_t size) {
        if (!fp_ || !buffer || size == 0) {
            state_ = StreamState::Error;
            return 0;
        }

        const size_t bytes = fread(buffer, 1, size, fp_);

        if (bytes == size) {
            state_ = StreamState::Good;
            return bytes;
        }

        if (feof(fp_)) {
            state_ = StreamState::EOFReached;
        } else if (ferror(fp_)) {
            state_ = StreamState::Error;
        } else {
            state_ = StreamState::Good;
        }

        return bytes;
    }

    size_t FileStream::write(const void* buffer, size_t size) {
        if (!fp_ || !buffer || size == 0) {
            state_ = StreamState::Error;
            return 0;
        }

        const size_t bytes = fwrite(buffer, 1, size, fp_);
        state_ = (bytes == size) ? StreamState::Good : StreamState::Error;
        return bytes;
    }

    bool FileStream::readline(char* buffer, size_t size) {
        if (!fp_ || !buffer || size == 0) {
            state_ = StreamState::Error;
            return false;
        }

        if (fgets(buffer, static_cast<int>(size), fp_) != nullptr) {
            state_ = StreamState::Good;
            return true;
        }

        state_ = feof(fp_) ? StreamState::EOFReached : StreamState::Error;
        return false;
    }

    Stream& FileStream::writeString(const String& text) {
        if (!fp_) {
            state_ = StreamState::Error;
            return *this;
        }

        const size_t length = text.length();
        if (length == 0) {
            state_ = StreamState::Good;
            return *this;
        }

        const size_t bytes = write(text.c_str(), length);
        state_ = (bytes == length) ? StreamState::Good : StreamState::Error;
        return *this;
    }

    Stream& FileStream::writeChar(char character) {
        if (!fp_) {
            state_ = StreamState::Error;
            return *this;
        }

        state_ = (fputc(character, fp_) == EOF) ? StreamState::Error : StreamState::Good;
        return *this;
    }

    Stream& FileStream::readString(String& out) {
        char buffer[512];

        if (!fp_) {
            out.clear();
            state_ = StreamState::Error;
            return *this;
        }

        if (fscanf(fp_, "%511s", buffer) == 1) {
            out = buffer;
            state_ = StreamState::Good;
        } else {
            out.clear();
            state_ = feof(fp_) ? StreamState::EOFReached : StreamState::Error;
        }

        return *this;
    }

    Stream& FileStream::readChar(char& ch) {
        if (!fp_) {
            ch = '\0';
            state_ = StreamState::Error;
            return *this;
        }

        const int c = fgetc(fp_);
        if (c == EOF) {
            ch = '\0';
            state_ = feof(fp_) ? StreamState::EOFReached : StreamState::Error;
            return *this;
        }

        ch = static_cast<char>(c);
        state_ = StreamState::Good;
        return *this;
    }

    bool FileStream::seek(CTF::size_t position) {
        if (!fp_) {
            state_ = StreamState::Error;
            return false;
        }

        clearerr(fp_);
        if (fseek(fp_, static_cast<long>(position), SEEK_SET) != 0) {
            state_ = StreamState::Error;
            return false;
        }

        state_ = StreamState::Good;
        return true;
    }

    CTF::size_t FileStream::tell() const {
        if (!fp_) {
            return 0;
        }

        const long pos = ftell(fp_);
        return pos < 0 ? 0 : static_cast<CTF::size_t>(pos);
    }

    void FileStream::flush() {
        if (!fp_) {
            state_ = StreamState::Error;
            return;
        }

        state_ = (fflush(fp_) == 0) ? StreamState::Good : StreamState::Error;
    }

    void FileStream::close() {
        if (!fp_) {
            state_ = StreamState::Good;
            return;
        }

        if (owns_fp_) {
            if (fclose(fp_) != 0) {
                state_ = StreamState::Error;
                return;
            }
        }

        fp_ = nullptr;
        owns_fp_ = false;
        state_ = StreamState::Good;
    }

}
