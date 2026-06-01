cmake_minimum_required(VERSION 3.31.6)
project("escapepuzzle" C)
include(CTest)

option(RAYLIB "Use Raylib for Graphics" ON)
option(TEST "Enable tests" ON)

# Get git commit hash for version
execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Project Version
add_compile_definitions("ESCAPEPUZZLE_VERSION=\"${GIT_COMMIT_HASH}\"")

add_subdirectory(external)
add_subdirectory(src)

if(TEST)
  add_subdirectory(test)
endif()
