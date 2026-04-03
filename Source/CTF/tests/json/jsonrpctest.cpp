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

#include <gtest/gtest.h>

using namespace CTF;

TEST(JsonRpcTest, ParseRequestMsg) {
    const char* requestJson =
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\","
        "\"params\":{\"client\":\"cereon\"}}";

    Json::JsonAllocator alloc(1024 * 1024);
    Json::JsonRpcMessage message;

    ASSERT_TRUE(Json::JsonRpc::Parse(requestJson, message, alloc));
    ASSERT_EQ(message.type, Json::JsonRpcMessageType::Request);
    ASSERT_TRUE(message.hasId);
    ASSERT_FALSE(message.idIsString);
    ASSERT_EQ(message.id, 1);
    ASSERT_EQ(message.method, "initialize");
    ASSERT_TRUE(message.hasParams);
}

TEST(JsonRpcTest, ParseNotificationMsg) {
    const char* notificationJson =
        "{\"jsonrpc\":\"2.0\",\"method\":\"$/progress\","
        "\"params\":{\"token\":\"abc\"}}";

    Json::JsonAllocator alloc(1024 * 1024);
    Json::JsonRpcMessage message;

    ASSERT_TRUE(Json::JsonRpc::Parse(notificationJson, message, alloc));
    ASSERT_EQ(message.type, Json::JsonRpcMessageType::Notification);
    ASSERT_FALSE(message.hasId);
}

TEST(JsonRpcTest, ParseSuccessResponseMsg) {
    const char* successResponseJson =
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"result\":{\"capabilities\":{}}}";

    Json::JsonAllocator alloc(1024 * 1024);
    Json::JsonRpcMessage message;

    ASSERT_TRUE(Json::JsonRpc::Parse(successResponseJson, message, alloc));
    ASSERT_EQ(message.type, Json::JsonRpcMessageType::SuccessResponse);
    ASSERT_TRUE(message.hasResult);
}

TEST(JsonRpcTest, ParseErrResponseMsg) {
    const char* errorResponseJson =
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"error\":{\"code\":-32601,\"message\":\"Method not found\"}}";

    Json::JsonAllocator alloc(1024 * 1024);
    Json::JsonRpcMessage message;

    ASSERT_TRUE(Json::JsonRpc::Parse(errorResponseJson, message, alloc));
    ASSERT_EQ(message.type, Json::JsonRpcMessageType::ErrorResponse);
    ASSERT_EQ(message.error.code, -32601);
    ASSERT_EQ(message.error.message, "Method not found");
}

TEST(JsonRpcTest, ParseMessageToString) {
    Json::JsonRpcMessage outbound;
    outbound.type = Json::JsonRpcMessageType::Request;
    outbound.hasId = true;
    outbound.id = 7;
    outbound.idIsString = false;
    outbound.method = "shutdown";

    const String serialized = Json::JsonRpc::Serialize(outbound);
    Json::JsonAllocator alloc(1024 * 1024);
    Json::JsonRpcMessage reparsed;

    ASSERT_TRUE(Json::JsonRpc::Parse(serialized.c_str(), reparsed, alloc));
    ASSERT_EQ(reparsed.type, outbound.type);
    ASSERT_TRUE(reparsed.hasId);
    ASSERT_EQ(reparsed.id, outbound.id);
    ASSERT_EQ(reparsed.method, outbound.method);
}
