set(COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Common)
set(COMMON_SOURCES
	${COMMON_DIR}/TestMain.cpp
	${SRC_DIR}/CTF/MemOverride.cpp
)

add_subdirectory(io)
add_subdirectory(threading)
add_subdirectory(json)
add_subdirectory(math)
#add_subdirectory(keyvalues)
