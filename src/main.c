#include <stdio.h>

#include "escapepuzzle.h"

int main(void) {
    printf("Escape Puzzle Game Version: %s\n", escapepuzzle_version());
    printf("Starting game...\n");
    return start_game();
}
