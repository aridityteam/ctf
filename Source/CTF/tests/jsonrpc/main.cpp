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

#include <IOStream.h>
#include <Json/Json.h>
#include <Json/JsonRpc.h>

using namespace CTF;

namespace {
    bool AssertOrFail(bool condition, const char* message) {
        if (!condition) {
            CErr << message << EndLine;
            return false;
        }
        return true;
    }
}

int main() {
    {
        const char* requestJson =
            "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\","
            "\"params\":{\"client\":\"cereon\"}}";

        Json::JsonAllocator alloc(1024 * 1024);
        Json::JsonRpcMessage message;

        if (!AssertOrFail(Json::JsonRpc::Parse(requestJson, message, alloc), "Failed to parse request")) return -1;
        if (!AssertOrFail(message.type == Json::JsonRpcMessageType::Request, "Request type mismatch")) return -1;
        if (!AssertOrFail(message.hasId && !message.idIsString && message.id == 1, "Request id mismatch")) return -1;
        if (!AssertOrFail(message.method == "initialize", "Request method mismatch")) return -1;
        if (!AssertOrFail(message.hasParams, "Request params missing")) return -1;
    }

    {
        const char* notificationJson =
            "{\"jsonrpc\":\"2.0\",\"method\":\"$/progress\","
            "\"params\":{\"token\":\"abc\"}}";

        Json::JsonAllocator alloc(1024 * 1024);
        Json::JsonRpcMessage message;

        if (!AssertOrFail(Json::JsonRpc::Parse(notificationJson, message, alloc), "Failed to parse notification")) return -1;
        if (!AssertOrFail(message.type == Json::JsonRpcMessageType::Notification, "Notification type mismatch")) return -1;
        if (!AssertOrFail(!message.hasId, "Notification should not have id")) return -1;
    }

    {
        const char* successResponseJson =
            "{\"jsonrpc\":\"2.0\",\"id\":1,\"result\":{\"capabilities\":{}}}";

        Json::JsonAllocator alloc(1024 * 1024);
        Json::JsonRpcMessage message;

        if (!AssertOrFail(Json::JsonRpc::Parse(successResponseJson, message, alloc), "Failed to parse success response")) return -1;
        if (!AssertOrFail(message.type == Json::JsonRpcMessageType::SuccessResponse, "Success response type mismatch")) return -1;
        if (!AssertOrFail(message.hasResult, "Success response result missing")) return -1;
    }

    {
        const char* errorResponseJson =
            "{\"jsonrpc\":\"2.0\",\"id\":1,\"error\":{\"code\":-32601,\"message\":\"Method not found\"}}";

        Json::JsonAllocator alloc(1024 * 1024);
        Json::JsonRpcMessage message;

        if (!AssertOrFail(Json::JsonRpc::Parse(errorResponseJson, message, alloc), "Failed to parse error response")) return -1;
        if (!AssertOrFail(message.type == Json::JsonRpcMessageType::ErrorResponse, "Error response type mismatch")) return -1;
        if (!AssertOrFail(message.error.code == -32601, "Error code mismatch")) return -1;
        if (!AssertOrFail(message.error.message == "Method not found", "Error message mismatch")) return -1;
    }

    {
        Json::JsonRpcMessage outbound;
        outbound.type = Json::JsonRpcMessageType::Request;
        outbound.hasId = true;
        outbound.id = 7;
        outbound.idIsString = false;
        outbound.method = "shutdown";

        const String serialized = Json::JsonRpc::Serialize(outbound);
        Json::JsonAllocator alloc(1024 * 1024);
        Json::JsonRpcMessage reparsed;

        if (!AssertOrFail(Json::JsonRpc::Parse(serialized.c_str(), reparsed, alloc), "Failed to parse serialized request")) return -1;
        if (!AssertOrFail(reparsed.type == Json::JsonRpcMessageType::Request, "Reparsed request type mismatch")) return -1;
        if (!AssertOrFail(reparsed.hasId && reparsed.id == 7, "Reparsed request id mismatch")) return -1;
        if (!AssertOrFail(reparsed.method == "shutdown", "Reparsed request method mismatch")) return -1;
    }

    return 0;
}
