/**
 * @file game/game.c
 * @brief Core game logic.
 */
#include "game/game.h"

#include "core/config.h"
#include "core/log.h"
#include "core/memory.h"
#include "core/random.h"
#include "game/map.h"
#include "game/tile.h"
#include "graphics/input.h"
#include "graphics/menu.h"
#include "graphics/render.h"
#include "puzzles/puzzle.h"

#include <stdlib.h>
#include <time.h>

i64  seed   = 0;
bool level1 = true;

/* -------------------------------------------------------------------------
 * Static helpers for the level-complete menu
 * ---------------------------------------------------------------------- */

/**
 * MenuItem action wrapper: starts a new random level.
 * Passes seed=0 so game_init picks a fresh random seed.
 */
EMSCRIPTEN_KEEPALIVE static void start_next_level(GameState* game) { game_init(game); }

/**
 * Shows the level-complete modal menu and blocks until the player chooses.
 *
 * "Next Level" → menu_init allocates a fresh GameState and calls
 *   start_next_level, which runs a brand-new game_init loop.
 * "Quit"       → game_exit terminates the process.
 *
 * If the player closes the window or presses Escape, menu_init returns
 * without selecting an item and game_init unwinds back to the main menu.
 */
EMSCRIPTEN_KEEPALIVE static void game_level_complete(GameState* game) {
    (void) game;
    menu_init(
        (MenuItem[]){
            { "Next Level", start_next_level },
            { "Quit", game_exit },
        },
        2);
}

/* -------------------------------------------------------------------------
 * Public API
 * ---------------------------------------------------------------------- */

EMSCRIPTEN_KEEPALIVE void game_init(GameState* game) {
    if (level1) {
        if (seed) {
            random_seed((u64) seed);
        } else {
            random_seed((u64) time(NULL));
        }

        level1 = false;
    }

    game->map.seed = random_i64();

    log_info_f("Level seed: %lld\n", game->map.seed);
    game->map.width  = MAX_MAP_WIDTH;
    game->map.height = MAX_MAP_HEIGHT;
    map_init(&game->map);
    player_init(&game->player);
    game->player.x = game->map.playerStartPos.x * TILE_SIZE;
    game->player.y = game->map.playerStartPos.y * TILE_SIZE;

    while (1) {
        i16 keys_pressed = get_keys_pressed();
        i32 num_puzzle_events;

        player_update(&game->player, &game->map, keys_pressed);

        PuzzleEvent* events = puzzle_generate_events(game->player.x / TILE_SIZE,
                                                     game->player.y / TILE_SIZE,
                                                     keys_pressed,
                                                     game->map.puzzles,
                                                     game->map.num_puzzles,
                                                     &num_puzzle_events);

        // Update lever puzzles (optional content — does not trigger win)
        puzzle_update(game->map.puzzles, game->map.num_puzzles, events, num_puzzle_events);

        // Win condition: player steps onto the exit tile
        i32         px                 = (i32) (game->player.x / TILE_SIZE);
        i32         py                 = (i32) (game->player.y / TILE_SIZE);
        Tile*       cur_tile           = map_get_tile(&game->map, px, py);

        static bool exit_warning_shown = false;

        if (cur_tile && cur_tile->type == TILE_EXIT) {

            if (map_all_levers_active(&game->map)) {
                log_info_f("You escaped! Level complete!");
                game_level_complete(game);
                return;
            } else if (!exit_warning_shown) {
                log_info_f("All levers must be activated before you can escape!");
                exit_warning_shown = true;
            }
        } else {
            exit_warning_shown = false;
        }

        render_frame(game);
        mem_reset_frame();
        WASM_YIELD();
    }
}

EMSCRIPTEN_KEEPALIVE void game_exit(GameState* game) {
    // TODO Perform any necessary cleanup here
    (void) game;
    exit(0);
}
