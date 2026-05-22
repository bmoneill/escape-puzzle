function(add_escapepuzzle_test name)
  set(test_src "${CMAKE_CURRENT_SOURCE_DIR}/test_${name}.c")
  set(runner_src "${CMAKE_CURRENT_BINARY_DIR}/test_${name}_runner.c")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DESCAPEPUZZLE_VERSION='\"${GIT_COMMIT_HASH}\"'")

  add_custom_command(
        OUTPUT ${runner_src}
        COMMAND ruby ${CMAKE_SOURCE_DIR}/external/Unity/auto/generate_test_runner.rb ${test_src} ${runner_src}
        DEPENDS ${test_src} ${CMAKE_SOURCE_DIR}/external/Unity/auto/generate_test_runner.rb
        COMMENT "Generating ${runner_src}"
    )

  # Define the test executable
  add_executable(${name}_tests ${test_src} ${runner_src})
  target_include_directories(${name}_tests PRIVATE ${CMAKE_SOURCE_DIR}/test)

  # Replace mylib with your library name if you have renamed it
  target_link_libraries(${name}_tests libescapepuzzle Unity)

  # Register as a CTest test
  add_test(NAME ${name} COMMAND ${name}_tests)
endfunction()

file(GLOB TEST_FILES "test_*.c")
foreach(test_file ${TEST_FILES})
  get_filename_component(test_name ${test_file} NAME_WE)
  string(REPLACE "test_" "" test_name ${test_name})
  add_escapepuzzle_test(${test_name})
endforeach()
