/**
 * @file core/memory.c
 * @brief Memory management system using arenas for efficient allocation and tracking.
 */
#include "core/memory.h"
#include "core/log.h"

#include <stdlib.h>

MemoryManager* gmem = NULL;

void           mem_init(void) {
    gmem = (MemoryManager*) malloc(sizeof(MemoryManager));

    if (!gmem) {
        LOG_ERROR("Failed to allocate memory for MemoryManager");
        exit(EXIT_FAILURE);
    }

    memset(gmem, 0, sizeof(MemoryManager));

    gmem->arenas[MEM_TAG_PERMANENT] = arena_init(ARENA_PERMANENT_SIZE, ARENA_MAX_BLOCKS, true);
    gmem->arenas[MEM_TAG_LEVEL] = arena_init(ARENA_LEVEL_SIZE, ARENA_MAX_BLOCKS, true);

    // Frame and temp arenas are unmanaged for speed.
    gmem->arenas[MEM_TAG_FRAME] = arena_init(ARENA_FRAME_SIZE, ARENA_MAX_BLOCKS, false);
    gmem->arenas[MEM_TAG_TEMP]  = arena_init(ARENA_TEMP_SIZE, ARENA_MAX_BLOCKS, false);

    for (int i = 0; i < MEM_TAG_COUNT; i++) {
        if (!gmem->arenas[i]) {
            LOG_ERROR("Failed to create memory arenas");
            exit(EXIT_FAILURE);
        }
    }

    log_info_f(
        "Memory system initialized with arenas: permanent=%zu MB, level=%zu MB, frame=%zu MB, "
                  "temp=%zu MB",
        ARENA_PERMANENT_SIZE / (1024 * 1024),
        ARENA_LEVEL_SIZE / (1024 * 1024),
        ARENA_FRAME_SIZE / (1024 * 1024),
        ARENA_TEMP_SIZE / (1024 * 1024));
}

void mem_shutdown(void) {
    if (gmem) {
        for (int i = 0; i < MEM_TAG_COUNT; i++) {
            arena_destroy(gmem->arenas[i]);
        }

        free(gmem);
        gmem = NULL;

        LOG_INFO("Memory system shutdown.");
    }
}

void mem_reset_frame(void) {
    arena_clear(gmem->arenas[MEM_TAG_FRAME]);
    gmem->stats.allocations[MEM_TAG_FRAME] = 0;
    gmem->stats.used[MEM_TAG_FRAME]        = 0;
}

void mem_reset_level(void) {
    arena_clear(gmem->arenas[MEM_TAG_LEVEL]);
    gmem->stats.allocations[MEM_TAG_LEVEL] = 0;
    gmem->stats.used[MEM_TAG_LEVEL]        = 0;
}

void mem_reset_temp(void) {
    arena_clear(gmem->arenas[MEM_TAG_TEMP]);
    gmem->stats.allocations[MEM_TAG_TEMP] = 0;
    gmem->stats.used[MEM_TAG_TEMP]        = 0;
}

void* mem_alloc(MemoryTag tag, u64 size) {
    if (tag >= MEM_TAG_COUNT || tag < 0) {
        log_error_f("alloc: Invalid memory tag: %d", tag);
        return NULL;
    }

    void* ptr = arena_malloc(gmem->arenas[tag], size);

    if (ptr) {
        gmem->stats.allocations[tag]++;
        gmem->stats.used[tag] += size;
        if (gmem->stats.used[tag] > gmem->stats.peak[tag]) {
            gmem->stats.peak[tag] = gmem->stats.used[tag];
        }
    }

    return ptr;
}

void* mem_calloc(MemoryTag tag, u64 size) {
    if (tag >= MEM_TAG_COUNT || tag < 0) {
        log_error_f("calloc: Invalid memory tag: %d", tag);
        return NULL;
    }

    void* ptr = arena_calloc(gmem->arenas[tag], 1, size);

    if (ptr) {
        gmem->stats.allocations[tag]++;
        gmem->stats.used[tag] += size;
        if (gmem->stats.used[tag] > gmem->stats.peak[tag]) {
            gmem->stats.peak[tag] = gmem->stats.used[tag];
        }
    }

    return ptr;
}

void* mem_realloc(MemoryTag tag, void* ptr, u64 new_size) {
    if (tag >= MEM_TAG_COUNT || tag < 0) {
        log_error_f("realloc: Invalid memory tag: %d", tag);
        return NULL;
    }

    void* new_ptr = arena_realloc(gmem->arenas[tag], ptr, new_size);

    if (new_ptr) {
        if (gmem->arenas[tag]->managed) {
            u64 old_size = ptr ? arena_get_block(gmem->arenas[tag], ptr)->size : 0;
            gmem->stats.used[tag] += (new_size - old_size);
        } else {
            // When the arena is unmanaged, realloc does not free the old memory
            gmem->stats.used[tag] += new_size;
        }

        if (gmem->stats.used[tag] > gmem->stats.peak[tag]) {
            gmem->stats.peak[tag] = gmem->stats.used[tag];
        }
    }

    return new_ptr;
}

void mem_free(MemoryTag tag, void* ptr) {
    if (tag >= MEM_TAG_COUNT || tag < 0) {
        log_error_f("free: Invalid memory tag: %d", tag);
        return;
    }

    if (gmem->arenas[tag]->managed) {
        gmem->stats.used[tag] -= arena_get_block(gmem->arenas[tag], ptr)->size;
    }

    gmem->stats.allocations[tag]--;

    arena_free(gmem->arenas[tag], ptr);
}

char* mem_strdup(MemoryTag tag, const char* str) {
    if (tag >= MEM_TAG_COUNT || tag < 0) {
        log_error_f("strdup: Invalid memory tag: %d", tag);
        return NULL;
    }

    size_t len = strlen(str) + 1;
    char*  dup = (char*) mem_alloc(tag, len);
    if (dup) {
        memcpy(dup, str, len);
    }
    return dup;
}
