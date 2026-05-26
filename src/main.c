#include <stdio.h>

#include "core/locale.h"
#include "core/log.h"
#include "core/memory.h"
#include "core/random.h"

#include "escapepuzzle.h"

int main(void) {
    log_info_f("Escape Puzzle Game Version: %s\n", escapepuzzle_version());
    log_info_f("Starting game...\n");
    mem_init();
    locale_init() return start_game();
}
