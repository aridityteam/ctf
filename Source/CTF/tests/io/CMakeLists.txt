project(testIO)

if(MSVC)
    add_compile_options(/wd4172)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wno-return-local-addr)
endif()

set(CTF_INC_DIRS ${SRC_DIR}/CTF/ctf)

set(SOURCES
	"main.cpp"
	"ostringstreamtest.cpp"
	"pipestreamtest.cpp"
)

if(MSVC)
	foreach(FILE ${SOURCES}) 
		get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

		string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
		string(REPLACE "/" "\\" GROUP "${GROUP}")

		if ("${FILE}" MATCHES ".*\\.cpp" OR "${FILE}" MATCHES ".*\\.inl" OR "${FILE}" MATCHES ".*\\.ui" OR "${FILE}" MATCHES ".*\\.qml")
			set(GROUP "Source Files\\${GROUP}")
		elseif("${FILE}" MATCHES ".*\\.h")
			set(GROUP "Header Files\\${GROUP}")
		endif()

		source_group("${GROUP}" FILES "${FILE}")
	endforeach()
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CTF_INC_DIRS}
	${THIRDPARTY_DIR}/googletest/googletest/include
)

add_dependencies(${PROJECT_NAME}
	CTF
	gtest
	gtest_main
)
target_link_libraries(${PROJECT_NAME} PRIVATE CTF)
target_link_libraries(${PROJECT_NAME} PRIVATE
	gtest
	gtest_main
)

add_test(
	NAME CTF_IO_Test
	COMMAND testIO
)
