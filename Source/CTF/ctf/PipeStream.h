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

#ifndef CTF_PIPESTREAM_H
#define CTF_PIPESTREAM_H
#pragma once

#include "CTF.h"
#include "Stream.h"

#ifdef _WIN32
#   include <Windows.h>
#endif // _WIN32

namespace CTF {
	class CTF_API PipeStream : public Stream {
	public:
		PipeStream() noexcept;

	#ifdef _WIN32
		PipeStream(HANDLE readHandle, HANDLE writeHandle) noexcept;
	#else
		PipeStream(int readFd, int writeFd) noexcept;
	#endif

		~PipeStream() override;
		PipeStream(const PipeStream&) = delete;
		PipeStream& operator=(const PipeStream&) = delete;
		PipeStream(PipeStream&& other) noexcept;
		PipeStream& operator=(PipeStream&& other) noexcept;

		bool create();

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

		bool valid() const noexcept;

	protected:
	#ifdef _WIN32
		HANDLE read_{INVALID_HANDLE_VALUE};
		HANDLE write_{INVALID_HANDLE_VALUE};
	#else
		int read_{-1};
		int write_{-1};
	#endif
		StreamState state_{StreamState::Good};
	};
}

#endif // !CTF_PIPESTREAM_H
