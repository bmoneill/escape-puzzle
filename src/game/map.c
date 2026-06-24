/**
 * @file game/map.c
 * @brief Map implementation.
 */
#include "map.h"

#include "core/log.h"
#include "core/random.h"
#include "puzzles/cipher.h"
#include "puzzles/door.h"
#include "puzzles/key.h"
#include "puzzles/lever.h"

Map* gmap = NULL;

// ---------------------------------------------------------------------------
// Internal types used only during generation
// ---------------------------------------------------------------------------

// Number of rooms in the 3×2 grid layout
#define NUM_ROOMS 6
// Maximum corridors: 5 spanning-tree edges + 1 optional shortcut
#define MAX_CORRIDORS 8

// Axis-aligned bounds of a room's floor area (tile coords, inclusive)
typedef struct {
    i32 x1, y1, x2, y2;
} MapRoom;

// A carved gap in a shared wall that connects two rooms
typedef struct {
    i32  room_a, room_b; // indices into rooms[]
    i32  x, y; // tile position of the gap
    bool locked; // true when a locked door sits here
} Corridor;

// ---------------------------------------------------------------------------
// Forward declarations
// ---------------------------------------------------------------------------

static void generate_walls(Map* map);
static void generate_puzzles(Map* map);

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

void map_init(Map* map) {
    gmap = map;
    map_generate(map);
}

void map_generate(Map* map) {
    random_seed(map->seed);

    map->num_puzzles       = 0;
    map->currentLeverOrder = 0;

    generate_walls(map); // rooms, corridors, doors, keys, exit
    generate_puzzles(map); // optional lever puzzles
}

Tile* map_get_tile(Map* map, int x, int y) {
    if (x < 0 || y < 0 || x >= map->width || y >= map->height)
        return NULL;
    return &map->tiles[y][x];
}

