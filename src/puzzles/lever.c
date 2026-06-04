#include "puzzles/lever.h"

#include "core/log.h"
#include "game/map.h"
#include "puzzles/puzzle.h"

void lever_puzzle_init(Puzzle* puzzle, i32 x, i32 y) {
    puzzle->position  = (vec2u) { .x = x, .y = y };
    puzzle->completed = false;
    puzzle->type      = PUZZLE_LEVER_TOGGLE;
    puzzle->update    = lever_puzzle_update;
    puzzle->state     = NULL;
}

void lever_puzzle_update(void* self, PuzzleEvent* event) {
    Puzzle* puzzle = (Puzzle*) self;
    if (event->type == PUZZLE_EVENT_PLAYER_INTERACT) {
        puzzle->completed                                              = true;
        gmap->tiles[puzzle->position.y][puzzle->position.x].texture_id = TILE_TEXTURE_LEVER_ON;
        log_info_f("Puzzle at (%d, %d) is complete", puzzle->position.x, puzzle->position.y);
    }
}
