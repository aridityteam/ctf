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
#include "AnonymousPipeStream.h"
#ifdef _WIN32
#   include <Windows.h>
#else
#	include <unistd.h>
#endif
#include "Utility.h"

namespace CTF {
	AnonymousPipeStream::AnonymousPipeStream() {
#ifdef _WIN32
		HANDLE r, w;

		SECURITY_ATTRIBUTES sa{};
		sa.nLength = sizeof(sa);
		sa.bInheritHandle = TRUE;

		if (!CreatePipe(&r, &w, &sa, 0)) {
			return;
		}

		read_stream_  = Move(PipeStream(r, INVALID_HANDLE_VALUE));
		write_stream_ = Move(PipeStream(INVALID_HANDLE_VALUE, w));
#else
		int fds[2];

		if (::pipe(fds) == 0) {
			// fds[0] = read end
			// fds[1] = write end

			read_stream_  = Move(PipeStream(fds[0], -1));
			write_stream_ = Move(PipeStream(-1, fds[1]));
		}
#endif
	}

	AnonymousPipeStream::~AnonymousPipeStream() {}

	PipeStream& AnonymousPipeStream::reader() {
		return read_stream_;
	}

	PipeStream& AnonymousPipeStream::writer() {
		return write_stream_;
	}
}
