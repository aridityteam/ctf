##############################################################################
# CTFInstall.cmake
#
# Copyright (c) 2026 The Aridity Team, all rights reserved.
#
# Included from Source/CTF/CMakeLists.txt AFTER add_library(CTF ...).
##############################################################################

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# ---------------------------------------------------------------------------
# Installation destinations
# ---------------------------------------------------------------------------

set(CTF_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}/CTF")
set(CTF_INSTALL_LIBDIR     "${CMAKE_INSTALL_LIBDIR}")
set(CTF_INSTALL_BINDIR     "${CMAKE_INSTALL_BINDIR}")
set(CTF_INSTALL_CMAKEDIR   "${CMAKE_INSTALL_LIBDIR}/cmake/CTF")

# ---------------------------------------------------------------------------
# Include directories
# ---------------------------------------------------------------------------

target_include_directories(CTF
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CTF_INSTALL_INCLUDEDIR}>
)

# ---------------------------------------------------------------------------
# RPATH / install_name (shared libraries only)
# ---------------------------------------------------------------------------

if(CTF_BUILD_SHARED)
    if(APPLE)
        set_target_properties(CTF PROPERTIES
            INSTALL_NAME_DIR "@rpath"
            BUILD_WITH_INSTALL_RPATH FALSE
            BUILD_RPATH            "@loader_path"
            INSTALL_RPATH          "@loader_path/../${CTF_INSTALL_LIBDIR}"
        )
        # Allow consumers to find the dylib via @rpath if they link against it.
        target_link_options(CTF INTERFACE
            $<INSTALL_INTERFACE:-Wl,-rpath,@loader_path/../${CTF_INSTALL_LIBDIR}>
        )
    elseif(UNIX)
        set_target_properties(CTF PROPERTIES
            BUILD_RPATH   "$ORIGIN"
            INSTALL_RPATH "$ORIGIN/../${CTF_INSTALL_LIBDIR}"
        )
    endif()
endif()

# ---------------------------------------------------------------------------
# Install: library artifact
# ---------------------------------------------------------------------------

install(TARGETS CTF
    EXPORT      CTFTargets
    ARCHIVE     DESTINATION "${CTF_INSTALL_LIBDIR}"
    LIBRARY     DESTINATION "${CTF_INSTALL_LIBDIR}"
    RUNTIME     DESTINATION "${CTF_INSTALL_BINDIR}"
    INCLUDES    DESTINATION "${CTF_INSTALL_INCLUDEDIR}"
)

# ---------------------------------------------------------------------------
# Install: PDB files (MSVC only)
# ---------------------------------------------------------------------------

if(MSVC AND CTF_BUILD_SHARED)
    install(FILES
        $<TARGET_PDB_FILE:CTF>
        DESTINATION "${CTF_INSTALL_BINDIR}"
        OPTIONAL                    # not present in Release builds
    )
endif()

# ---------------------------------------------------------------------------
# Install: public headers
# ---------------------------------------------------------------------------

install(
    DIRECTORY   "${CMAKE_CURRENT_SOURCE_DIR}/"
    DESTINATION "${CTF_INSTALL_INCLUDEDIR}"
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.inl"
    PATTERN "CMakeLists.txt" EXCLUDE
)

# ---------------------------------------------------------------------------
# Export: installed targets file (CTFTargets.cmake)
# ---------------------------------------------------------------------------

install(EXPORT CTFTargets
    FILE        CTFTargets.cmake
    NAMESPACE   CTF::
    DESTINATION "${CTF_INSTALL_CMAKEDIR}"
)

export(EXPORT CTFTargets
    FILE      "${CMAKE_BINARY_DIR}/CTFTargets.cmake"
    NAMESPACE CTF::
)

# ---------------------------------------------------------------------------
# Package config + version files
# ---------------------------------------------------------------------------

configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/cmake/CTFConfig.cmake.in"
    "${CMAKE_BINARY_DIR}/CTFConfig.cmake"
    INSTALL_DESTINATION "${CTF_INSTALL_CMAKEDIR}"
)

write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/CTFConfigVersion.cmake"
    VERSION       "${PROJECT_VERSION}"
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_BINARY_DIR}/CTFConfig.cmake"
    "${CMAKE_BINARY_DIR}/CTFConfigVersion.cmake"
    DESTINATION "${CTF_INSTALL_CMAKEDIR}"
)

# ---------------------------------------------------------------------------
# Register this build tree in CMake's package registry
# ---------------------------------------------------------------------------

export(PACKAGE CTF)
