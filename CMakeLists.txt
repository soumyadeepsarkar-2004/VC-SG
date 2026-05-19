cmake_minimum_required(VERSION 3.10)
project(VC_SG VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -O2)
endif()

# Common source files (shared across targets)
set(COMMON_SOURCES
    src/image_utils.cpp
    src/vcs.cpp
    src/rg.cpp
    src/dhcod.cpp
)

# Main program
add_executable(vc_program src/main.cpp ${COMMON_SOURCES})

# Analysis tool
add_executable(analyze src/analyze.cpp ${COMMON_SOURCES})

# Test suite
add_executable(vc_test src/test.cpp ${COMMON_SOURCES})

# Custom targets for convenience
add_custom_target(run
    COMMAND vc_program
    DEPENDS vc_program
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running main program..."
)

add_custom_target(run_analyze
    COMMAND analyze
    DEPENDS analyze
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running analysis tool..."
)

add_custom_target(run_test
    COMMAND vc_test
    DEPENDS vc_test
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running test suite..."
)
