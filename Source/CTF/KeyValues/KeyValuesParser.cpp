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
#include "KeyValuesParser.h"

#include <cctype>
#include <cerrno>
#include <cstdlib>

namespace CTF::KV {
    char KeyValuesParser::Peek() const {
        if (IsEnd()) {
            return '\0';
        }

        return text.CStr()[offset];
    }

    char KeyValuesParser::Next() {
        if (IsEnd()) {
            return '\0';
        }

        return text.CStr()[offset++];
    }

    bool KeyValuesParser::IsEnd() const {
        return offset >= text.Length() || !text.CStr();
    }

    void KeyValuesParser::SkipWhitespace()
    {
        while (!IsEnd()) {
            if (A_isspace(Peek())) {
                ++offset;
                continue;
            }

            // Valve-style line comment.
            if (Peek() == '/' && offset + 1 < text.Length() && text.CStr()[offset + 1] == '/') {
                offset += 2;
                while (!IsEnd() && Peek() != '\n') {
                    ++offset;
                }
                continue;
            }

            // C-style block comment.
            if (Peek() == '/' && offset + 1 < text.Length() && text.CStr()[offset + 1] == '*') {
                offset += 2;
                while (!IsEnd()) {
                    if (Peek() == '*' && offset + 1 < text.Length() && text.CStr()[offset + 1] == '/') {
                        offset += 2;
                        break;
                    }
                    ++offset;
                }
                continue;
            }

            break;
        }
    }

    String KeyValuesParser::ParseToken()
    {
        SkipWhitespace();

        if (IsEnd())
            return "";

        String token;
        const char current = Peek();

        if (current == '{' || current == '}')
        {
            token += String(Next());
            return token;
        }

        if (current == '\"')
        {
            Next();
            while (!IsEnd() && Peek() != '\"')
            {
                if (Peek() == '\\')
                {
                    Next();
                    if (IsEnd()) {
                        break;
                    }

                    const char escaped = Next();
                    switch (escaped) {
                    case 'n':
                        token += String('\n');
                        break;
                    case 't':
                        token += String('\t');
                        break;
                    case 'r':
                        token += String('\r');
                        break;
                    case '\\':
                    case '\"':
                        token += String(escaped);
                        break;
                    default:
                        token += String(escaped);
                        break;
                    }
                }
                else
                {
                    token += String(Next());
                }
            }

            if (!IsEnd() && Peek() == '\"') {
                Next();
            }
        }
        else
        {
            while (!IsEnd())
            {
                const char ch = Peek();
                if (A_isspace(ch) || ch == '{' || ch == '}') {
                    break;
                }
                token += String(Next());
            }
        }

        return token;
    }

    KeyValuesValue KeyValuesParser::InferValue(const String& token)
    {
        KeyValuesValue v;

        if (token.Empty())
        {
            v.type = KeyValuesType::String;
            v.stringValue = "";
            return v;
        }

        if (A_stricmp(token.CStr(), "true") == 0)
        {
            v.type = KeyValuesType::Bool;
            v.boolValue = true;
            return v;
        }

        if (A_stricmp(token.CStr(), "false") == 0)
        {
            v.type = KeyValuesType::Bool;
            v.boolValue = false;
            return v;
        }

        errno = 0;
        char* intEnd = nullptr;
        const long intValue = std::strtol(token.CStr(), &intEnd, 10);
        if (errno == 0 && intEnd && *intEnd == '\0')
        {
            v.type = KeyValuesType::Int;
            v.intValue = static_cast<int>(intValue);
            return v;
        }

        errno = 0;
        char* floatEnd = nullptr;
        const float floatValue = std::strtof(token.CStr(), &floatEnd);
        if (errno == 0 && floatEnd && *floatEnd == '\0')
        {
            v.type = KeyValuesType::Float;
            v.floatValue = floatValue;
            return v;
        }

        v.type = KeyValuesType::String;
        v.stringValue = token;
        return v;
    }

    std::unique_ptr<KeyValues> KeyValuesParser::ParseKeyValuesRecursive(bool expectBlockEnd)
    {
        auto kv = std::make_unique<KeyValues>();

        while (true)
        {
            SkipWhitespace();

            if (IsEnd()) {
                break;
            }

            String key = ParseToken();

            if (key.Empty()) {
                break;
            }

            if (key == "}")
            {
                if (expectBlockEnd) {
                    break;
                }
                continue;
            }

            if (key == "{")
            {
                // A block without a key is malformed; skip this token.
                continue;
            }

            SkipWhitespace();

            String valueOrBlock = ParseToken();
            if (valueOrBlock.Empty()) {
                break;
            }

            if (valueOrBlock == "{")
            {
                auto child = ParseKeyValuesRecursive(true);
                child->m_name = key;
                kv->m_children[key] = std::move(child);
                continue;
            }

            if (valueOrBlock == "}")
            {
                // Missing value before closing block.
                if (expectBlockEnd) {
                    break;
                }
                continue;
            }

            KeyValuesValue val = InferValue(valueOrBlock);

            auto child = std::make_unique<KeyValues>();
            child->m_name = key;
            child->m_value = val;

            kv->m_children[key] = std::move(child);
        }

        return kv;
    }

    std::unique_ptr<KeyValues> KeyValuesParser::Parse(const String& data)
    {
        KeyValuesParser parser;
        parser.text = data;
        parser.offset = 0;

        return parser.ParseKeyValuesRecursive(false);
    }

}
