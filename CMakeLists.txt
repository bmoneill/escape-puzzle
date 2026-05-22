cmake_minimum_required(VERSION 3.31.6)
project("escapepuzzle" C)
include(CTest)

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

function(Build_Game)
  add_subdirectory(src)
  include_directories(src)
endfunction()

function(Build_Tests)
  add_subdirectory(external)
  add_subdirectory(test)
endfunction()

Build_Game()
if(TEST)
  Build_Tests()
endif()
