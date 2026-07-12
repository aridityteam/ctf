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

#ifndef __IOSTREAM_H__
#define __IOSTREAM_H__
#pragma once

#include "CTF.h"
#include "OStream.h"
#include "IStream.h"
#include "Stream.h"

namespace CTF {

    extern CTF_API OStream &COut;
    extern CTF_API IStream &CIn;
    extern CTF_API OStream &CErr;
    extern CTF_API OStream &CLog;

    CTF_API Stream &EndLine(Stream &stream);
    CTF_API OStream &EndLine(OStream &stream);

}

#endif //__IOSTREAM_H__
