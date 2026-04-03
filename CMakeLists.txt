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
	add_compile_options(/W4 /clang:-Wno-character-conversion /clang:-Wno-error=character-conversion)
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
		add_compile_definitions(-Wno-narrowing)
	endif()
endif()

# global options start here

# end global options

if(WIN32)
    	message(STATUS "Building for Windows")
elseif(APPLE)
    	message(STATUS "Building for macOS")
elseif(UNIX)
    	message(STATUS "Building for Linux")
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
