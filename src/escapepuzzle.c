/**
 * @file escapepuzzle.c
 * @brief Implementation of the escapepuzzle.h game functions.
 * This file provides the definitions for the functions declared in escapepuzzle.h.
 */
#include "escapepuzzle.h"

#include "ui/ui.h"

#include <stdio.h>

int start_game(void) {
<<<<<<< HEAD
    printf("Welcome to the Escape Puzzle Game!\n");
    printf("Test commit ignore");
=======
    ui_init(&(UI) { 0 });
>>>>>>> 8d41d9c65597102b9646521c04723c06dffd9672
    return 0;
}

const char* escapepuzzle_version(void) { return ESCAPEPUZZLE_VERSION; }

printf("Test commit ignore");