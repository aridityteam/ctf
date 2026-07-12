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

#ifndef CTF_JSONRPC_H
#define CTF_JSONRPC_H
#pragma once

#include "JsonAllocator.h"
#include "JsonValue.h"

namespace CTF::Json {
    enum class JsonRpcMessageType {
        Invalid = 0,
        Request,
        Notification,
        SuccessResponse,
        ErrorResponse
    };

    enum class JsonRpcErrorCode {
        ParseError = -32700,
        InvalidRequest = -32600,
        MethodNotFound = -32601,
        InvalidParams = -32602,
        InternalError = -32603,

        ReservedErrorStart = -32099,
        ServerErrorStart = ReservedErrorStart,

        ServerNotInitialized = -32002,
        UnknownErrorCode = -32001,

        ReservedErrorEnd = -32000,
        ServerErrorEnd = ReservedErrorEnd,

        LSPReservedErrorStart = -32899,
        RequestFailed = -32803,
        ServerCancelled = -32802,
        ContentModified = -32801,
        RequestCancelled = -32800,
        LSPReservedErrorEnd = -32800,
    };

    struct JsonRpcError {
        int code{0};
        String message{};
        JsonValue data{};
        bool hasData{false};
    };

    struct JsonRpcMessage {
        JsonRpcMessageType type{JsonRpcMessageType::Invalid};

        bool hasId{false};
        bool idIsString{false};
        int id{0};
        String idString{};
        String method{};
        JsonValue params{};
        bool hasParams{false};
        JsonValue result{};
        bool hasResult{false};
        JsonRpcError error{};
    };

    class CTF_API JsonRpc {
    public:
        static bool Parse(const char* json, JsonRpcMessage& out, JsonAllocator& allocator);
        static String Serialize(const JsonRpcMessage& message);
    };
}

#endif // !CTF_JSONRPC_H
