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
#include "JsonRpc.h"
#include "JsonParser.h"

#include <cstdio>

namespace CTF::Json {
    namespace {
        bool IsIntegral(const double value) {
            const int asInt = static_cast<int>(value);
            return value == static_cast<double>(asInt);
        }

        const JsonValue* GetObjectField(const JsonObject& object, const char* key) {
            if (!key) {
                return nullptr;
            }

            for (int i = 0; i < object.count; ++i) {
                const JsonObjectEntry& entry = object.entries[i];
                if (!entry.key || !entry.value) {
                    continue;
                }

                if (A_strcmp(entry.key, key) == 0) {
                    return entry.value;
                }
            }

            return nullptr;
        }

        void AppendCString(String& out, const char* text) {
            if (text && text[0] != '\0') {
                out += String(text);
            }
        }

        void AppendEscapedJsonString(String& out, const char* text) {
            out += String('"');

            if (text) {
                for (const char* p = text; *p != '\0'; ++p) {
                    switch (const char ch = *p) {
                    case '\"': out += String("\\\""); break;
                    case '\\': out += String("\\\\"); break;
                    case '\b': out += String("\\b"); break;
                    case '\f': out += String("\\f"); break;
                    case '\n': out += String("\\n"); break;
                    case '\r': out += String("\\r"); break;
                    case '\t': out += String("\\t"); break;
                    default:
                        if (static_cast<unsigned char>(ch) < 0x20) {
                            char escaped[7];
                            snprintf(escaped, sizeof(escaped), "\\u%04x", static_cast<unsigned char>(ch));
                            out += String(escaped);
                        } else {
                            out += String(ch);
                        }
                        break;
                    }
                }
            }

            out += String('"');
        }

        void AppendJsonValue(String& out, const JsonValue& value);

        void AppendJsonArray(String& out, const JsonArray& array) {
            out += String('[');
            for (int i = 0; i < array.count; ++i) {
                if (i > 0) {
                    out += String(',');
                }

                if (!array.values) {
                    out += String("null");
                    continue;
                }

                AppendJsonValue(out, array.values[i]);
            }
            out += String(']');
        }

        void AppendJsonObject(String& out, const JsonObject& object) {
            out += String('{');
            for (int i = 0; i < object.count; ++i) {
                if (i > 0) {
                    out += String(',');
                }

                const auto&[key, value] = object.entries[i];
                AppendEscapedJsonString(out, key ? key : "");
                out += String(':');

                if (value) {
                    AppendJsonValue(out, *value);
                } else {
                    out += String("null");
                }
            }
            out += String('}');
        }

        void AppendJsonValue(String& out, const JsonValue& value) {
            switch (value.type) {
            case JSON_NULL:
                out += String("null");
                break;
            case JSON_BOOL:
                out += String(value.boolean ? "true" : "false");
                break;
            case JSON_NUMBER: {
                char buffer[64];
                snprintf(buffer, sizeof(buffer), "%.17g", value.number);
                out += String(buffer);
                break;
            }
            case JSON_STRING:
                AppendEscapedJsonString(out, value.string.CStr());
                break;
            case JSON_ARRAY:
                AppendJsonArray(out, value.array);
                break;
            case JSON_OBJECT:
                AppendJsonObject(out, value.object);
                break;
            case JSON_INVALID:
            default:
                out += String("null");
                break;
            }
        }

        bool ParseId(const JsonValue* idField, JsonRpcMessage& out) {
            if (!idField) {
                out.hasId = false;
                return true;
            }

            if (idField->type == JSON_NULL) {
                out.hasId = false;
                return true;
            }

            if (idField->type == JSON_STRING) {
                out.hasId = true;
                out.idIsString = true;
                out.idString = idField->string;
                return true;
            }

            if (idField->type == JSON_NUMBER && IsIntegral(idField->number)) {
                out.hasId = true;
                out.idIsString = false;
                out.id = static_cast<int>(idField->number);
                return true;
            }

            return false;
        }
    }

