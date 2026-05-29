// tile.h
#ifndef TILE_H
#define TILE_H

typedef enum {
    TILE_FLOOR,
    TILE_WALL,
    TILE_LEVER,
    TILE_EXIT
} TileType;

typedef struct {
    TileType type;
    int texture_id;
    int solid;
} Tile;

#endif