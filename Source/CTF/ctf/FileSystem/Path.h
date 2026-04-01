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

#ifndef PATH_H
#define PATH_H
#pragma once

#include "String.h"

namespace CTF::FileSystem {

class CTF_API Path {
public:
    Path(const char *path);
    Path(String &path);
    ~Path();

    bool file_exists();
    bool dir_exists();
    const String& str() { return path_; }
    const String& get_name();

    Path combine(String &path);
    Path combine(const char *path);

    Path operator=(String &path);
    Path operator=(const char *path);
    Path operator/(String &path);
    Path operator/(const char *path);

    operator const String&() { return path_; }

private:
    String path_;
};

} // namespace CTF::FileSystem


#endif // PATH_H
