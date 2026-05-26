/**
 * @file core/memory.h
 * @brief Memory management system using arenas for efficient allocation and tracking.
 */
#ifndef CORE_MEMORY_H
#define CORE_MEMORY_H

#include "arena.h"
#include "types.h"

// TODO Late in implementation, consider whether these arena sizes can be shrunk
#define ARENA_PERMANENT_SIZE (64 * 1024 * 1024) // 64 MB - core systems
#define ARENA_LEVEL_SIZE     (64 * 1024 * 1024) // 64 MB - game state
#define ARENA_FRAME_SIZE     (8 * 1024 * 1024) // 8 MB - per-frame temp data
#define ARENA_TEMP_SIZE      (4 * 1024 * 1024) // 4 MB - short-lived allocations

#define ARENA_MAX_BLOCKS 1024

/**
 * @brief MemoryTag is used to categorize allocations for tracking and debugging purposes.
 */
typedef enum {
    MEM_TAG_PERMANENT, //!< Memory that persists for the entire application lifetime (e.g. global state, config).
    MEM_TAG_LEVEL, //!< Memory that persists for the duration of a level or scene (e.g. level data, entities).
    MEM_TAG_FRAME, //!< Memory that is allocated and freed within a single frame (e.g. temporary buffers for rendering).
    MEM_TAG_TEMP, //!< Memory that is allocated and freed within a single function or short scope (e.g. temporary strings).
    MEM_TAG_COUNT, //!< Total number of memory tags (used for array sizing)
} MemoryTag;

/**
 * @brief Memory system state
 */
typedef struct {
    Arena* arenas[MEM_TAG_COUNT]; //!< Array of arenas for each memory tag
    struct {
        u64 allocations[MEM_TAG_COUNT]; //!< Total number of allocations made for each tag
        u64 used[MEM_TAG_COUNT]; //!< Current memory usage in bytes for each tag
        u64 peak[MEM_TAG_COUNT]; //!< Peak memory usage in bytes for each tag
    } stats;
} MemoryManager;

/**
 * @brief Initializes the memory system.
 */
void mem_init(void);

/**
 * @brief Shuts down the memory system.
 */
void mem_shutdown(void);

// =============================================================================
// Arena management and allocation functions
// =============================================================================

/**
 * @brief Resets the frame arena, clearing all allocations.
 *
 * This updates allocations and used stats, but not peak.
 */
void mem_reset_frame(void);

/**
 * @brief Resets the level arena, clearing all allocations.
 *
 * This updates allocations and used stats, but not peak.
 */
void mem_reset_level(void);

/**
 * @brief Resets the temp arena, clearing all allocations.
 *
 * This updates allocations and used stats, but not peak.
 */
void mem_reset_temp(void);

/**
 * @brief Allocates memory from the specified arena.
 * @param tag The memory tag.
 * @param size The size of the allocation in bytes.
 * @return A pointer to the allocated memory, or NULL on failure.
 */
void* mem_alloc(MemoryTag tag, u64 size);

/**
 * @brief Allocates zero-initialized memory from the specified arena.
 * @param tag The memory tag.
 * @param size The size of the allocation in bytes.
 * @return A pointer to the allocated memory, or NULL on failure.
 */
void* mem_calloc(MemoryTag tag, u64 size);

/**
 * @brief Reallocates memory from the specified arena, resizing the allocation.
 *
 * @param tag The memory tag.
 * @param ptr The pointer to the existing memory allocation, or NULL to allocate new memory.
 * @param new_size The new size of the allocation in bytes.
 *
 * @return A pointer to the reallocated memory, or NULL on failure.
 */
void* mem_realloc(MemoryTag tag, void* ptr, u64 new_size);

/**
 * @brief Frees memory back to the specified arena.
 *
 * This updates allocations and used stats.
 *
 * @param tag The memory tag.
 * @param ptr The pointer to the memory to free.
 */
void mem_free(MemoryTag tag, void* ptr);

/**
 * @brief Duplicates a string into memory allocated from the specified arena.
 * @param tag The memory tag.
 * @param str The null-terminated string to duplicate.
 * @return A pointer to the duplicated string, or NULL on failure.
 */
char* mem_strdup(MemoryTag tag, const char* str);

// =============================================================================
// Convenience macros
// =============================================================================

#define MEM_PERM(size)       mem_alloc(MEM_TAG_PERMANENT, size)
#define MEM_PERM_STRDUP(str) mem_strdup(MEM_TAG_PERMANENT, str)

#define MEM_LEVEL(size)       mem_alloc(MEM_TAG_LEVEL, size)
#define MEM_LEVEL_STRDUP(str) mem_strdup(MEM_TAG_LEVEL, str)

#define MEM_FRAME(size)       mem_alloc(MEM_TAG_FRAME, size)
#define MEM_FRAME_STRDUP(str) mem_strdup(MEM_TAG_FRAME, str)

#define MEM_TEMP(size)       mem_alloc(MEM_TAG_TEMP, size)
#define MEM_TEMP_STRDUP(str) mem_strdup(MEM_TAG_TEMP, str)

// =============================================================================
// Globals
// =============================================================================

extern MemoryManager* gmem; //!< Global memory manager instance

#endif
