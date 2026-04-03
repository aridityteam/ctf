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

#ifndef CTF_H
#define CTF_H
#pragma once

#include <cstddef>
#include <cstring>
#include <cwchar>
#include <cstdlib>

#ifndef CTF_API
#    ifdef _WIN32
#        if defined(CTF_BUILD_SHARED) /* build dll */
#            define CTF_API __declspec(dllexport)
#        elif !defined(CTF_BUILD_STATIC) /* use dll */
#            define CTF_API __declspec(dllimport)
#        else /* static library */
#            define CTF_API
#        endif
#    else
#        if __GNUC__ >= 4
#            define CTF_API __attribute__((visibility("default")))
#        else
#            define CTF_API
#        endif
#    endif
#endif

#ifndef CTF_CONSTEXPR
#   define CTF_CONSTEXPR constexpr
#endif

#ifdef _WIN32
#define PATH_SEP '\\'
#else
#define PATH_SEP '/'
#endif

namespace CTF {
	using size_t = std::size_t;
	using wint_t = std::wint_t;

    //===============================================================================
    // String manipulation utils

    CTF_API char *A_strcpy(char* dest, const char* src);
    CTF_API char *A_strncpy(char* dest, const char* src, size_t count);

    CTF_API char *A_strcat(char* dest, const char* src);
    CTF_API char *A_strncat(char* dest, const char* src, size_t count);
    CTF_API size_t A_strlen(const char* str);
    CTF_API size_t A_strxfrm(char* dest, const char* src, size_t count);

    CTF_API int A_isspace(int c);
    CTF_API int A_tolower(int c);

    CTF_API int A_strcmp(const char* str1, const char* str2);
    CTF_API int A_stricmp(const char* str1, const char* str2);
    CTF_API int A_strncmp(const char* lhs, const char *rhs, size_t count);
    CTF_API int A_strnicmp(const char* lhs, const char *rhs, size_t count);
    CTF_API int A_strcoll( const char* lhs, const char* rhs );

    // astrchr
    CTF_API const char* A_strchr(const char* str, int ch);
    CTF_API char* A_strchr(char* str, int ch);

    // astrrchr
    CTF_API const char* A_strrchr(const char* str, int ch);
    CTF_API char* A_strrchr(char* str, int ch);

    CTF_API size_t A_strspn(const char* dest, const char* src);
    CTF_API size_t A_strcspn(const char *dest, const char *src);

    // astrpbrk
    CTF_API const char* A_strpbrk(const char* str, const char* breakset);
    CTF_API char* A_strpbrk(char* str, const char* breakset);

    // astrstr
    CTF_API const char* A_strstr(const char* haystack, const char* needle);
    CTF_API char* A_strstr(char* haystack, const char* needle);

    CTF_API char* A_strtok(char* str, const char* delim);

    // amemchr
    CTF_API const void *A_memchr(const void* ptr, int value, size_t num);
    CTF_API void *A_memchr(void* ptr, int value, size_t num);

    CTF_API int A_memcmp(const void *lhs, const void *rhs, size_t count);
    CTF_API void *A_memcpy(void* dest, const void* src, size_t count);
    CTF_API void *A_memmove(void* dest, const void* src, size_t count);
    CTF_API void *A_memset(void* ptr, int value, size_t count);

    CTF_API char *A_strtrim(char *buf);
    CTF_API char *A_strtrimws(char *buf);

    CTF_API int A_atoi(const char* str);
    CTF_API char* A_itoa(int value, char* buffer);

    //===============================================================================
    // WString manipulation utils

    CTF_API wchar_t* A_wcscpy(wchar_t* dest, const wchar_t* src);
    CTF_API wchar_t* A_wcsncpy(wchar_t* dest, const wchar_t* src, size_t n);
    CTF_API wchar_t* A_wcscat(wchar_t* dest, const wchar_t* src);
    CTF_API wchar_t* A_wcsncat(wchar_t* dest, const wchar_t* src, size_t n);

    CTF_API int A_iswspace(wint_t c);
    CTF_API wint_t A_towlower(wint_t c);

    CTF_API int A_wcscmp(const wchar_t* lhs, const wchar_t* rhs);
    CTF_API int A_wcsicmp(const wchar_t* lhs, const wchar_t* rhs);
    CTF_API int A_wcsncmp(const wchar_t* lhs, const wchar_t* rhs, size_t n);
    CTF_API int A_wcsnicmp(const wchar_t* lhs, const wchar_t* rhs, size_t n);
    CTF_API int A_wcscoll(const wchar_t* lhs, const wchar_t* rhs); // wrapper for wcsxfrm + wcscmp

    CTF_API const wchar_t* A_wcschr(const wchar_t* str, wchar_t ch);
    CTF_API wchar_t* A_wcschr(wchar_t* str, wchar_t ch);
    CTF_API const wchar_t* A_wcsrchr(const wchar_t* str, wchar_t ch);
    CTF_API wchar_t* A_wcsrchr(wchar_t* str, wchar_t ch);
    CTF_API const wchar_t* A_wcsstr(const wchar_t* haystack, const wchar_t* needle);
    CTF_API wchar_t* A_wcsstr(wchar_t* haystack, const wchar_t* needle);
    CTF_API const wchar_t* A_wcsnchr(const wchar_t* str, wchar_t ch, size_t count); // search first n chars
    CTF_API wchar_t* A_wcsnchr(wchar_t* str, wchar_t ch, size_t count);

    CTF_API size_t A_wcslen(const wchar_t* str);
    CTF_API size_t A_wcsspn(const wchar_t* dest, const wchar_t* src);
    CTF_API size_t A_wcscspn(const wchar_t* dest, const wchar_t* src);
    CTF_API size_t A_wcsxfrm(wchar_t* dest, const wchar_t* src, size_t count); // same as strxfrm

    CTF_API wchar_t* A_wcstok(wchar_t* str, const wchar_t* delim); // static internal context
    CTF_API wchar_t* A_wcstok_r(wchar_t* str, const wchar_t* delim, wchar_t** saveptr); // reentrant

    CTF_API void* A_wmemset(void* ptr, wchar_t value, size_t count);
    CTF_API int A_wmemcmp(const void* lhs, const void* rhs, size_t count);
    CTF_API void* A_wmemcpy(void* dest, const void* src, size_t count);
    CTF_API void* A_wmemmove(void* dest, const void* src, size_t count);
    CTF_API const void* A_wmemchr(const void* ptr, wchar_t value, size_t num);
    CTF_API void* A_wmemchr(void* ptr, wchar_t value, size_t num);

    CTF_API wchar_t* A_wcstrim(wchar_t* buf);              // trim spaces/tabs/newlines
    CTF_API wchar_t* A_wcstrimws(wchar_t* buf);  // trim any iswspace()

    CTF_API wchar_t* A_wcsdup(const wchar_t* src);
}

#endif // !CTF_H
