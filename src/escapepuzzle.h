/**
 * @file escapepuzzle.h
 * @brief Header file for the game entry point
 */
#ifndef ESCAPEPUZZLE_H
#define ESCAPEPUZZLE_H

#ifndef ESCAPEPUZZLE_VERSION
#define ESCAPEPUZZLE_VERSION "unknown"
#endif

/**
 * @brief Starts the Escape Puzzle game.
 * This function initializes the game and begins the main game loop.
 * @return An integer status code indicating the result of the game.
 */
int start_game(void);

/**
 * @brief Retrieves the version of the Escape Puzzle game.
 * This function returns a string representing the current version of the game.
 * @return A constant character pointer to the version string.
 */
const char* escapepuzzle_version(void);

#endif
