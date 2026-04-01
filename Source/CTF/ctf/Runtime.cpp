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
#include "Runtime.h"
#include "IOStream.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#if defined(_WIN32)
#  include <process.h> /* _exit on Windows */
#endif

namespace CTF {

    [[noreturn]] void abort() {
        static volatile sig_atomic_t in_abort = 0;
        if (in_abort) {
            _exit(EXIT_FAILURE);
        }
        in_abort = 1;

        raise(SIGABRT);

        CErr << "CTF::abort: Program aborting..." << EndLine;

        _exit(EXIT_FAILURE);
    }

}