    bool JsonRpc::Parse(const char* json, JsonRpcMessage& out, JsonAllocator& allocator) {
        out = JsonRpcMessage{};

        JsonParser parser(&allocator);
        JsonValue root = parser.Parse(json);

        if (root.type != JSON_OBJECT) {
            return false;
        }

        const JsonValue* jsonrpcField = GetObjectField(root.object, "jsonrpc");
        if (!jsonrpcField || jsonrpcField->type != JSON_STRING || A_strcmp(jsonrpcField->string.CStr(), "2.0") != 0) {
            return false;
        }

        const JsonValue* methodField = GetObjectField(root.object, "method");
        const JsonValue* idField = GetObjectField(root.object, "id");
        const JsonValue* paramsField = GetObjectField(root.object, "params");
        const JsonValue* resultField = GetObjectField(root.object, "result");
        const JsonValue* errorField = GetObjectField(root.object, "error");

        if (methodField) {
            if (methodField->type != JSON_STRING) {
                return false;
            }

            out.method = methodField->string;
            out.hasParams = (paramsField != nullptr);
            if (paramsField) {
                out.params = *paramsField;
            }

            if (!ParseId(idField, out)) {
                return false;
            }

            out.type = out.hasId ? JsonRpcMessageType::Request : JsonRpcMessageType::Notification;
            return true;
        }

        if (!ParseId(idField, out) || !out.hasId) {
            return false;
        }

        const bool hasResult = (resultField != nullptr);
        if (const bool hasError = (errorField != nullptr); hasResult == hasError) {
            return false;
        }

        if (hasResult) {
            out.type = JsonRpcMessageType::SuccessResponse;
            out.hasResult = true;
            out.result = *resultField;
            return true;
        }

        if (errorField->type != JSON_OBJECT) {
            return false;
        }

        const JsonValue* codeField = GetObjectField(errorField->object, "code");
        const JsonValue* messageField = GetObjectField(errorField->object, "message");
        const JsonValue* dataField = GetObjectField(errorField->object, "data");

        if (!codeField || codeField->type != JSON_NUMBER || !IsIntegral(codeField->number)) {
            return false;
        }

        if (!messageField || messageField->type != JSON_STRING) {
            return false;
        }

        out.type = JsonRpcMessageType::ErrorResponse;
        out.error.code = static_cast<int>(codeField->number);
        out.error.message = messageField->string;
        out.error.hasData = (dataField != nullptr);
        if (dataField) {
            out.error.data = *dataField;
        }

        return true;
    }

    String JsonRpc::Serialize(const JsonRpcMessage& message) {
        String out;
        out += String("{\"jsonrpc\":\"2.0\"");

        switch (message.type) {
        case JsonRpcMessageType::Request:
        case JsonRpcMessageType::Notification: {
            out += String(",\"method\":");
            AppendEscapedJsonString(out, message.method.CStr());

            if (message.hasParams) {
                out += String(",\"params\":");
                AppendJsonValue(out, message.params);
            }

            if (message.type == JsonRpcMessageType::Request && message.hasId) {
                out += String(",\"id\":");
                if (message.idIsString) {
                    AppendEscapedJsonString(out, message.idString.CStr());
                } else {
                    char idBuffer[32];
                    snprintf(idBuffer, sizeof(idBuffer), "%d", message.id);
                    AppendCString(out, idBuffer);
                }
            }
            break;
        }

        case JsonRpcMessageType::SuccessResponse: {
            if (message.hasId) {
                out += String(",\"id\":");
                if (message.idIsString) {
                    AppendEscapedJsonString(out, message.idString.CStr());
                } else {
                    char idBuffer[32];
                    snprintf(idBuffer, sizeof(idBuffer), "%d", message.id);
                    AppendCString(out, idBuffer);
                }
            }

            out += String(",\"result\":");
            AppendJsonValue(out, message.result);
            break;
        }

        case JsonRpcMessageType::ErrorResponse: {
            if (message.hasId) {
                out += String(",\"id\":");
                if (message.idIsString) {
                    AppendEscapedJsonString(out, message.idString.CStr());
                } else {
                    char idBuffer[32];
                    snprintf(idBuffer, sizeof(idBuffer), "%d", message.id);
                    AppendCString(out, idBuffer);
                }
            }

            out += String(",\"error\":{\"code\":");
            {
                char codeBuffer[32];
                snprintf(codeBuffer, sizeof(codeBuffer), "%d", message.error.code);
                AppendCString(out, codeBuffer);
            }
            out += String(",\"message\":");
            AppendEscapedJsonString(out, message.error.message.CStr());

            if (message.error.hasData) {
                out += String(",\"data\":");
                AppendJsonValue(out, message.error.data);
            }

            out += String("}");
            break;
        }

        case JsonRpcMessageType::Invalid:
        default:
            out += String(",\"error\":{\"code\":-32600,\"message\":\"Invalid Request\"}");
            break;
        }

        out += String("}");
        return out;
    }
}
