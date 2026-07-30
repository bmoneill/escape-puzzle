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

# We don't include main.c here because it's not used for testing
set(SOURCES
  "escapepuzzle.c"
  "core/ht.c"
  "core/locale.c"
  "core/log.c"
  "core/memory.c"
  "core/random.c"
  "core/strings.c"

  "game/game.c"
  "game/main_menu.c"
  "game/map.c"
  "game/player.c"

  "puzzles/cipher.c"
  "puzzles/lever.c"
  "puzzles/puzzle.c"
  "puzzles/key.c"
  "puzzles/door.c"
)

if (RAYLIB)
  set(SOURCES ${SOURCES}
    "graphics/raylib/input.c"
    "graphics/raylib/menu.c"
    "graphics/raylib/render.c"
    "graphics/raylib/textinput.c"
  )
endif()

add_executable(${PROJECT_NAME} ${SOURCES} "main.c")
target_include_directories(${PROJECT_NAME} PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}"
)
target_link_libraries(${PROJECT_NAME} raylib arena)

if(WASM)
    # Output a self-contained HTML page ready for web deployment.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")

    # Preload all game assets into the Emscripten virtual filesystem so that
    # fopen()/LoadTexture() calls resolve correctly inside the browser sandbox.
    set(WASM_ASSETS_FLAG "--preload-file=${CMAKE_SOURCE_DIR}/assets@assets")
    set(WASM_SHELL_FLAG  "--shell-file=${CMAKE_SOURCE_DIR}/web/shell.html")

    target_link_options(${PROJECT_NAME} PRIVATE
        # The core gameplay loop (game_init()/game_frame() in game/game.c)
        # follows raylib's WASM guidelines directly: it registers a per-frame
        # callback via emscripten_set_main_loop_arg() instead of blocking in a
        # while() loop, so the browser drives frame timing itself.
        #
        # Everything else (main menu, level-complete menu, text input prompts)
        # still uses nested while(1) loops. Asyncify lets those yield to the
        # browser each frame via WASM_YIELD() without requiring a full
        # state-machine refactor. The stack size is increased from the
        # default 4 KiB because the menu -> game -> menu call chain can be
        # several frames deep.
        -sASYNCIFY=1
        -sASYNCIFY_STACK_SIZE=131072

        -sUSE_GLFW=3           # Emscripten's built-in GLFW emulation for raylib
        -sWASM=1
        -sALLOW_MEMORY_GROWTH=1
        "${WASM_ASSETS_FLAG}"
        "${WASM_SHELL_FLAG}"
    )
endif()

# Copy assets next to the executable so LoadTexture() can find them at runtime
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)

if(LINUX)
  target_link_libraries(${PROJECT_NAME} m)
endif()

if(TEST)
  add_library(lib${PROJECT_NAME} STATIC ${SOURCES})
  target_include_directories(lib${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
  )
  target_link_libraries(lib${PROJECT_NAME} raylib arena)

  if(LINUX)
    target_link_libraries(lib${PROJECT_NAME} m)
  endif()
endif()
