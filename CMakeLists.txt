cmake_minimum_required(VERSION 3.16)
project(Cereon
	LANGUAGES CXX
)

# begin global configuration here

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SRC_DIR ${ROOT_DIR}/Source)

list(APPEND CMAKE_MODULE_PATH "${ROOT_DIR}/cmake")

set(GLOBAL_INC_DIR ${SRC_DIR}/Public)
set(GLOBAL_LIB_DIR ${SRC_DIR}/Lib)
set(THIRDPARTY_DIR ${SRC_DIR}/ThirdParty)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)

	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
		Debug
		Release
		RelWithDebInfo
		MinSizeRel
	)
endif()

if(MSVC)
	add_compile_options(
		$<$<COMPILE_LANGUAGE:C,CXX>:/W4>
	)
	
	add_compile_definitions(
		$<$<COMPILE_LANGUAGE:C,CXX>:COMPILER_MSVC=1>
		$<$<COMPILE_LANGUAGE:C,CXX>:MSVC=1>
	)

	if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		add_compile_options(/clang:-Wno-character-conversion /clang:-Wno-error=character-conversion)
	else()
	endif()
else()
	set(CMAKE_THREAD_LIBS_INIT "-lpthread")
	set(CMAKE_HAVE_THREADS_LIBRARY 1)
	set(CMAKE_USE_WIN32_THREADS_INIT 0)
	set(CMAKE_USE_PTHREADS_INIT 1)
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	add_compile_options(-Wall -Wextra -Wpedantic)
	if (WIN32)
		add_compile_options(-Wno-character-conversion -Wno-error=character-conversion)
	else()
		add_compile_options(-Wno-narrowing)
	endif()
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_compile_definitions(COMPILER_GCC=1)
	add_compile_definitions(GNUC=1)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	add_compile_definitions(COMPILER_CLANG=1)
	add_compile_definitions(CLANG=1)
endif()

set(PLATFORM_64BITS 0)
set(PLATFORM_32BITS 0)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	add_compile_definitions(__x86_64__=1)
	add_compile_definitions(PLATFORM_64BITS=1)
	set(PLATFORM_64BITS 1)
	set(PLATFORM_32BITS 0)

	if (WIN32)
		add_compile_definitions(WIN64=1)
		add_compile_definitions(_WIN64=1)

		if (MSVC)
			add_compile_definitions($<$<COMPILE_LANGUAGE:C,CXX>:COMPILER_MSVC64=1>)

			# C4267: 'var' : conversion from 'size_t' to 'type', possible loss of data
			#add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:/wd4267>)
		endif()
	endif()
else()
	set(PLATFORM_64BITS 0)
	set(PLATFORM_32BITS 1)

	add_compile_definitions(PLATFORM_32BITS=1)

	if (WIN32 and MSVC)
		add_compile_definitions(COMPILER_MSVC32=1)
	endif()
endif()

# global options start here

option(CTF_BUILD_SHARED "Builds the Cereon Template Framework as a shared library" ON)

# end global options

set(IS_WINDOWS 0)
set(IS_UNIX 0)
set(IS_POSIX 0)
set(IS_MACOS 0)
set(IS_LINUX 0)

if(WIN32)
	set(IS_WINDOWS 1)
	message(STATUS "Building for Windows")
elseif(UNIX)
	set(IS_UNIX 1)
	set(IS_POSIX 1)

	if (APPLE)
		set(IS_MACOS 1)
		message(STATUS "Building for macOS")
	elseif(LINUX)
		set(IS_LINUX 1)

		message(STATUS "Building for Linux")
	endif()
endif()

include_directories(
    ${GLOBAL_INC_DIR}
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(Doxygen OPTIONAL_COMPONENTS dot)

if(DOXYGEN_FOUND)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${ROOT_DIR}/README.md")

    doxygen_add_docs(doc ${SRC_DIR})
endif()   

enable_testing()

# end global configuration here

add_subdirectory(${SRC_DIR})
