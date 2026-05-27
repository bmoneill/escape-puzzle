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
    printf("Test commit");
    printf("Test2");
    ui_init(&(UI) { 0 });
    return 0;
}

const char* escapepuzzle_version(void) { return ESCAPEPUZZLE_VERSION; }

