/**
 * @file escapepuzzle.c
 * @brief Implementation of the escapepuzzle.h game functions.
 * This file provides the definitions for the functions declared in escapepuzzle.h.
 */
#include "escapepuzzle.h"
#include "core/config.h"

#include <stdio.h>

EMSCRIPTEN_KEEPALIVE const char* escapepuzzle_version(void) { return ESCAPEPUZZLE_VERSION; }
