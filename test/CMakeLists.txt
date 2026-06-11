function(add_escapepuzzle_test dir name)
  set(test_src "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/test_${name}.c")
  set(runner_src "${CMAKE_CURRENT_BINARY_DIR}/test_${name}_runner.c")

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

# CORE TESTS
add_escapepuzzle_test(core ht)
add_escapepuzzle_test(core locale)
add_escapepuzzle_test(core memory)
add_escapepuzzle_test(core random)
