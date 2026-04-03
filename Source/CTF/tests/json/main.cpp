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
#include <OStringStream.h>
#include <BasicString.h>

#include <Json/Json.h>
#include <Json/JsonParser.h>
#include <Json/JsonType.h>

#include <gtest/gtest.h>

using namespace CTF;

TEST(JsonTest, Parse) {
	OStringStream str;
	str << "{" << EndLine;
	str << "\t\"message\": \"Hello, world!\"," << EndLine;
	str << "\t\"status\": 200" << EndLine;
	str << "}";

	const String json = str.str();
	const char* szJson = json.c_str();
	if (!szJson || szJson[0] == '\0') {
		FAIL() << "Recevied an invalid JSON string from ostringstream!";
	}

	Json::JsonAllocator alloc(1024 * 1024);
	Json::JsonParser parser(&alloc);

	auto value = parser.Parse(szJson);
	if (value.type == Json::JSON_INVALID) {
		FAIL() << "Received an invalid value from JSON parser!";
	}
}

int main(int argc, char *argv[]) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}