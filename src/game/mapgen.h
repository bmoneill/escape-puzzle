#ifndef MAPGEN_H
#define MAPGEN_H

#include "core/types.h"
#include "game/map.h"

typedef enum {
    THEME_NONE,
    THEME_DUNGEON,
    THEME_TEMPLE,
    THEME_PYRAMID,
} MapTheme;

typedef struct {
    i32      width;
    i32      height;
    i64      seed;
    f32      difficulty; //!< Unused for now
    MapTheme theme; //!< Unused for now
} MapParams;

void generate_map(Map* map, MapParams* params);

#endif
