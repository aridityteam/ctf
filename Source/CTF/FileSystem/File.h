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

#ifndef FILE_H
#define FILE_H
#pragma once

#include "FileStream.h"
#include "Path.h"


namespace CTF::FileSystem {

    class CTF_API File {
    public:
        explicit File(Path& path);
        ~File() = default;

        bool exists() { return path_.file_exists(); }
        const String& get_name() { return path_.get_name(); }
        CTF::FileStream *open(const char *mode);

    private:
        Path path_;
    };

}

#endif // FILE_H
