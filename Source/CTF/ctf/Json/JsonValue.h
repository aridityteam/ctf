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

#ifndef CTF_JSONVALUE_H
#define CTF_JSONVALUE_H
#pragma once

#include "String.h"
#include "JsonType.h"
#include "JsonArray.h"
#include "JsonObject.h"

namespace CTF::Json {
    class CTF_API JsonValue {
    public:
        JsonType type{JSON_NULL};
        bool boolean{false};
        double number{0.0};
        String string{};
        JsonArray array{nullptr, 0};
        JsonObject object{nullptr, 0};

        JsonValue();
        JsonValue(const JsonValue& other);
        ~JsonValue();

        JsonValue& operator=(const JsonValue& other);
    };
}

#endif // !CTF_JSONVALUE_H
