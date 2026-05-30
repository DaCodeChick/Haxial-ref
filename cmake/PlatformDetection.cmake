# PlatformDetection.cmake - Detect and configure platform-specific settings

function(detect_platform)
    message(STATUS "Detecting platform configuration...")
    
    # Detect operating system
    if(WIN32)
        set(HAXIAL_PLATFORM "Windows" PARENT_SCOPE)
        set(HAXIAL_WINDOWS TRUE PARENT_SCOPE)
        message(STATUS "Platform: Windows")
        
        # Require UCRT
        if(MSVC)
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL" PARENT_SCOPE)
        endif()
        
        # Winsock2
        set(HAXIAL_SOCKET_LIBS "ws2_32" PARENT_SCOPE)
        
    elseif(APPLE)
        set(HAXIAL_PLATFORM "macOS" PARENT_SCOPE)
        set(HAXIAL_MACOS TRUE PARENT_SCOPE)
        message(STATUS "Platform: macOS")
        
        # POSIX sockets
        set(HAXIAL_SOCKET_LIBS "" PARENT_SCOPE)
        
        # Require modern macOS
        if(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
            set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" PARENT_SCOPE)
        endif()
        
    elseif(UNIX)
        set(HAXIAL_PLATFORM "Linux" PARENT_SCOPE)
        set(HAXIAL_LINUX TRUE PARENT_SCOPE)
        message(STATUS "Platform: Linux")
        
        # POSIX sockets
        set(HAXIAL_SOCKET_LIBS "" PARENT_SCOPE)
        
        # Pthread
        find_package(Threads REQUIRED)
        set(HAXIAL_THREAD_LIBS "Threads::Threads" PARENT_SCOPE)
        
    else()
        message(FATAL_ERROR "Unsupported platform")
    endif()
    
    # Detect architecture
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(HAXIAL_ARCH "64-bit" PARENT_SCOPE)
        message(STATUS "Architecture: 64-bit")
    else()
        set(HAXIAL_ARCH "32-bit" PARENT_SCOPE)
        message(STATUS "Architecture: 32-bit")
    endif()
    
    # Detect C library
    if(UNIX AND NOT APPLE)
        execute_process(
            COMMAND ldd --version
            OUTPUT_VARIABLE LDD_OUTPUT
            ERROR_QUIET
        )
        if(LDD_OUTPUT MATCHES "GLIBC")
            message(STATUS "C Library: glibc")
            set(HAXIAL_LIBC "glibc" PARENT_SCOPE)
        elseif(LDD_OUTPUT MATCHES "musl")
            message(STATUS "C Library: musl")
            set(HAXIAL_LIBC "musl" PARENT_SCOPE)
        else()
            message(STATUS "C Library: unknown")
        endif()
    endif()
    
endfunction()