bool tile_adjacent_to(Map* map, int x, int y, TileType type) {
    Tile* t = map_get_tile(map, x + 1, y);
    if (t->type == type) {
        return true;
    }

    t = map_get_tile(map, x - 1, y);
    if (t->type == type) {
        return true;
    }

    t = map_get_tile(map, x, y + 1);
    if (t->type == type) {
        return true;
    }

    t = map_get_tile(map, x, y - 1);
    if (t->type == type) {
        return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// Room and wall generation
// ---------------------------------------------------------------------------

static void generate_walls(Map* map) {

    // ------------------------------------------------------------------
    // 1. Fill the entire map with solid wall tiles
    // ------------------------------------------------------------------
    for (i32 y = 0; y < map->height; y++) {
        for (i32 x = 0; x < map->width; x++) {
            map->tiles[y][x].type       = TILE_WALL;
            map->tiles[y][x].solid      = 1;
            map->tiles[y][x].texture_id = TILE_TEXTURE_WALL;
        }
    }

    // ------------------------------------------------------------------
    // 2. Choose random divider positions for a 3×2 room grid
    //
    //  Columns: | room 0 | vsplit1 | room 1 | vsplit2 | room 2 |
    //  Rows:    | top rooms        | hsplit  | bottom rooms     |
    // ------------------------------------------------------------------
    i32 vsplit1 = random_i32_range(5, 8);
    i32 vsplit2 = random_i32_range(vsplit1 + 5, map->width - 5);
    i32 hsplit  = random_i32_range(5, map->height - 5);

    // ------------------------------------------------------------------
    // 3. Define the six rooms (floor tile bounds, inclusive)
    // ------------------------------------------------------------------
    MapRoom rooms[NUM_ROOMS];
    // Top row
    rooms[0].x1 = 1;
    rooms[0].y1 = 1;
    rooms[0].x2 = vsplit1 - 1;
    rooms[0].y2 = hsplit - 1;

    rooms[1].x1 = vsplit1 + 1;
    rooms[1].y1 = 1;
    rooms[1].x2 = vsplit2 - 1;
    rooms[1].y2 = hsplit - 1;

    rooms[2].x1 = vsplit2 + 1;
    rooms[2].y1 = 1;
    rooms[2].x2 = map->width - 2;
    rooms[2].y2 = hsplit - 1;

    // Bottom row
    rooms[3].x1 = 1;
    rooms[3].y1 = hsplit + 1;
    rooms[3].x2 = vsplit1 - 1;
    rooms[3].y2 = map->height - 2;

    rooms[4].x1 = vsplit1 + 1;
    rooms[4].y1 = hsplit + 1;
    rooms[4].x2 = vsplit2 - 1;
    rooms[4].y2 = map->height - 2;

    rooms[5].x1 = vsplit2 + 1;
    rooms[5].y1 = hsplit + 1;
    rooms[5].x2 = map->width - 2;
    rooms[5].y2 = map->height - 2;

    // ------------------------------------------------------------------
    // 4. Carve floor tiles inside each room
    // ------------------------------------------------------------------
    for (i32 r = 0; r < NUM_ROOMS; r++) {
        for (i32 y = rooms[r].y1; y <= rooms[r].y2; y++) {
            for (i32 x = rooms[r].x1; x <= rooms[r].x2; x++) {
                map->tiles[y][x].type       = TILE_FLOOR;
                map->tiles[y][x].solid      = 0;
                map->tiles[y][x].texture_id = TILE_TEXTURE_FLOOR;
            }
        }
    }

    // ------------------------------------------------------------------
    // 5. Define the seven possible adjacencies between rooms
    //
    //   horiz == false : rooms share a vertical wall column (x = wall)
    //   horiz == true  : rooms share a horizontal wall row  (y = wall)
    // ------------------------------------------------------------------
    struct {
        i32  a, b; // room indices
        i32  wall; // x-position (if !horiz) or y-position (if horiz)
        bool horiz; // which axis the dividing wall runs along
    } adj[7];

    adj[0].a     = 0;
    adj[0].b     = 1;
    adj[0].wall  = vsplit1;
    adj[0].horiz = false;
    adj[1].a     = 1;
    adj[1].b     = 2;
    adj[1].wall  = vsplit2;
    adj[1].horiz = false;
    adj[2].a     = 3;
    adj[2].b     = 4;
    adj[2].wall  = vsplit1;
    adj[2].horiz = false;
    adj[3].a     = 4;
    adj[3].b     = 5;
    adj[3].wall  = vsplit2;
    adj[3].horiz = false;
    adj[4].a     = 0;
    adj[4].b     = 3;
    adj[4].wall  = hsplit;
    adj[4].horiz = true;
    adj[5].a     = 1;
    adj[5].b     = 4;
    adj[5].wall  = hsplit;
    adj[5].horiz = true;
    adj[6].a     = 2;
    adj[6].b     = 5;
    adj[6].wall  = hsplit;
    adj[6].horiz = true;

    // Shuffle adjacency processing order (Fisher-Yates) for a random spanning tree
    i32 order[7];
    for (i32 i = 0; i < 7; i++)
        order[i] = i;
    for (i32 i = 6; i > 0; i--) {
        i32 j    = random_i32_range(0, i);
        i32 tmp  = order[i];
        order[i] = order[j];
        order[j] = tmp;
    }

    // ------------------------------------------------------------------
    // 6. Build a random spanning tree with Kruskal's + union-find,
    //    then optionally punch one extra shortcut corridor
    // ------------------------------------------------------------------
    i32 uf[NUM_ROOMS];
    for (i32 i = 0; i < NUM_ROOMS; i++)
        uf[i] = i;

    Corridor corridors[MAX_CORRIDORS];
    i32      num_corridors = 0;
    bool     in_tree[7];
    for (i32 i = 0; i < 7; i++)
        in_tree[i] = false;

    for (i32 i = 0; i < 7; i++) {
        i32 idx = order[i];
        i32 a   = adj[idx].a;
        i32 b   = adj[idx].b;

        // Find roots
        i32 pa = a;
        while (uf[pa] != pa)
            pa = uf[pa];
        i32 pb = b;
        while (uf[pb] != pb)
            pb = uf[pb];
        if (pa == pb)
            continue; // already connected — skip
        uf[pa]       = pb;

        in_tree[idx] = true;

        // Pick a gap position inside the shared wall segment
        i32 cx, cy;
        if (!adj[idx].horiz) {
            // Vertical divider wall: gap is a single tile in column adj[idx].wall
            i32 y_min = rooms[a].y1 > rooms[b].y1 ? rooms[a].y1 : rooms[b].y1;
            i32 y_max = rooms[a].y2 < rooms[b].y2 ? rooms[a].y2 : rooms[b].y2;
            cx        = adj[idx].wall;
            cy        = random_i32_range(y_min + 1, y_max - 1);
        } else {
            // Horizontal divider wall: gap is a single tile in row adj[idx].wall
            i32 x_min = rooms[a].x1 > rooms[b].x1 ? rooms[a].x1 : rooms[b].x1;
            i32 x_max = rooms[a].x2 < rooms[b].x2 ? rooms[a].x2 : rooms[b].x2;
            cy        = adj[idx].wall;
            cx        = random_i32_range(x_min + 1, x_max - 1);
        }

        // Open the gap tile (floor)
        map->tiles[cy][cx].type         = TILE_FLOOR;
        map->tiles[cy][cx].solid        = 0;
        map->tiles[cy][cx].texture_id   = TILE_TEXTURE_FLOOR;

        corridors[num_corridors].room_a = a;
        corridors[num_corridors].room_b = b;
        corridors[num_corridors].x      = cx;
        corridors[num_corridors].y      = cy;
        corridors[num_corridors].locked = false;
        num_corridors++;
    }

    // Add one extra unlocked shortcut from a non-tree edge (creates an alternative route)
    for (i32 i = 0; i < 7 && num_corridors < MAX_CORRIDORS; i++) {
        i32 idx = order[i];
        if (in_tree[idx])
            continue;

        i32 a = adj[idx].a, b = adj[idx].b;
        i32 cx, cy;
        if (!adj[idx].horiz) {
            i32 y_min = rooms[a].y1 > rooms[b].y1 ? rooms[a].y1 : rooms[b].y1;
            i32 y_max = rooms[a].y2 < rooms[b].y2 ? rooms[a].y2 : rooms[b].y2;
            cx        = adj[idx].wall;
            cy        = random_i32_range(y_min + 1, y_max - 1);
        } else {
            i32 x_min = rooms[a].x1 > rooms[b].x1 ? rooms[a].x1 : rooms[b].x1;
            i32 x_max = rooms[a].x2 < rooms[b].x2 ? rooms[a].x2 : rooms[b].x2;
            cy        = adj[idx].wall;
            cx        = random_i32_range(x_min + 1, x_max - 1);
        }

        map->tiles[cy][cx].type         = TILE_FLOOR;
        map->tiles[cy][cx].solid        = 0;
        map->tiles[cy][cx].texture_id   = TILE_TEXTURE_FLOOR;

        corridors[num_corridors].room_a = a;
        corridors[num_corridors].room_b = b;
        corridors[num_corridors].x      = cx;
        corridors[num_corridors].y      = cy;
        corridors[num_corridors].locked = false;
        num_corridors++;
        break; // one shortcut is enough
    }

    // ------------------------------------------------------------------
    // 7. BFS from room 0 to compute parent relationships and depths
    // ------------------------------------------------------------------
    i32  bfs_parent[NUM_ROOMS];
    i32  bfs_parent_corr[NUM_ROOMS]; // corridor index that leads to each room
    i32  bfs_depth[NUM_ROOMS];
    bool bfs_visited[NUM_ROOMS];
    i32  bfs_queue[NUM_ROOMS];
    i32  bfs_order[NUM_ROOMS];
    i32  bfs_count = 0;

    for (i32 i = 0; i < NUM_ROOMS; i++) {
        bfs_parent[i]      = -1;
        bfs_parent_corr[i] = -1;
        bfs_depth[i]       = 0;
        bfs_visited[i]     = false;
    }

    i32 qhead = 0, qtail = 0;
    bfs_queue[qtail++] = 0;
    bfs_visited[0]     = true;

    while (qhead < qtail) {
        i32 cur                = bfs_queue[qhead++];
        bfs_order[bfs_count++] = cur;

        for (i32 c = 0; c < num_corridors; c++) {
            i32 neighbor = -1;
            if (corridors[c].room_a == cur)
                neighbor = corridors[c].room_b;
            if (corridors[c].room_b == cur)
                neighbor = corridors[c].room_a;
            if (neighbor < 0 || bfs_visited[neighbor])
                continue;

            bfs_visited[neighbor]     = true;
            bfs_parent[neighbor]      = cur;
            bfs_parent_corr[neighbor] = c;
            bfs_depth[neighbor]       = bfs_depth[cur] + 1;
            bfs_queue[qtail++]        = neighbor;
        }
    }

    // ------------------------------------------------------------------
    // 8. Pick the deepest room as the exit
    // ------------------------------------------------------------------
    i32 exit_room = bfs_order[0];
    for (i32 i = 0; i < bfs_count; i++) {
        if (bfs_depth[bfs_order[i]] > bfs_depth[exit_room])
            exit_room = bfs_order[i];
    }

    // ------------------------------------------------------------------
    // 9. Lock 1–3 spanning-tree corridors leading to the deepest rooms.
    //
    //    For each locked corridor, a door is placed at the gap tile and a key
    //    is placed somewhere in the parent room.  Because the key is always in
    //    the room the player visits before the door, the level is guaranteed
    //    to be completable regardless of which path the player takes.
    // ------------------------------------------------------------------
    i32 num_to_lock  = random_i32_range(1, 3);
    i32 locked_count = 0;

    // Walk rooms in reverse BFS order (deepest first), skipping room 0
    for (i32 i = bfs_count - 1; i >= 1 && locked_count < num_to_lock; i--) {
        i32 room     = bfs_order[i];
        i32 corr_idx = bfs_parent_corr[room];

        if (corr_idx < 0 || corridors[corr_idx].locked)
            continue;

        corridors[corr_idx].locked = true;
        locked_count++;

        // riddle door spawn

        if (locked_count == 1) {
            if (random_u8_range(0, 1) == 0) {
                riddle_door_spawn(map, corridors[corr_idx].x, corridors[corr_idx].y);
            } else {
                cipher_door_spawn(map, corridors[corr_idx].x, corridors[corr_idx].y);
            }
        } else {
            // Place a locked door at the corridor gap
            door_spawn(map, corridors[corr_idx].x, corridors[corr_idx].y);
        }
        // Place the matching key somewhere on the floor of the parent room
        MapRoom pr = rooms[bfs_parent[room]];
        for (;;) {
            i32   kx = random_i32_range(pr.x1, pr.x2);
            i32   ky = random_i32_range(pr.y1, pr.y2);
            Tile* t  = map_get_tile(map, kx, ky);
            if (t && t->type == TILE_FLOOR) {
                key_spawn(map, kx, ky);
                break;
            }
        }
    }

    // ------------------------------------------------------------------
    // 10. Place exit tile at the centre of the deepest room
    // ------------------------------------------------------------------
    MapRoom er                      = rooms[exit_room];
    i32     ecx                     = (er.x1 + er.x2) / 2;
    i32     ecy                     = (er.y1 + er.y2) / 2;
    map->tiles[ecy][ecx].type       = TILE_EXIT;
    map->tiles[ecy][ecx].solid      = 0;
    map->tiles[ecy][ecx].texture_id = TILE_TEXTURE_EXIT;

    // ------------------------------------------------------------------
    // 11. Player starts at the centre of room 0
    // ------------------------------------------------------------------
    map->playerStartPos.x = (f32) ((rooms[0].x1 + rooms[0].x2) / 2);
    map->playerStartPos.y = (f32) ((rooms[0].y1 + rooms[0].y2) / 2);
}

// ---------------------------------------------------------------------------
// Puzzle (lever) generation – scattered across floor tiles as optional content
// ---------------------------------------------------------------------------

static void generate_puzzles(Map* map) {
    const i32 lever_count = 3;
    i32       failures    = 0;

    for (i32 i = 0; i < lever_count; i++) {
        i32 x = 0, y = 0;
        i32 tries = 0;

do {
    x = random_i32_range(1, map->width - 2);
    y = random_i32_range(1, map->height - 2);
    tries++;

    bool invalid =
        (i == 0
            ? (map->tiles[y][x].type != TILE_WALL)
            : (map->tiles[y][x].type != TILE_FLOOR))
        || ((i32)map->playerStartPos.x == x && (i32)map->playerStartPos.y == y)
        || tile_adjacent_to(map, x, y, TILE_DOOR);

    if (!invalid)
        break;

} while (tries < 20);

if (tries >= 20) {
    failures++;
    continue;
}

        bool make_hidden = (i == 0);

        if (make_hidden) {
            map->tiles[y][x].type       = TILE_HIDDEN_LEVER;
            map->tiles[y][x].texture_id = TILE_TEXTURE_HIDDEN_LEVER; // temporary
              map->tiles[y][x].solid      = 1;

        } else {
        map->tiles[y][x].type       = TILE_LEVER;
        map->tiles[y][x].texture_id = TILE_TEXTURE_LEVER_OFF;
        map->tiles[y][x].solid      = 1;
        }

        Puzzle* puzzle              = &map->puzzles[i - failures];
        lever_puzzle_init(puzzle, x, y);
        LeverState* state = (LeverState*) puzzle->state;
        state->order      = (i - failures) + 1;
        map->num_puzzles++;
    }
}

bool map_all_levers_active(Map* map) {
    for (i32 i = 0; i < map->num_puzzles; i++) {

        Puzzle* puzzle = &map->puzzles[i];

        if (puzzle->type != PUZZLE_LEVER_TOGGLE)
            continue;

        LeverState* state = (LeverState*) puzzle->state;

        if (!state->activated)
            return false;
    }

    return true;
}
