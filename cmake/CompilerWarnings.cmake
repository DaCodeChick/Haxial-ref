# CompilerWarnings.cmake - Set strict compiler warning flags

function(set_project_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE
            /W4              # High warning level
            /WX              # Treat warnings as errors
            /permissive-     # Standards conformance
            /w14640          # Thread-safe static initialization
            /w14826          # Pointer conversion warning
            /w14905          # String literal mismatch
            /w14906          # Wide string literal mismatch
        )
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror
            -Wshadow
            -Wformat=2
            -Wunused
            -Wcast-align
            -Wconversion
            -Wsign-conversion
            -Wmissing-declarations
            -Wdouble-promotion
            $<$<COMPILE_LANGUAGE:C>:
                -Wstrict-prototypes
                -Wmissing-prototypes
            >
            $<$<COMPILE_LANGUAGE:CXX>:
                -Wnon-virtual-dtor
                -Woverloaded-virtual
            >
        )
        
        # GCC-specific
        if(CMAKE_C_COMPILER_ID MATCHES "GNU")
            target_compile_options(${target} PRIVATE
                -Wlogical-op
                -Wduplicated-cond
                -Wduplicated-branches
            )
        endif()
        
        # Clang-specific
        if(CMAKE_C_COMPILER_ID MATCHES "Clang")
            target_compile_options(${target} PRIVATE
                -Wno-gnu-zero-variadic-macro-arguments
            )
        endif()
    endif()
endfunction()
