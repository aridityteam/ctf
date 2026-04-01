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

namespace CTF {

    //===============================================================================
    // basic string manipulation and examination funcs start here!!

    char *A_strcpy(char* dest, const char* src) {
        while (*src != '\0') {
            *dest = *src; // copy the character
            dest++;
            src++;
        }

        *dest = '\0';
        return dest;
    }

    char *A_strncpy(char* dest, const char* src, size_t n) {
        size_t i;
        for (i = 0; i < n && *src != '\0'; i++) {
            *dest = *src; // copy the character
            dest++;
            src++;
        }

        for (; i < n; i++) {
            *dest = '\0';
            dest++;
        }

        return dest;
    }

    char *A_strcat(char* dest, const char* src) {
        while (*dest != '\0') {
            dest++;
        }

        while (*src != '\0') {
            *dest = *src;
            dest++;
            src++;
        }
        
        *dest = '\0'; // null-terminate the result
        return dest;
    }

    char *A_strncat(char* dest, const char* src, size_t n) {
        while (*dest != '\0') {
            dest++;
        }

        size_t i;
        for (i = 0; i < n && *src != '\0'; i++) {
            *dest = *src;
            dest++;
            src++;
        }

        *dest = '\0'; // null-terminate the result
        return dest;
    }

    int A_strcmp(const char* str1, const char* str2) {
        while (*str1 != '\0' && *str2 != '\0') {
            if (*str1 != *str2) {
                return (*str1 - *str2);
            }
            str1++;
            str2++;
        }

        return (*str1 - *str2);
    }

    int A_isspace(int c) {
        unsigned char ch = (unsigned char)c;

        return (ch == ' ' ||
            ch == '\t' ||
            ch == '\n' ||
            ch == '\r' ||
            ch == '\f' ||
            ch == '\v');
    }

    int A_tolower(int c) {
        unsigned char ch = (unsigned char)c;

        if (ch >= 'A' && ch <= 'Z')
            return ch + ('a' - 'A');

        return ch;
    }

    int A_stricmp(const char* str1, const char* str2) {
        while (*str1 && *str2) {
            if (A_tolower((unsigned char)*str1) != A_tolower((unsigned char)*str2)) {
                return A_tolower((unsigned char)*str1) - A_tolower((unsigned char)*str2);
            }
            str1++;
            str2++;
        }
        return A_tolower((unsigned char)*str1) - A_tolower((unsigned char)*str2);
    }

    int A_strncmp(const char *lhs, const char *rhs, size_t count) {
        size_t i;
        for (i = 0; i < count; i++) {
            if (lhs[i] != rhs[i]) {
                return (lhs[i] - rhs[i]);
            }
            if (lhs[i] == '\0') {
                return 0;
            }
        }
        return 0;
    }

    int A_strnicmp(const char *lhs, const char *rhs, size_t count) {
        size_t i;
        for (i = 0; i < count; i++) {
            if (A_tolower((unsigned char)lhs[i]) != A_tolower((unsigned char)rhs[i])) {
                return (lhs[i] - rhs[i]);
            }
            if (A_tolower((unsigned char)lhs[i]) == '\0') {
                return 0;
            }
        }
        return 0;
    }

    int A_strcoll(const char* lhs, const char* rhs) {
        return A_strcmp(lhs, rhs);
    }

    const char *A_strchr(const char *str, int ch) {
        while (*str != '\0') {
            if (*str == (char)ch) {
                return str;
            }
            str++;
        }
        return nullptr;
    }

    char *A_strchr(char *str, int ch) {
        while (*str != '\0') {
            if (*str == (char)ch) {
                return str;
            }
            str++;
        }
        return nullptr;
    }

    const char *A_strrchr(const char *str, int ch) {
        const char *last = nullptr;
        while (*str != '\0') {
            if (*str == (char)ch) {
                last = str;
            }
            str++;
        }
        return last;
    }

    char *A_strrchr(char *str, int ch) {
        char *last = nullptr;
        while (*str != '\0') {
            if (*str == (char)ch) {
                last = str;
            }
            str++;
        }
        return last;
    }

    const char *A_strpbrk(const char* str, const char* breakset) {
        while (*str != '\0') {
            const char* b = breakset;
            while (*b != '\0') {
                if (*str == *b) {
                    return str;
                }
                b++;
            }
            str++;
        }
        return nullptr;
    }

    char *A_strpbrk(char* str, const char* breakset) {
        while (*str != '\0') {
            const char* b = breakset;
            while (*b != '\0') {
                if (*str == *b) {
                    return str;
                }
                b++;
            }
            str++;
        }
        return nullptr;
    }

    const char *A_strstr(const char* haystack, const char* needle) {
        if (*needle == '\0') {
            return haystack;
        }

        while (*haystack != '\0') {
            const char* h = haystack;
            const char* n = needle;

            while (*h != '\0' && *n != '\0' && *h == *n) {
                h++;
                n++;
            }

            if (*n == '\0') {
                return haystack;
            }

            haystack++;
        }

        return nullptr;
    }

