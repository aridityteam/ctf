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
#include "Path.h"
#ifdef _WIN32
    #include <Windows.h>
#else
    #include <sys/stat.h>
#endif

namespace CTF::FileSystem {
    static char* combine_path(const char* base, const char* relative) {
        if (!base) base = "";
        if (!relative) relative = "";

        size_t len_base = Q_strlen(base);
        size_t len_rel = Q_strlen(relative);

        char* result = (char*)malloc(len_base + len_rel + 2);
        if (!result) return NULL;

        Q_strcpy(result, base);

        if (len_base > 0 && result[len_base - 1] != PATH_SEP) {
            result[len_base] = PATH_SEP;
            result[len_base + 1] = '\0';
        }

        if (len_rel > 0 && relative[0] == PATH_SEP) {
            Q_strcat(result, relative + 1);
        }
        else {
            Q_strcat(result, relative);
        }

        return result;
    }

    Path::Path(const char* path) :
        path_(path) {}

    Path::Path(String& path) :
        path_(path) {}

    Path::~Path() = default;

    Path Path::combine(String& path) {
        return combine_path(path_, path);
    }

    Path Path::combine(const char *path) {
        return combine_path(path_, path);
    }

    Path Path::operator=(const char* path) {
        return Path(path);
    }
    Path Path::operator=(String& path) {
        return Path(path.c_str());
    }

    Path Path::operator/(const char* path) {
        return combine(path);
    }
    Path Path::operator/(String& path) {
        return combine(path);
    }

    bool Path::file_exists() {
#ifdef _WIN32
        DWORD fileAttributes = GetFileAttributesA(path_.c_str());
        return fileAttributes != INVALID_FILE_ATTRIBUTES;
#else
        struct stat buffer;
        return (stat(path_.c_str(), &buffer) == 0);
#endif
    }

    bool Path::dir_exists() {
#ifdef _WIN32
        DWORD fileAttributes = GetFileAttributesA(path_.c_str());
        return (fileAttributes != INVALID_FILE_ATTRIBUTES) &&
            (fileAttributes & FILE_ATTRIBUTE_DIRECTORY);
#else
        struct stat buffer;
        return (stat(path_.c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode));
#endif
    }

    const String& Path::get_name() {
        static String name;

        const char* path = path_;
        if (!path || !*path) {
            name.clear();
            return name;
        }

        const char* last = path;

        for (const char* p = path; *p; ++p) {
            if (*p == '/' || *p == '\\') {
                last = p + 1;
            }
        }

        name = last;
        return name;
    }
}
