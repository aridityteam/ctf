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

    //===============================================================================
    // String manipulation utils

    typedef unsigned long size_t;

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

    //===============================================================================
    // WString manipulation utils
	typedef unsigned short wint_t;

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

#define USE_CTF_STRING_UTILS 1

#if USE_CTF_STRING_UTILS
#define Q_strcpy CTF::A_strcpy
#define Q_strncpy CTF::A_strncpy
#define Q_strcat CTF::A_strcat
#define Q_strncat CTF::A_strncat
#define Q_strlen CTF::A_strlen
#define Q_strxfrm CTF::A_strxfrm
#define Q_strcmp CTF::A_strcmp
#define Q_stricmp CTF::A_stricmp
#define Q_strncmp CTF::A_strncmp
#define Q_strnicmp CTF::A_strnicmp
#define Q_strcoll CTF::A_strcoll
#define Q_strchr CTF::A_strchr
#define Q_strrchr CTF::A_strrchr
#define Q_strspn CTF::A_strspn
#define Q_strcspn CTF::A_strcspn
#define Q_strpbrk CTF::A_strpbrk
#define Q_strstr CTF::A_strstr
#define Q_strtok CTF::A_strtok
#define Q_strtrim CTF::A_strtrim
#define Q_strtrimws CTF::A_strtrimws

#define Q_memcmp CTF::A_memcmp
#define Q_memchr CTF::A_memchr
#define Q_memcpy CTF::A_memcpy
#define Q_memmove CTF::A_memmove
#define Q_memset CTF::A_memset

#define Q_isspace CTF::A_isspace
#define Q_tolower CTF::A_tolower

#define Q_wcscpy      	CTF::A_wcscpy
#define Q_wcsncpy     	CTF::A_wcsncpy
#define Q_wcscat      	CTF::A_wcscat
#define Q_wcsncat     	CTF::A_wcsncat
#define Q_wcslen      	CTF::A_wcslen
#define Q_wcsxfrm     	CTF::A_wcsxfrm
#define Q_wcscmp      	CTF::A_wcscmp
#define Q_wcsncmp     	CTF::A_wcsncmp
#define Q_wcscoll     	CTF::A_wcscoll
#define Q_wcsicmp     	CTF::A_wcsicmp
#define Q_wcsnicmp    	CTF::A_wcsnicmp
#define Q_wcschr      	CTF::A_wcschr
#define Q_wcsrchr     	CTF::A_wcsrchr
#define Q_wcsspn      	CTF::A_wcsspn
#define Q_wcscspn     	CTF::A_wcscspn
#define Q_wcsstr      	CTF::A_wcsstr
#define Q_wcstok      	CTF::A_wcstok
#define Q_wcstok_r    	CTF::A_wcstok_r
#define Q_wcstrim		CTF::A_wcstrim
#define Q_wcstrimws 	CTF::A_wcstrimws
#define Q_wcsdup      	CTF::A_wcsdup

#define Q_wmemchr     CTF::A_wmemchr
#define Q_wmemcpy     CTF::A_wmemcpy
#define Q_wmemmove    CTF::A_wmemmove
#define Q_wmemset     CTF::A_wmemset
#define Q_wmemcmp     CTF::A_wmemcmp
#else
#define Q_strcpy ::strcpy
#define Q_strncpy ::strncpy
#define Q_strcat ::strcat
#define Q_strncat ::strncat
#define Q_strlen ::strlen
#define Q_strxfrm ::strxfrm
#define Q_strcmp ::strcmp
#define Q_strncmp ::strncmp
#define Q_strcoll ::strcoll
#define Q_strchr ::strchr
#define Q_strrchr ::strrchr
#define Q_strspn ::strspn
#define Q_strcspn ::strcspn
#define Q_strpbrk ::strpbrk
#define Q_strstr ::strstr
#define Q_strtok ::strtok

#define Q_memchr ::memchr
#define Q_memcpy ::memcpy
#define Q_memmove ::memmove
#define Q_memset ::memset

#define Q_wcscpy      ::wcscpy
#define Q_wcsncpy     ::wcsncpy
#define Q_wcscat      ::wcscat
#define Q_wcsncat     ::wcsncat
#define Q_wcslen      ::wcslen
#define Q_wcsxfrm     ::wcsxfrm
#define Q_wcscmp      ::wcscmp
#define Q_wcsncmp     ::wcsncmp
#define Q_wcscoll     ::wcscoll
#define Q_wcschr      ::wcschr
#define Q_wcsrchr     ::wcsrchr
#define Q_wcsspn      ::wcsspn
#define Q_wcscspn     ::wcscspn
#define Q_wcsstr      ::wcsstr
#define Q_wcstok      ::wcstok
#define Q_wmemchr     ::wmemchr
#define Q_wmemcpy     ::wmemcpy
#define Q_wmemmove    ::wmemmove
#define Q_wmemset     ::wmemset
#define Q_wmemcmp     ::wmemcmp
#endif // USE_CTF_STRING_UTILS

namespace CTF {
	typedef unsigned long size_t;
	typedef unsigned short wint_t;
}

#endif // !CTF_H