    char* A_strstr(char* haystack, const char* needle) {
        if (*needle == '\0') {
            return haystack;
        }

        while (*haystack != '\0') {
            char* h = haystack;
            const char* n = needle;

            while (*h != '\0' && *n != '\0' && *h == *n) {
                h++;
                n++;
            }

            if (*n == '\0') {
                return haystack;
            }

            haystack++;
        }

        return nullptr;
    }

    size_t A_strlen(const char* str) {
        size_t length = 0;
        while (*str != '\0') {
            length++;
            str++;
        }
        
        return length;
    }

    size_t A_strspn(const char* dest, const char* src) {
        size_t count = 0;
        while (*dest != '\0') {
            const char* s = src;
            bool found = false;
            while (*s != '\0') {
                if (*dest == *s) {
                    found = true;
                    break;
                }
                s++;
            }
            if (!found) {
                break;
            }
            count++;
            dest++;
        }
        return count;
    }

    size_t A_strcspn(const char *dest, const char *src) {
        size_t count = 0;
        while (*dest != '\0') {
            const char* s = src;
            bool found = false;
            while (*s != '\0') {
                if (*dest == *s) {
                    found = true;
                    break;
                }
                s++;
            }
            if (found) {
                break;
            }
            count++;
            dest++;
        }
        return count;
    }

    size_t A_strxfrm(char* dest, const char* src, size_t count) {
        size_t i;
        for (i = 0; i < count && *src != '\0'; i++) {
            *dest = *src;
            dest++;
            src++;
        }

        if (i < count) {
            *dest = '\0'; // null-terminate if there's space
        }

        return i;
    }

    char *A_strtok(char* str, const char* delim) {
        static char* last = nullptr;

        if (str != nullptr) {
            last = str;
        } else if (last == nullptr) {
            return nullptr;
        }

        while (*last != '\0' && A_strchr(delim, *last) != nullptr) {
            last++;
        }

        if (*last == '\0') {
            last = nullptr;
            return nullptr;
        }

        char* tokenStart = last;

        while (*last != '\0' && A_strchr(delim, *last) == nullptr) {
            last++;
        }

        if (*last != '\0') {
            *last = '\0';
            last++;
        } else {
            last = nullptr;
        }

        return tokenStart;
    }

    const void *A_memchr(const void* ptr, int value, size_t num) {
        const unsigned char* p = static_cast<const unsigned char*>(ptr);
        for (size_t i = 0; i < num; i++) {
            if (p[i] == static_cast<unsigned char>(value)) {
                return p + i;
            }
        }
        return nullptr;
    }

    void *A_memchr(void* ptr, int value, size_t num) {
        unsigned char* p = static_cast<unsigned char*>(ptr);
        for (size_t i = 0; i < num; i++) {
            if (p[i] == static_cast<unsigned char>(value)) {
                return p + i;
            }
        }
        return nullptr;
    }

    int A_memcmp(const void *lhs, const void *rhs, size_t count) {
        const unsigned char* l = static_cast<const unsigned char*>(lhs);
        const unsigned char* r = static_cast<const unsigned char*>(rhs);
        for (size_t i = 0; i < count; i++) {
            if (l[i] != r[i]) {
                return l[i] - r[i];
            }
        }
        return 0;
    }

    void* A_memset(void* ptr, int value, size_t count) {
        unsigned char* p = static_cast<unsigned char*>(ptr);
        for (size_t i = 0; i < count; i++) {
            p[i] = static_cast<unsigned char>(value);
        }
        return ptr;
    }

    void* A_memcpy(void* dest, const void* src, size_t count) {
        unsigned char* d = static_cast<unsigned char*>(dest);
        const unsigned char* s = static_cast<const unsigned char*>(src);
        for (size_t i = 0; i < count; i++) {
            d[i] = s[i];
        }
        return dest;
    }

    void *A_memmove(void* dest, const void* src, size_t count) {
        unsigned char* d = static_cast<unsigned char*>(dest);
        const unsigned char* s = static_cast<const unsigned char*>(src);

        if (d < s) {
            for (size_t i = 0; i < count; i++) {
                d[i] = s[i];
            }
        } else if (d > s) {
            for (size_t i = count; i > 0; i--) {
                d[i - 1] = s[i - 1];
            }
        }

        return dest;
    }

    char *A_strtrim(char *buf) {
        if (buf == nullptr) return nullptr;

        while (*buf != '\0' && (*buf == ' ' || *buf == '\t' || *buf == '\n' || *buf == '\r')) {
            buf++;
        }

        if (*buf == '\0') {
            return buf;
        }

        char* end = buf + A_strlen(buf) - 1;
        while (end > buf && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
            end--;
        }

        *(end + 1) = '\0';

        return buf;
    }

    char *A_strtrimws(char *buf) {
        if (buf == nullptr) return nullptr;

        while (*buf != '\0' && A_isspace(static_cast<unsigned char>(*buf))) {
            buf++;
        }

        if (*buf == '\0') {
            return buf;
        }

        char* end = buf + A_strlen(buf) - 1;
        while (end > buf && A_isspace(static_cast<unsigned char>(*end))) {
            end--;
        }

        *(end + 1) = '\0';

        return buf;
    }

} // namespace CTF

