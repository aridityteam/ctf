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

#ifndef CTF_JSONPARSER_H
#define CTF_JSONPARSER_H
#pragma once

#include "JsonValue.h"
#include "JsonAllocator.h"

namespace CTF::Json {
	class CTF_API JsonParser {
	public:
        JsonParser(JsonAllocator* alloc);

        JsonValue Parse(const char* json);

    private:
        char Peek();
        char Next();
        bool End() const;
        void SkipWhitespace();

        JsonValue ParseValue();
        JsonValue ParseObject();
        JsonValue ParseArray();
        bool ParseString(String& out);
        bool ParseNumber(double& out);
        bool MatchLiteral(const char* literal);
        bool IsDigit(char c) const;
        char ParseHexDigit(char c) const;
        JsonValue MakeInvalid() const;

    private:
        const char* text;
        int pos;
        JsonAllocator* allocator;
	};
}

#endif // !CTF_JSONVALUE_H
