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
#include "PipeStream.h"

#ifdef _WIN32
#   include <Windows.h>
#else
#	include <unistd.h>
#	include <errno.h>
#endif

namespace CTF {
	namespace {
#ifdef _WIN32
		inline bool validReadHandle(HANDLE handle) {
			return handle != INVALID_HANDLE_VALUE && handle != nullptr;
		}

		inline bool validWriteHandle(HANDLE handle) {
			return handle != INVALID_HANDLE_VALUE && handle != nullptr;
		}
#else
		inline bool validReadFd(int fd) {
			return fd != -1;
		}

		inline bool validWriteFd(int fd) {
			return fd != -1;
		}
#endif
	}

	PipeStream::PipeStream() noexcept {
#ifdef _WIN32
		read_  = INVALID_HANDLE_VALUE;
		write_ = INVALID_HANDLE_VALUE;
#else
		read_  = -1;
		write_ = -1;
#endif
	}

	PipeStream::PipeStream(
#ifdef _WIN32
		HANDLE r, HANDLE w
#else
		int r, int w
#endif
	) noexcept : read_(r), write_(w) {}

	PipeStream::~PipeStream() {
		close();
	}

	PipeStream::PipeStream(PipeStream&& other) noexcept
		: read_(other.read_), write_(other.write_), state_(other.state_) {
#ifdef _WIN32
		other.read_ = INVALID_HANDLE_VALUE;
		other.write_ = INVALID_HANDLE_VALUE;
#else
		other.read_ = -1;
		other.write_ = -1;
#endif
		other.state_ = StreamState::Good;
	}

	PipeStream& PipeStream::operator=(PipeStream&& other) noexcept {
		if (this == &other) {
			return *this;
		}

		close();

		read_ = other.read_;
		write_ = other.write_;
		state_ = other.state_;

#ifdef _WIN32
		other.read_ = INVALID_HANDLE_VALUE;
		other.write_ = INVALID_HANDLE_VALUE;
#else
		other.read_ = -1;
		other.write_ = -1;
#endif
		other.state_ = StreamState::Good;

		return *this;
	}

	bool PipeStream::create() {
		close();

#ifdef _WIN32
		SECURITY_ATTRIBUTES sa{};
		sa.nLength = sizeof(sa);
		sa.bInheritHandle = TRUE;
		sa.lpSecurityDescriptor = nullptr;

		if (!CreatePipe(&read_, &write_, &sa, 0)) {
			read_ = write_ = INVALID_HANDLE_VALUE;
			state_ = StreamState::Error;
			return false;
		}
		state_ = StreamState::Good;
		return true;
#else
		int fds[2];
		if (::pipe(fds) != 0) {
			state_ = StreamState::Error;
			return false;
		}

		read_  = fds[0];
		write_ = fds[1];
		state_ = StreamState::Good;
		return true;
#endif
	}

	size_t PipeStream::read(void* buffer, size_t size) {
		if (!buffer || size == 0) {
			state_ = StreamState::Error;
			return 0;
		}

#ifdef _WIN32
		if (!validReadHandle(read_)) {
			state_ = StreamState::Error;
			return 0;
		}

		DWORD bytes = 0;
		if (!ReadFile(read_, buffer, (DWORD)size, &bytes, nullptr)) {
			state_ = StreamState::Error;
			return 0;
		}
		state_ = bytes > 0 ? StreamState::Good : StreamState::EOFReached;
		return (size_t)bytes;
#else
		if (!validReadFd(read_)) {
			state_ = StreamState::Error;
			return 0;
		}

		ssize_t result;
		do {
			result = ::read(read_, buffer, size);
		} while (result < 0 && errno == EINTR);

		if (result > 0) {
			state_ = StreamState::Good;
			return static_cast<size_t>(result);
		}

		if (result == 0) {
			state_ = StreamState::EOFReached;
			return 0;
		}

		state_ = StreamState::Error;
		return 0;
#endif
	}

