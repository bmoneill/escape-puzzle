/**
 * @file escapepuzzle.c
 * @brief Implementation of the escapepuzzle.h game functions.
 * This file provides the definitions for the functions declared in escapepuzzle.h.
 */
#include "escapepuzzle.h"

#include "ui/ui.h"

#include <stdio.h>

int start_game(void) {
    printf("Welcome to the Escape Puzzle Game!\n");
<<<<<<< HEAD
    printf("Test commit ignore");
=======
>>>>>>> caa678744798bc2d2cb7352bf01cd57a09c467b5
    ui_init(&(UI) { 0 });
    return 0;
}

<<<<<<< HEAD
const char* escapepuzzle_version(void) { return ESCAPEPUZZLE_VERSION; }
=======
const char* escapepuzzle_version(void) { return ESCAPEPUZZLE_VERSION; }

>>>>>>> caa678744798bc2d2cb7352bf01cd57a09c467b5
