project("escapepuzzle" C)
include(GNUInstallDirs)

# Windows stuff
if(CMAKE_PLATFORM_ID STREQUAL "Windows")
  set(CMAKE_C_COMPILER "x86_64-w64-mingw32-gcc")
endif()

######################
### COMPILER FLAGS ###
######################


# C Standard
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c17 -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=700 -D_FILE_OFFSET_BITS=64")

# Warnings
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wno-missing-field-initializers")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-prototypes -Wshadow")

# Optimization
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -ffast-math")

#########################
### SOURCES & HEADERS ###
#########################

include_directories(
  "${CMAKE_SOURCE_DIR}/../external/raylib/src"
  )

# We don't include main.c here because it's not used for testing
set(SOURCES
  "escapepuzzle.c"
)

add_executable(${PROJECT_NAME} ${SOURCES} "main.c")
target_include_directories(${PROJECT_NAME} PUBLIC
  "${CMAKE_SOURCE_DIR}/src"
  "${CMAKE_SOURCE_DIR}/../external/raylib/src"
)
target_link_libraries(${PROJECT_NAME} raylib)

if(TEST)
  add_library(lib${PROJECT_NAME} STATIC ${SOURCES})
  target_include_directories(lib${PROJECT_NAME} PUBLIC
  "${CMAKE_SOURCE_DIR}/src"
  "${CMAKE_SOURCE_DIR}/../external/raylib/src"
)
  target_link_libraries(lib${PROJECT_NAME} raylib)
endif()
