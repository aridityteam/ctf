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
#include "JsonParser.h"
#include <cstdlib>
#include <new>

namespace CTF::Json {
    JsonParser::JsonParser(JsonAllocator* alloc) {
        allocator = alloc;
        text = nullptr;
        pos = 0;
    }

    JsonValue JsonParser::Parse(const char* json) {
        if (!json || !allocator) {
            return MakeInvalid();
        }

        text = json;
        pos = 0;

        SkipWhitespace();

        JsonValue parsed = ParseValue();
        SkipWhitespace();

        if (parsed.type == JSON_INVALID || !End()) {
            return MakeInvalid();
        }

        return parsed;
    }

    JsonValue JsonParser::MakeInvalid() const {
        JsonValue invalid;
        invalid.type = JSON_INVALID;
        return invalid;
    }

    char JsonParser::Peek() {
        if (!text || End()) {
            return '\0';
        }

        return text[pos];
    }

    char JsonParser::Next() {
        if (!text || End()) {
            return '\0';
        }

        return text[pos++];
    }

    bool JsonParser::End() const {
        return !text || text[pos] == '\0';
    }

    void JsonParser::SkipWhitespace() {
        while (true) {
            char c = Peek();

            if (c == ' ' || c == '\n' || c == '\r' || c == '\t')
                pos++;
            else
                break;
        }
    }

    bool JsonParser::IsDigit(char c) const {
        return c >= '0' && c <= '9';
    }

    char JsonParser::ParseHexDigit(char c) const {
        if (c >= '0' && c <= '9') {
            return static_cast<char>(c - '0');
        }

        if (c >= 'a' && c <= 'f') {
            return static_cast<char>(10 + (c - 'a'));
        }

        if (c >= 'A' && c <= 'F') {
            return static_cast<char>(10 + (c - 'A'));
        }

        return -1;
    }

    bool JsonParser::ParseString(String& out) {
        out.clear();

        if (Next() != '"')
            return false;

        while (!End()) {
            const char c = Next();

            if (c == '"') {
                return true;
            }

            if (c == '\\') {
                if (End()) {
                    return false;
                }

                const char escaped = Next();
                switch (escaped) {
                case '"':
                case '\\':
                case '/':
                    out += String(escaped);
                    break;
                case 'b':
                    out += String('\b');
                    break;
                case 'f':
                    out += String('\f');
                    break;
                case 'n':
                    out += String('\n');
                    break;
                case 'r':
                    out += String('\r');
                    break;
                case 't':
                    out += String('\t');
                    break;
                case 'u': {
                    // Unicode escapes are collapsed to ASCII when possible.
                    int codePoint = 0;
                    for (int i = 0; i < 4; ++i) {
                        if (End()) {
                            return false;
                        }

                        const char hex = ParseHexDigit(Next());
                        if (hex < 0) {
                            return false;
                        }
                        codePoint = (codePoint << 4) | hex;
                    }

                    const char encoded = (codePoint >= 0 && codePoint <= 0x7F)
                        ? static_cast<char>(codePoint)
                        : '?';
                    out += String(encoded);
                    break;
                }
                default:
                    return false;
                }
            } else {
                // Control characters must be escaped in JSON strings.
                if (static_cast<unsigned char>(c) < 0x20) {
                    return false;
                }
                out += String(c);
            }
        }

        return false;
    }

    bool JsonParser::ParseNumber(double& out) {
        const int start = pos;

        if (Peek() == '-') {
            ++pos;
        }

        if (Peek() == '0') {
            ++pos;
        } else if (IsDigit(Peek())) {
            while (IsDigit(Peek())) {
                ++pos;
            }
        } else {
            pos = start;
            return false;
        }

        if (Peek() == '.') {
            ++pos;
            if (!IsDigit(Peek())) {
                pos = start;
                return false;
            }

            while (IsDigit(Peek())) {
                ++pos;
            }
        }

        if (Peek() == 'e' || Peek() == 'E') {
            ++pos;

            if (Peek() == '+' || Peek() == '-') {
                ++pos;
            }

            if (!IsDigit(Peek())) {
                pos = start;
                return false;
            }

            while (IsDigit(Peek())) {
                ++pos;
            }
        }

        char* end = nullptr;
        out = std::strtod(text + start, &end);

        if (!end || end != text + pos) {
            pos = start;
            return false;
        }

        return true;
    }

    bool JsonParser::MatchLiteral(const char* literal) {
        int tmpPos = pos;
        int i = 0;
        while (literal[i] != '\0') {
            const char c = text[tmpPos];
            if (c == '\0' || c != literal[i]) {
                return false;
            }
            ++tmpPos;
            ++i;
        }

        pos = tmpPos;
        return true;
    }

