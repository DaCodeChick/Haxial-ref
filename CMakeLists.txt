cmake_minimum_required(VERSION 3.20)
project(Haxial VERSION 0.1.0 LANGUAGES C CXX)

# C11 for library/server/tracker, C++23 for client
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for editor integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project options
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_SERVER "Build haxial-server" ON)
option(BUILD_TRACKER "Build haxial-tracker" ON)
option(BUILD_CLIENT "Build haxial-client" ON)
option(BUILD_TOOLS "Build analysis tools" ON)
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)

# Include CMake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(PlatformDetection)
include(CompilerWarnings)

# Platform-specific configuration
detect_platform()

# Position independent code for shared libraries
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Sanitizers
if(ENABLE_ASAN)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

if(ENABLE_UBSAN)
    add_compile_options(-fsanitize=undefined)
    add_link_options(-fsanitize=undefined)
endif()

# Add subdirectories
add_subdirectory(lib)

if(BUILD_SERVER)
    add_subdirectory(server)
endif()

if(BUILD_TRACKER)
    add_subdirectory(tracker)
endif()

if(BUILD_CLIENT)
    find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)
    add_subdirectory(client)
endif()

if(BUILD_TOOLS)
    add_subdirectory(tools)
endif()

# Install documentation
install(FILES README.md AGENTS.md DESTINATION share/doc/haxial)
install(DIRECTORY docs/ DESTINATION share/doc/haxial/docs)

# CPack configuration
set(CPACK_PACKAGE_NAME "haxial")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Haxial KDX Protocol Implementation")
set(CPACK_PACKAGE_VENDOR "Haxial Project")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE-GPL-3.0")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
include(CPack)
