/**
 * @file game/game.c
 * @brief Core game logic.
 */
#include "game/game.h"

#include "core/log.h"
#include "core/memory.h"
#include "game/map.h"
#include "game/puzzle.h"
#include "graphics/input.h"
#include "graphics/render.h"

#include <stdlib.h>
#include <time.h>

void game_init(GameState* game) {
    game->map.seed   = time(NULL);
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
        if (puzzle_update(game->map.puzzles, game->map.num_puzzles, events, num_puzzle_events)) {
            log_info_f("You won!");
            game_exit(game);
        }
        render_frame(game);
        mem_reset_frame();
    }
}

void game_exit(GameState* game) {
    // TODO Perform any necessary cleanup here
    exit(0);
}
