# Project Architecture

The project is structured into several modules, each responsible for a specific
aspect of the game. This modular design allows for better organization and
maintainability, as well as making it easier to port the game to different
platforms (e.g. web, mobile).

Most modules are designed to have a similar lifecycle: `MODULE_init`, `MODULE_update`,
`MODULE_render` (where applicable), and `MODULE_shutdown`.

## Table of Contents

- [Core Modules](#core-modules)
  - [Hash Table](#hash-table)
  - [Locale](#locale)
  - [Logging](#logging)
  - [Memory](#memory)
  - [Random](#random)
  - [Types](#types)
- [Game Logic Modules](#game-logic-modules)
  - [Game](#game)
  - [Main Menu](#main-menu)
  - [Map](#map)
  - [Player](#player)
- [Graphics Modules](#graphics-modules)
  - [Input](#input)
  - [Menu](#menu)
  - [Render](#render)

## Core Modules

### Hash Table

File: `core/ht.c`

This is an implementation of a hash table, which is used for storing key-value
pairs. This can be used for various purposes in the project,
but is not used anywhere yet.

### Locale

File: `core/locale.c`

This module is responsible for handling localization and internationalization
in the game. It provides functions to load and manage language files, as well as
to retrieve localized strings based on the current language setting.

Currently the only supported language is US English.

### Logging

File: `core/log.c`

This module adds logging functions (e.g. `log_info`, `log_warning`, `log_error`).

## Memory

File: `core/memory.c`

This module provides memory management functions, such as allocating and freeing
memory, as well as tracking memory usage and detecting leaks.

This project uses an arena allocator, which allows for efficient memory management.
For example, once the player completes a level, all memory associated with that
level can be freed in one operation, rather than having to track and free
individual allocations.

Four arenas are used in the project:

- `MEM_TEMP`: Temporary memory for short-term use. TODO: Figure out when this
  should be cleared or exist at all.
- `MEM_FRAME`: Memory for the current frame, freed after frame is drawn.
- `MEM_LEVEL`: Memory for the current level, freed when player completes level.
- `MEM_PERM`: Permanent memory for the entire game, freed when game is closed.
  This is used for things like textures and other resources that are loaded once
  and used throughout the game.

## Random

File: `core/random.c`

This module provides functions for generating random numbers of various types.

## Types

File: `core/types.h`

This is simply a header that defines common types used throughout the project,
including integer types, float types, and vectors.

## Game Logic Modules

### Game

File: `game/game.c`

This module contains the main game loop and overall game management. It initializes
the game state, handles user input, updates the game world, and renders the
game.

#### Main Menu

File: `game/main_menu.c`

This module functions similar to game.c, but simply manages the main menu. On
`main_menu_init`, the menu will be displayed and the function will loop until
the player chooses an option.

### Map

File: `game/map.c`

TODO

### Player

File: `game/player.c`

TODO

## Graphics Modules

Graphics modules use a rendering backend (just raylib right now), which
is abstracted away behind a common interface. This allows for easy portability.

### Input

File: `graphics/input.h`
Raylib implementation: `graphics/raylib/input.c`

This module handles keyboard input.

### Menu

File: `graphics/menu.h`
Raylib implementation: `graphics/raylib/menu.c`

This handles rendering of menus (e.g. main menu, settings when implemented).

### Render

File: `graphics/render.h`
Raylib implementation: `graphics/raylib/render.c`

This module handles rendering of the map and player.
