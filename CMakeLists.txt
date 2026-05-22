cmake_minimum_required(VERSION 3.31.6)
project("escapepuzzle" C)
include(CTest)

option(TEST "Enable tests" ON)

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
