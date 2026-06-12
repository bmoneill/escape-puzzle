#include "puzzles/lever.h"
#include <stdlib.h>
#include "core/log.h"
#include "game/map.h"
#include "puzzles/puzzle.h"

static i32 g_expected_lever = 1;

void lever_puzzle_init(Puzzle* puzzle, i32 x, i32 y) {
    puzzle->position  = (vec2u) { .x = x, .y = y };
    puzzle->completed = false;
    puzzle->type      = PUZZLE_LEVER_TOGGLE;
    puzzle->update    = lever_puzzle_update;
    LeverState* state = malloc(sizeof(LeverState));
    state->order = 0;
    state->activated = false;
    puzzle->state     = state;
}

void lever_puzzle_update(void* self, PuzzleEvent* event) {
    static i8 g_reset_lock = 0;
      Puzzle* puzzle = (Puzzle*)self;

    if (event->type != PUZZLE_EVENT_PLAYER_INTERACT)
        return;

    LeverState* state = (LeverState*)puzzle->state;

    if (g_reset_lock)
         return;

    if (state->order == g_expected_lever) {


        puzzle->completed = true;

        gmap->tiles[puzzle->position.y][puzzle->position.x].texture_id =
            TILE_TEXTURE_LEVER_ON;

        g_expected_lever++;

        log_info_f(
            "Correct lever %d activated",
            state->order
        );
    }
    else {

        log_info_f("Wrong lever activated. Resetting sequence.");

        g_expected_lever = 1;

        for (i32 i = 0; i < gmap->num_puzzles; i++) {

            Puzzle* p = &gmap->puzzles[i];

            p->completed = false;

            gmap->tiles[p->position.y][p->position.x].texture_id =
                TILE_TEXTURE_LEVER_OFF;
        }
    }
     
    }