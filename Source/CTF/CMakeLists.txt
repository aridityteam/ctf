project(CTF
	VERSION 0.1.0
    DESCRIPTION "Aridity Cereon Template Framework"
    LANGUAGES CXX
)

if(MSVC)
    add_compile_options(/wd4172)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wno-return-local-addr)
endif()

set(SOURCES
	# basic stuff
	"Algorithm.cpp"
	"Algorithm.h"
	"Assert.h"
	"Color.cpp"
	"Color.h"
	"ColorUtil.cpp"
	"ColorUtil.h"

	# Collections
	"Collections/Hash.cpp"
	"Collections/Hash.h"
	"Collections/List.inl"
	"Collections/List.h"
	"Collections/Map.inl"
	"Collections/Map.h"
	"Collections/Pair.cpp"
	"Collections/Pair.h"
	"Collections/Vector.inl"
	"Collections/Vector.h"

	# filesystem
	"FileSystem/File.cpp"
	"FileSystem/File.h"
	"FileSystem/Path.cpp"
	"FileSystem/Path.h"

	# logging
	"Logging/Logger.cpp"
	"Logging/Logger.h"
	"Logging/LoggerSystem.cpp"
	"Logging/LoggerSystem.h"
	"Logging/LogListener.cpp"
	"Logging/LogListener.h"

	# mathematics
	"Math/Logarithms.h"
	"Math/Power.h"
	"Math/Rounding.h"
	"Math/Trigonometry.h"

	# string
	#"String.inl"
	"BasicString.h"

	# threading
	"Threading/CancellationToken.cpp"
	"Threading/CancellationToken.h"
	#"Threading/Thread.cpp"
	"Threading/Thread.h"
	#"Threading/Mutex.cpp"
	"Threading/Mutex.h"
	#"Threading/Task.cpp"
	"Threading/Task.h"

	# utilities
	"Util/CLIParser.cpp"
	"Util/CLIParser.h"

	## parsers
	### JSON
	"Json/JsonAllocator.cpp"
	"Json/JsonAllocator.h"
	"Json/JsonArray.h"
	"Json/Json.cpp"
	"Json/Json.h"
	"Json/JsonObject.h"
	"Json/JsonParser.cpp"
	"Json/JsonParser.h"
	"Json/JsonRpc.cpp"
	"Json/JsonRpc.h"
	"Json/JsonType.h"
	"Json/JsonValue.cpp"
	"Json/JsonValue.h"

	### Valve KeyValues
	"KeyValues/KeyValues.cpp"
	"KeyValues/KeyValues.h"
	"KeyValues/KeyValuesParser.cpp"
	"KeyValues/KeyValuesParser.h"
	"KeyValues/KeyValuesType.h"
	"KeyValues/KeyValuesValue.h"

	# I/O streams
	"AnonymousPipeStream.cpp"
	"AnonymousPipeStream.h"
	"Stream.cpp"
	"Stream.h"
	"FileStream.cpp"
	"FileStream.h"
	"NamedPipeStream.cpp"
	"NamedPipeStream.h"
	"IOStream.cpp"
	"IOStream.h"
	"IStream.cpp"
	"IStream.h"
	"OStream.cpp"
	"OStream.h"
	"OStringStream.cpp"
	"OStringStream.h"
	"PipeStream.cpp"
	"PipeStream.h"

	# memory management
	"LinearArena.cpp"
	"LinearArena.h"
	"Memory.cpp"
	"Memory.h"
	"RefCounted.cpp"
	"RefCounted.h"
	"RefCounter.cpp"
	"RefCounter.h"
	"RefPtr.cpp"
	"RefPtr.h"

	# global overrides
	"MemOverride.cpp"

	# others
	"CTF.cpp"
	"CTF.h"
	"Runtime.cpp"
	"Runtime.h"
	"Utility.cpp"
	"Utility.h"
	"Validation.h"
)

if(MSVC)
	foreach(FILE ${SOURCES}) 
		get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

		string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

		string(REPLACE "/" "\\" GROUP "${GROUP}")

		if ("${FILE}" MATCHES ".*\\.cpp" OR "${FILE}" MATCHES ".*\\.inl")
		set(GROUP "Source Files\\${GROUP}")
		elseif("${FILE}" MATCHES ".*\\.h")
		set(GROUP "Header Files\\${GROUP}")
		endif()

		source_group("${GROUP}" FILES "${FILE}")
	endforeach()
endif()

if (CTF_BUILD_SHARED)
	add_compile_definitions(CTF_BUILD_SHARED)
	add_library(${PROJECT_NAME}
		SHARED
		${SOURCES}
	)
else()
	add_compile_definitions(CTF_BUILD_STATIC)
	add_library(${PROJECT_NAME}
		STATIC
		${SOURCES}
	)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION   "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_VISIBILITY_PRESET   hidden
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)

target_precompile_headers(${PROJECT_NAME} PRIVATE CTF.h)

include("${CMAKE_SOURCE_DIR}/cmake/CTFInstall.cmake")

