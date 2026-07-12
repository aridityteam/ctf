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
#include "NamedPipeStream.h"
#ifdef _WIN32
#   include <Windows.h>
#else
#	include <sys/stat.h>
#	include <fcntl.h>
#	include <cerrno>
#endif

namespace CTF {
	NamedPipeStream::NamedPipeStream(const char* name, bool server) {
#ifdef _WIN32

		if (server)
		{
			read_ = CreateNamedPipeA(
				name,
				PIPE_ACCESS_DUPLEX,
				PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
				1,
				4096,
				4096,
				0,
				nullptr
			);

			write_ = read_;
		}
		else
		{
			read_ = CreateFileA(
				name,
				GENERIC_READ | GENERIC_WRITE,
				0,
				nullptr,
				OPEN_EXISTING,
				0,
				nullptr
			);

			write_ = read_;
		}
#else
		if (server)
		{
			if (mkfifo(name, 0666) != 0 && errno != EEXIST) {
				read_ = write_ = -1;
				return;
			}

			const int fd = ::open(name, O_RDWR);
			read_ = write_ = fd;
		}
		else
		{
			const int fd = ::open(name, O_RDWR);
			read_ = write_ = fd;
		}
#endif
	}

	bool NamedPipeStream::connect() {
#ifdef _WIN32
		return ConnectNamedPipe(read_, nullptr) ||
			GetLastError() == ERROR_PIPE_CONNECTED;
#else
		return read_ != -1;
#endif
	}
}
