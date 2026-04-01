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

#include <CTF.h>
#include <AnonymousPipeStream.h>
#include <IOStream.h>
#include <OStringStream.h>
#include <PipeStream.h>

#include <cstring>

using namespace CTF;

namespace {
    bool AssertOrFail(bool condition, const char* message) {
        if (!condition) {
            CErr << message << EndLine;
            return false;
        }
        return true;
    }
}

int main() {
    {
        OStringStream s;
        s << "line" << EndLine;
        if (!AssertOrFail(s.str() == "line\n", "EndLine manipulator failed on Stream/OStringStream")) return -1;
    }

    {
        AnonymousPipeStream pipe;
        PipeStream& reader = pipe.reader();
        PipeStream& writer = pipe.writer();

        if (!AssertOrFail(reader.valid(), "AnonymousPipeStream reader should be valid")) return -1;
        if (!AssertOrFail(writer.valid(), "AnonymousPipeStream writer should be valid")) return -1;

        const char payload[] = "ping";
        const size_t wrote = writer.write(payload, sizeof(payload) - 1);
        if (!AssertOrFail(wrote == sizeof(payload) - 1, "Failed to write payload into pipe")) return -1;

        char buffer[8] = {0};
        const size_t got = reader.read(buffer, sizeof(payload) - 1);
        if (!AssertOrFail(got == sizeof(payload) - 1, "Failed to read payload from pipe")) return -1;
        if (!AssertOrFail(Q_memcmp(buffer, payload, sizeof(payload) - 1) == 0, "Pipe payload mismatch")) return -1;

        const char* line = "hello pipe\n";
        if (!AssertOrFail(writer.write(line, Q_strlen(line)) == Q_strlen(line), "Failed to write newline payload")) return -1;

        char lineBuffer[64] = {0};
        if (!AssertOrFail(reader.readline(lineBuffer, sizeof(lineBuffer)), "readline failed")) return -1;
        if (!AssertOrFail(Q_strcmp(lineBuffer, line) == 0, "readline payload mismatch")) return -1;

        writer.close();
        char ch = '\0';
        const size_t eofRead = reader.read(&ch, 1);
        if (!AssertOrFail(eofRead == 0, "Expected EOF read size 0 after writer close")) return -1;
        if (!AssertOrFail(reader.state() == StreamState::EOFReached, "Reader state should be EOF after writer close")) return -1;

        const size_t invalidRead = writer.read(&ch, 1);
        if (!AssertOrFail(invalidRead == 0, "Writer-only stream read should fail")) return -1;
        if (!AssertOrFail(writer.state() == StreamState::Error, "Writer-only stream read should set Error state")) return -1;

        const char x = 'x';
        const size_t invalidWrite = reader.write(&x, 1);
        if (!AssertOrFail(invalidWrite == 0, "Reader-only stream write should fail")) return -1;
        if (!AssertOrFail(reader.state() == StreamState::Error, "Reader-only stream write should set Error state")) return -1;
    }

    {
        PipeStream stream;
        if (!AssertOrFail(stream.create(), "PipeStream::create failed")) return -1;
        if (!AssertOrFail(stream.valid(), "PipeStream should be valid after create")) return -1;

        stream << "abc\n";
        char buffer[16] = {0};
        if (!AssertOrFail(stream.readline(buffer, sizeof(buffer)), "PipeStream readline after operator<< failed")) return -1;
        if (!AssertOrFail(Q_strcmp(buffer, "abc\n") == 0, "PipeStream operator<< payload mismatch")) return -1;

        if (!AssertOrFail(!stream.seek(0), "PipeStream seek should fail")) return -1;
        if (!AssertOrFail(stream.state() == StreamState::Error, "PipeStream seek failure should set Error state")) return -1;

        stream.close();
        if (!AssertOrFail(!stream.valid(), "PipeStream should be invalid after close")) return -1;
    }

    return 0;
}
