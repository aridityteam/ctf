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
#include <PipeStream.h>
#include <gtest/gtest.h>

using namespace CTF;

TEST(PipeStreamTest, WriteAndRead) {
    PipeStream stream;
    ASSERT_TRUE(stream.create());
    ASSERT_TRUE(stream.valid());

    stream << "abc\n";
    char buffer[16] = { 0 };
    ASSERT_TRUE(stream.readline(buffer, sizeof(buffer)));
    ASSERT_EQ(A_strcmp(buffer, "abc\n"), 0);

    ASSERT_FALSE(stream.seek(0));
    ASSERT_EQ(stream.state(), StreamState::Error);

    stream.close();
    ASSERT_FALSE(stream.valid());
}

TEST(PipeStreamTest, AnonymousWriteAndRead) {
    AnonymousPipeStream pipe;
    PipeStream& reader = pipe.reader();
    PipeStream& writer = pipe.writer();

    ASSERT_TRUE(reader.valid());
    ASSERT_TRUE(writer.valid());

    const char payload[] = "ping";
    const size_t wrote = writer.write(payload, sizeof(payload) - 1);
    ASSERT_EQ(wrote, sizeof(payload) - 1);

    char buffer[8] = { 0 };
    const size_t got = reader.read(buffer, sizeof(payload) - 1);
    ASSERT_EQ(got, sizeof(payload) - 1);
    ASSERT_EQ(A_memcmp(buffer, payload, sizeof(payload) - 1), 0);

    const char* line = "hello pipe\n";
    ASSERT_EQ(writer.write(line, A_strlen(line)), A_strlen(line));

    char lineBuffer[64] = { 0 };
    ASSERT_TRUE(reader.readline(lineBuffer, sizeof(lineBuffer)));
    ASSERT_EQ(A_strcmp(lineBuffer, line), 0);

    writer.close();
    char ch = '\0';
    const size_t eofRead = reader.read(&ch, 1);
    ASSERT_EQ(eofRead, 0);
    ASSERT_EQ(reader.state(), StreamState::EOFReached);

    const size_t invalidRead = writer.read(&ch, 1);
    ASSERT_EQ(invalidRead, 0);
    ASSERT_EQ(writer.state(), StreamState::Error);

    const char x = 'x';
    const size_t invalidWrite = reader.write(&x, 1);
    ASSERT_EQ(invalidWrite, 0);
    ASSERT_EQ(reader.state(), StreamState::Error);
}