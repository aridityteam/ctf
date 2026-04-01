set_property(DIRECTORY . PROPERTY FOLDER "CTF")

option(CTF_BUILD_SHARED "Builds the Cereon Template Framework as a shared library" ON)

add_subdirectory(ctf)
add_subdirectory(tests)