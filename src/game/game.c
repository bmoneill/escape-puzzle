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

/**
 * Runs a single iteration of the gameplay update/draw cycle: input, player
 * movement, puzzle events, the win-condition check, and drawing the frame.
 *
 * On native builds this is driven by a plain while(1) loop in game_init().
 * On WASM builds it is instead registered as the Emscripten main loop
 * callback. Per raylib's web guidelines
 * (https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5)), the
 * preferred approach for the web is to avoid a blocking
 * while(!WindowShouldClose()) loop entirely and instead hand a single
 * Update-Draw callback to the browser, which then drives frame timing
 * itself via requestAnimationFrame. This avoids the small performance
 * penalty ASYNCIFY's emscripten_sleep()-based yielding incurs on the
 * hot per-frame loop, and lets the browser correctly pause/resume the
 * loop (e.g. when the tab is inactive).
 *
 * The rest of the game (menus, text input prompts) still uses nested
 * blocking while(1) loops kept alive via ASYNCIFY, since converting those
 * to callbacks would require a full state-machine refactor; only this
 * hot gameplay loop is worth adapting to the canonical WASM pattern.
 */
static void game_frame(void* arg) {
    GameState* game         = (GameState*) arg;

    i16        keys_pressed = get_keys_pressed();
    i32        num_puzzle_events;

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
#ifdef __EMSCRIPTEN__
            // Stop the browser from scheduling further frames before handing
            // control off to the (still ASYNCIFY-backed) level-complete menu.
            emscripten_cancel_main_loop();
#endif
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
}

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

#ifdef __EMSCRIPTEN__
    // Per raylib's WASM guidelines: hand frame scheduling over to the
    // browser instead of blocking in a while() loop. fps=0 lets the
    // browser drive timing via requestAnimationFrame, and
    // simulate_infinite_loop=1 tells Emscripten this call never returns
    // (control unwinds back to the browser's event loop immediately).
    emscripten_set_main_loop_arg(game_frame, game, 0, 1);
#else
    while (1) {
        game_frame(game);
    }
#endif
}

EMSCRIPTEN_KEEPALIVE void game_exit(GameState* game) {
    // TODO Perform any necessary cleanup here
    (void) game;
    exit(0);
}
