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

#ifndef CTF_KVPARSER_H
#define CTF_KVPARSER_H
#pragma once

#include "KeyValues.h"
#include "KeyValuesType.h"
#include "KeyValuesValue.h"
#include "BasicString.h"
#include <memory>

namespace CTF::KV {
    class CTF_API KeyValuesParser
    {
    public:
        KeyValuesParser() = default;

        // Parse from string into a KeyValues hierarchy
        static std::unique_ptr<KeyValues> Parse(const String& data);

    private:
        size_t offset = 0;
        String text;

        void SkipWhitespace();
        char Peek() const;
        char Next();
        bool IsEnd() const;
        String ParseToken();
        KeyValuesValue InferValue(const String& token);

        std::unique_ptr<KeyValues> ParseKeyValuesRecursive(bool expectBlockEnd);
    };
}

#endif // !CTF_KVTYPES_H
