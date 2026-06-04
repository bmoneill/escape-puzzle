/**
 * @file puzzles/lever.h
 * @brief Header for a lever toggle puzzle, which simply toggles its completion state when interacted with
 */
#ifndef PUZZLES_LEVER_H
#define PUZZLES_LEVER_H

#include "core/types.h"
#include "puzzles/puzzle.h"

/**
 * @brief State structure for a lever toggle puzzle, which tracks whether the lever is currently on or off.
 */
typedef struct {
    bool isOn; //!< Indicates whether the lever is currently on (true) or off (false).
} LeverTogglePuzzleState;

/**
 * @brief Initializes a lever toggle puzzle at the specified coordinates, setting its type and update function.
 * @param puzzle Pointer to the Puzzle instance to initialize.
 * @param x The x-coordinate of the puzzle in tile units.
 * @param y The y-coordinate of the puzzle in tile units.
 */
void lever_puzzle_init(Puzzle* puzzle, i32 x, i32 y);

/**
 * @brief Update function for a lever puzzle, which toggles the puzzle's completion state based on the provided state.
 * @param self Pointer to the Puzzle instance being updated.
 * @param state Pointer to the PuzzleEvent
 */
void lever_puzzle_update(void* self, PuzzleEvent* state);

#endif
