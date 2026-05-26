add_library(Unity STATIC
  Unity/src/unity.c
)

target_include_directories(Unity PUBLIC
  Unity/src
)

add_subdirectory(raylib)

add_library(arena STATIC
  arena/src/arena/arena.c
)

target_include_directories(arena PUBLIC
  arena/src/arena
)
