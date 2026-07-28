/**
 * @file puzzles/lever.c
 */
#include "puzzles/lever.h"

#include "core/config.h"
#include "core/log.h"
#include "game/map.h"
#include "puzzles/puzzle.h"
#include <stdlib.h>

static i32                g_expected_lever = 1;

EMSCRIPTEN_KEEPALIVE void lever_puzzle_init(Puzzle* puzzle, i32 x, i32 y) {
    puzzle->position  = (vec2u){ .x = x, .y = y };
    puzzle->completed = false;
    puzzle->type      = PUZZLE_LEVER_TOGGLE;
    puzzle->update    = lever_puzzle_update;

    LeverState* state = malloc(sizeof(LeverState));
    state->order      = 0;
    state->activated  = false;

    puzzle->state     = state;
}

EMSCRIPTEN_KEEPALIVE void lever_puzzle_update(void* self, PuzzleEvent* event) {

    Puzzle* puzzle = (Puzzle*) self;

    if (event->type != PUZZLE_EVENT_PLAYER_INTERACT)
        return;

    LeverState* state = (LeverState*) puzzle->state;

    if (state->order == g_expected_lever) {

        // FIX: logical activation state (this was missing before)
        state->activated  = true;

        puzzle->completed = true;

        // Only floor levers switch their texture_id to the "on" sprite.
        // Hidden (wall) levers keep TILE_TEXTURE_HIDDEN_LEVER; the renderer
        // picks the on/off wall-lever sprite based on state->activated.
        Tile* tile = &gmap->tiles[puzzle->position.y][puzzle->position.x];
        if (tile->type != TILE_HIDDEN_LEVER) {
            tile->texture_id = TILE_TEXTURE_LEVER_ON;
        }

        g_expected_lever++;

        log_info_f("Correct lever %d activated", state->order);
    } else {

        if (g_expected_lever == 1) {

            log_info_f("Wrong lever. Try again.");

        } else {

            log_info_f("Wrong lever. Resetting sequence.");

            g_expected_lever = 1;

            for (i32 i = 0; i < gmap->num_puzzles; i++) {

                Puzzle* p    = &gmap->puzzles[i];

                p->completed = false;

                // NOTE: this resets visuals, but NOT state->activated (intentional? depends on design)
                // Only floor levers use TILE_TEXTURE_LEVER_OFF; hidden (wall)
                // levers keep TILE_TEXTURE_HIDDEN_LEVER and are rendered
                // based on their LeverState->activated flag instead.
                Tile* t = &gmap->tiles[p->position.y][p->position.x];
                if (t->type != TILE_HIDDEN_LEVER) {
                    t->texture_id = TILE_TEXTURE_LEVER_OFF;
                }
            }
        }
    }
}
