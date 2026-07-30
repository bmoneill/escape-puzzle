add_library(Unity STATIC
  Unity/src/unity.c
)

target_include_directories(Unity PUBLIC
  Unity/src
)

if(WASM)
    # Force raylib into Web/Emscripten mode.  emcmake would set EMSCRIPTEN
    # automatically, but an explicit cache entry ensures the subdirectory
    # picks it up even if CMakeOptions.txt runs before our parent sets it.
    set(PLATFORM "Web" CACHE STRING "Platform to build for." FORCE)
endif()

add_subdirectory(raylib)

add_library(arena STATIC
  arena/src/arena/arena.c
)

target_include_directories(arena PUBLIC
  arena/src/arena
)