    JsonValue JsonParser::ParseArray() {
        if (Next() != '[') {
            return MakeInvalid();
        }

        JsonArray arr;
        arr.values = nullptr;
        arr.count = 0;

        JsonValue v;
        v.type = JSON_ARRAY;

        SkipWhitespace();
        if (Peek() == ']') {
            Next();
            v.array = arr;
            return v;
        }

        while (!End()) {
            JsonValue value = ParseValue();
            if (value.type == JSON_INVALID) {
                return MakeInvalid();
            }

            JsonValue* newValues = static_cast<JsonValue*>(
                allocator->Allocate(static_cast<int>(sizeof(JsonValue) * (arr.count + 1))));
            if (!newValues) {
                return MakeInvalid();
            }

            for (int i = 0; i < arr.count; i++) {
                new (&newValues[i]) JsonValue(arr.values[i]);
            }

            new (&newValues[arr.count]) JsonValue(value);
            ++arr.count;
            arr.values = newValues;

            SkipWhitespace();

            if (Peek() == ',') {
                Next();
                SkipWhitespace();
                if (Peek() == ']') {
                    return MakeInvalid();
                }
                continue;
            }

            if (Peek() == ']') {
                Next();
                v.array = arr;
                return v;
            }

            return MakeInvalid();
        }

        return MakeInvalid();
    }

    JsonValue JsonParser::ParseObject() {
        if (Next() != '{') {
            return MakeInvalid();
        }

        JsonObject obj;
        obj.entries = nullptr;
        obj.count = 0;

        JsonValue v;
        v.type = JSON_OBJECT;

        SkipWhitespace();
        if (Peek() == '}') {
            Next();
            v.object = obj;
            return v;
        }

        while (!End()) {
            String keyText;
            if (!ParseString(keyText)) {
                return MakeInvalid();
            }

            SkipWhitespace();
            if (Next() != ':') {
                return MakeInvalid();
            }

            SkipWhitespace();

            JsonValue value = ParseValue();
            if (value.type == JSON_INVALID) {
                return MakeInvalid();
            }

            JsonObjectEntry* newEntries = static_cast<JsonObjectEntry*>(
                allocator->Allocate(static_cast<int>(sizeof(JsonObjectEntry) * (obj.count + 1))));
            if (!newEntries) {
                return MakeInvalid();
            }

            for (int i = 0; i < obj.count; i++)
                newEntries[i] = obj.entries[i];

            char* key = static_cast<char*>(allocator->Allocate(static_cast<int>(keyText.length() + 1)));
            if (!key) {
                return MakeInvalid();
            }

            for (size_t i = 0; i < keyText.length(); ++i) {
                key[i] = keyText.c_str()[i];
            }
            key[keyText.length()] = '\0';

            newEntries[obj.count].key = key;

            JsonValue* stored = static_cast<JsonValue*>(allocator->Allocate(static_cast<int>(sizeof(JsonValue))));
            if (!stored) {
                return MakeInvalid();
            }

            new (stored) JsonValue(value);
            newEntries[obj.count].value = stored;

            obj.entries = newEntries;
            obj.count++;

            SkipWhitespace();

            if (Peek() == ',') {
                Next();
                SkipWhitespace();
                if (Peek() == '}') {
                    return MakeInvalid();
                }
                continue;
            }

            if (Peek() == '}') {
                Next();
                v.object = obj;
                return v;
            }

            return MakeInvalid();
        }

        return MakeInvalid();
    }

    JsonValue JsonParser::ParseValue() {
        SkipWhitespace();

        JsonValue v;
        char c = Peek();

        if (c == '"')
        {
            String parsed;
            if (!ParseString(parsed)) {
                return MakeInvalid();
            }
            v.type = JSON_STRING;
            v.string = parsed;
        }
        else if (c == '{')
        {
            v = ParseObject();
        }
        else if (c == '[')
        {
            v = ParseArray();
        }
        else if (c == 't')
        {
            if (MatchLiteral("true"))
            {
                v.type = JSON_BOOL;
                v.boolean = true;
            }
            else
            {
                v.type = JSON_INVALID;
            }
        }
        else if (c == 'f')
        {
            if (MatchLiteral("false"))
            {
                v.type = JSON_BOOL;
                v.boolean = false;
            }
            else
            {
                v.type = JSON_INVALID;
            }
        }
        else if (c == 'n')
        {
            if (MatchLiteral("null"))
            {
                v.type = JSON_NULL;
            }
            else
            {
                v.type = JSON_INVALID;
            }
        }
        else if (c == '-' || (c >= '0' && c <= '9'))
        {
            double number = 0.0;
            if (!ParseNumber(number)) {
                return MakeInvalid();
            }
            v.type = JSON_NUMBER;
            v.number = number;
        }
        else
        {
            v.type = JSON_INVALID;
        }

        return v;
    }
}
