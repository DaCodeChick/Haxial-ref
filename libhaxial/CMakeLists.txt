cmake_minimum_required(VERSION 3.15)
project(libhaxial VERSION 0.1.0 LANGUAGES C)

# C standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Build options
option(BUILD_SHARED_LIBS "Build shared library" ON)
option(BUILD_TESTS "Build test suite" ON)

# Compiler flags
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wno-unused-parameter
    )
endif()

# Library sources
set(LIBHAXIAL_SOURCES
    src/errors.c
    src/errno_convert.c
    src/crypto/sha1.c
    src/crypto/blowfish.c
    src/list.c
    src/pstring.c
    src/fnv1a.c
    src/random.c
)

# Public headers
set(LIBHAXIAL_PUBLIC_HEADERS
    include/haxial.h
    include/haxial/errors.h
    include/haxial/sha1.h
    include/haxial/blowfish.h
    include/haxial/list.h
    include/haxial/pstring.h
    include/haxial/fnv1a.h
    include/haxial/random.h
)

# Create library
add_library(haxial ${LIBHAXIAL_SOURCES})

# Include directories
target_include_directories(haxial
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Set library properties
set_target_properties(haxial PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "${LIBHAXIAL_PUBLIC_HEADERS}"
)

# Install targets
include(GNUInstallDirs)

install(TARGETS haxial
    EXPORT haxialTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/haxial
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

# Export configuration
install(EXPORT haxialTargets
    FILE haxialTargets.cmake
    NAMESPACE haxial::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/haxial
)

# Create config file
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/haxialConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/haxialConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/haxial
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/haxialConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/haxialConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/haxialConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/haxial
)

# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Summary
message(STATUS "libhaxial ${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C standard: C${CMAKE_C_STANDARD}")
message(STATUS "  Build shared: ${BUILD_SHARED_LIBS}")
message(STATUS "  Build tests: ${BUILD_TESTS}")
