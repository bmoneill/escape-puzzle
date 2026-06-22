/**
 * @file puzzles/lever.c
 */
#include "puzzles/lever.h"
#include "core/log.h"
#include "game/map.h"
#include "puzzles/puzzle.h"
#include <stdlib.h>

static i32 g_expected_lever      = 1;
static i8  g_wrong_message_shown = 0;

void       lever_puzzle_init(Puzzle* puzzle, i32 x, i32 y) {
    puzzle->position  = (vec2u) { .x = x, .y = y };
    puzzle->completed = false;
    puzzle->type      = PUZZLE_LEVER_TOGGLE;
    puzzle->update    = lever_puzzle_update;

    LeverState* state = malloc(sizeof(LeverState));
    state->order      = 0;
    state->activated  = false;

    puzzle->state     = state;
}

void lever_puzzle_update(void* self, PuzzleEvent* event) {

    Puzzle* puzzle = (Puzzle*) self;

    if (event->type != PUZZLE_EVENT_PLAYER_INTERACT)
        return;

    LeverState* state = (LeverState*) puzzle->state;

    if (state->order == g_expected_lever) {

        g_wrong_message_shown = 0;

        // FIX: logical activation state (this was missing before)
        state->activated                                               = true;

        puzzle->completed                                              = true;

        gmap->tiles[puzzle->position.y][puzzle->position.x].texture_id = TILE_TEXTURE_LEVER_ON;

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
                gmap->tiles[p->position.y][p->position.x].texture_id = TILE_TEXTURE_LEVER_OFF;
            }
        }
    }
}