	size_t PipeStream::write(const void* buffer, size_t size) {
		if (!buffer || size == 0) {
			state_ = StreamState::Error;
			return 0;
		}

#ifdef _WIN32
		if (!validWriteHandle(write_)) {
			state_ = StreamState::Error;
			return 0;
		}

		const char* ptr = (const char*)buffer;
		size_t total = 0;

		while (total < size) {
			DWORD written = 0;
			if (!WriteFile(write_, ptr + total, (DWORD)(size - total), &written, nullptr)) {
				state_ = StreamState::Error;
				break;
			}

			if (written == 0) {
				state_ = StreamState::Error;
				break;
			}

			total += written;
		}

		if (total == size) {
			state_ = StreamState::Good;
		}

		return total;
#else
		if (!validWriteFd(write_)) {
			state_ = StreamState::Error;
			return 0;
		}

		const char* ptr = (const char*)buffer;
		size_t total = 0;

		while (total < size) {
			ssize_t result = ::write(write_, ptr + total, size - total);

			if (result < 0) {
				if (errno == EINTR)
					continue;
				state_ = StreamState::Error;
				break;
			}

			if (result == 0) {
				state_ = StreamState::Error;
				break;
			}

			total += result;
		}

		if (total == size) {
			state_ = StreamState::Good;
		}

		return total;
#endif
	}

	bool PipeStream::readline(char* buffer, size_t size) {
		if (!buffer || size == 0) {
			state_ = StreamState::Error;
			return false;
		}

		size_t pos = 0;

		while (pos < size - 1) {
			char ch;
			if (read(&ch, 1) != 1)
				break;

			buffer[pos++] = ch;

			if (ch == '\n')
				break;
		}

		buffer[pos] = '\0';
		if (pos > 0) {
			state_ = StreamState::Good;
		}
		return pos > 0;
	}

	bool PipeStream::valid() const noexcept {
#ifdef _WIN32
		return validReadHandle(read_) || validWriteHandle(write_);
#else
		return validReadFd(read_) || validWriteFd(write_);
#endif
	}

	Stream& PipeStream::writeString(const String& text) {
		const size_t total = text.length();
		if (total == 0) {
			state_ = StreamState::Good;
			return *this;
		}

		if (write(text.c_str(), total) != total) {
			state_ = StreamState::Error;
		}

		return *this;
	}

	Stream& PipeStream::writeChar(char character) {
		if (write(&character, 1) != 1) {
			state_ = StreamState::Error;
		}
		return *this;
	}

	Stream& PipeStream::readString(String& out) {
		char buffer[512];
		if (readline(buffer, sizeof(buffer))) {
			out = buffer;
		} else {
			out.clear();
		}
		return *this;
	}

	Stream& PipeStream::readChar(char& ch) {
		if (read(&ch, 1) != 1) {
			ch = '\0';
		}
		return *this;
	}

	bool PipeStream::seek(CTF::size_t) {
		state_ = StreamState::Error;
		return false;
	}

	CTF::size_t PipeStream::tell() const {
		return 0;
	}

	void PipeStream::flush() {
#ifdef _WIN32
		if (validWriteHandle(write_)) {
			state_ = FlushFileBuffers(write_) ? StreamState::Good : StreamState::Error;
		}
#else
		state_ = StreamState::Good;
#endif
	}

	void PipeStream::close() {
#ifdef _WIN32
		const bool sameEndpoint = validReadHandle(read_) && read_ == write_;

		if (validReadHandle(read_)) {
			CloseHandle(read_);
			read_ = INVALID_HANDLE_VALUE;
		}

		if (validWriteHandle(write_) && !sameEndpoint) {
			CloseHandle(write_);
			write_ = INVALID_HANDLE_VALUE;
		}
		write_ = INVALID_HANDLE_VALUE;
#else
		const bool sameEndpoint = validReadFd(read_) && read_ == write_;

		if (read_ != -1) {
			::close(read_);
			read_ = -1;
		}

		if (write_ != -1 && !sameEndpoint) {
			::close(write_);
			write_ = -1;
		}
		write_ = -1;
#endif
		state_ = StreamState::Good;
	}
}
