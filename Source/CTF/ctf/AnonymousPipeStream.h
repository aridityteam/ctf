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

#ifndef CTF_ANONYMOUSPIPESTREAM_H
#define CTF_ANONYMOUSPIPESTREAM_H
#pragma once

#include "PipeStream.h"

namespace CTF {
	class CTF_API AnonymousPipeStream {
	public:
		AnonymousPipeStream();
		~AnonymousPipeStream();

		PipeStream& reader();
		PipeStream& writer();

	private:
		PipeStream read_stream_;
		PipeStream write_stream_;
	};
}

#endif // !CTF_ANONYMOUSPIPESTREAM_H
