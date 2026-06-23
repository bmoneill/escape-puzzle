
#include "core/locale.h"
#include "core/log.h"
#include "core/memory.h"
#include "core/random.h"
#include "game/game.h"
#include "game/main_menu.h"
#include "graphics/render.h"

#include "escapepuzzle.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc > 1) {
        seed = atoi(argv[1]);
    }
    log_info_f("Escape Puzzle Game Version: %s\n", escapepuzzle_version());

    log_info_f("Starting game...\n");
    mem_init();
    locale_init(NULL);
    random_init();
    render_init();

    main_menu_init();
}
