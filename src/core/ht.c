/**
 * @file core/ht.c
 * @brief Hash table implementation for mapping 64-bit keys to void* values.
 */
#include "ht.h"

#include "core/config.h"
#include "core/log.h"

// =============================================================================
// Constants
// =============================================================================

#define HT_LOAD_FACTOR_THRESHOLD 0.75
#define HT_MIN_CAPACITY          8

// =============================================================================
// Helper functions
// =============================================================================

/**
 * @brief Hash function for 64-bit integers
 *
 * Uses a mixing function to ensure good distribution even for sequential keys.
 * Based on MurmurHash3's finalizer.
 *
 * @param key The key to hash
 * @return The hashed value
 */
EMSCRIPTEN_KEEPALIVE static inline u64 ht_hash(u64 key) {
    key ^= key >> 33;
    key *= 0xff51afd7ed558ccdULL;
    key ^= key >> 33;
    key *= 0xc4ceb9fe1a85ec53ULL;
    key ^= key >> 33;
    return key;
}

/**
 * @brief Round up to the next power of 2
 *
 * @param n The number to round up
 * @return The next power of 2 >= n
 */
EMSCRIPTEN_KEEPALIVE static inline u64 ht_next_power_of_2(u64 n) {
    if (n == 0)
        return 1;
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    return n + 1;
}

/**
 * @brief Rehash all entries into a new table with given capacity
 *
 * @param ht The hash table to rehash
 * @param new_capacity The new capacity for the table
 * @return true on success, false on failure
 */
EMSCRIPTEN_KEEPALIVE static bool ht_rehash(HashTable* ht, u64 new_capacity) {
    if (!ht) {
        return false;
    }

    // Allocate new entries array
    HashEntry* new_entries = (HashEntry*) mem_calloc(ht->tag, sizeof(HashEntry) * new_capacity);
    if (!new_entries) {
        LOG_ERROR("Failed to allocate memory for hash table rehashing.");
        return false;
    }

    // Save old entries
    HashEntry* old_entries  = ht->entries;
    u64        old_capacity = ht->capacity;

    // Update table with new entries
    ht->entries    = new_entries;
    ht->capacity   = new_capacity;
    ht->length     = 0;
    ht->tombstones = 0;

    // Rehash all old entries into new table
    for (u64 i = 0; i < old_capacity; i++) {
        if (old_entries[i].key != HT_EMPTY && old_entries[i].key != HT_TOMBSTONE) {
            // Find slot in new table
            u64 hash = ht_hash(old_entries[i].key);
            u64 idx  = hash & (new_capacity - 1); // Fast modulo for power of 2

            for (u64 j = 0; j < new_capacity; j++) {
                u64 probe_idx = (idx + j) & (new_capacity - 1);
                if (new_entries[probe_idx].key == HT_EMPTY) {
                    new_entries[probe_idx].key   = old_entries[i].key;
                    new_entries[probe_idx].value = old_entries[i].value;
                    ht->length++;
                    break;
                }
            }
        }
    }

    // Free old entries
    mem_free(ht->tag, old_entries);

    return true;
}

/**
 * @brief Check if the table needs resizing and resize if necessary
 *
 * Resizes when load factor (including tombstones) exceeds threshold.
 *
 * @param ht The hash table to check
 * @return true if no resize needed or resize succeeded, false on resize failure
 */
EMSCRIPTEN_KEEPALIVE static bool ht_check_resize(HashTable* ht) {
    if (!ht) {
        return false;
    }

    double load_factor = (double) (ht->length + ht->tombstones) / (double) ht->capacity;

    if (load_factor > HT_LOAD_FACTOR_THRESHOLD) {
        u64 new_capacity = ht->capacity * 2;
        log_info_f("Hash table load factor %.2f exceeds threshold %.2f, resizing from %llu to %llu",
                   load_factor,
                   HT_LOAD_FACTOR_THRESHOLD,
                   ht->capacity,
                   new_capacity);
        return ht_rehash(ht, new_capacity);
    }

    return true;
}

// =============================================================================
// Public functions
// =============================================================================

HashTable* ht_init(MemoryTag tag, u64 capacity) {
    HashTable* ht = (HashTable*) mem_alloc(tag, sizeof(HashTable));
    if (!ht) {
        LOG_ERROR("Failed to allocate memory for hash table.");
        return NULL;
    }

    // Ensure capacity is at least minimum and a power of 2
    if (capacity < HT_MIN_CAPACITY) {
        capacity = HT_MIN_CAPACITY;
    }
    capacity       = ht_next_power_of_2(capacity);

    ht->tag        = tag;
    ht->capacity   = capacity;
    ht->length     = 0;
    ht->tombstones = 0;
    ht->entries    = (HashEntry*) mem_calloc(tag, sizeof(HashEntry) * capacity);

    if (!ht->entries) {
        LOG_ERROR("Failed to allocate memory for hash table entries.");
        mem_free(tag, ht);
        return NULL;
    }

    return ht;
}

EMSCRIPTEN_KEEPALIVE bool ht_deinit(HashTable* ht) {
    if (!ht) {
        LOG_ERROR("Cannot deinitialize a NULL hash table.");
        return false;
    }

    mem_free(ht->tag, ht->entries);
    mem_free(ht->tag, ht);
    return true;
}

EMSCRIPTEN_KEEPALIVE void* ht_get(HashTable* ht, u64 key) {
    if (!ht) {
        LOG_ERROR("Cannot get from a NULL hash table.");
        return NULL;
    }

    if (key == HT_EMPTY || key == HT_TOMBSTONE) {
        LOG_ERROR("Cannot use reserved key values HT_EMPTY or HT_TOMBSTONE.");
        return NULL;
    }

    u64 hash = ht_hash(key);
    u64 idx  = hash & (ht->capacity - 1); // Fast modulo for power of 2

    for (u64 i = 0; i < ht->capacity; i++) {
        u64 probe_idx = (idx + i) & (ht->capacity - 1);

        if (ht->entries[probe_idx].key == key) {
            return ht->entries[probe_idx].value;
        }

        // Stop probing on empty slot (not tombstone, as key could be after tombstone)
        if (ht->entries[probe_idx].key == HT_EMPTY) {
            return NULL;
        }
    }

    return NULL; // Not found after probing entire table
}

HashEntry* ht_next(HashTable* ht, HashEntry* iter, u64* found) {
    if (!ht) {
        LOG_ERROR("Cannot iterate with a NULL hash table.");
        return NULL;
    }

    if (*found == ht->length) {
        return NULL; // All entries have been found
    }

    u64 idx = 0;

    if (iter) {
        idx = (iter - ht->entries) + 1; // Move to next index
    }

    while (idx < ht->capacity) {
        if (ht->entries[idx].key != HT_EMPTY && ht->entries[idx].key != HT_TOMBSTONE) {
            (*found)++;
            return &ht->entries[idx];
        }
        idx++;
    }

    return NULL; // End of table
}

EMSCRIPTEN_KEEPALIVE void* ht_set(HashTable* ht, u64 key, void* value) {
    if (!ht) {
        LOG_ERROR("Cannot set in a NULL hash table.");
        return NULL;
    }

    if (key == HT_EMPTY || key == HT_TOMBSTONE) {
        LOG_ERROR("Cannot use reserved key values HT_EMPTY or HT_TOMBSTONE.");
        return NULL;
    }

    // Check if we need to resize before insertion
    if (!ht_check_resize(ht)) {
        LOG_ERROR("Failed to resize hash table.");
        return NULL;
    }

    u64 hash            = ht_hash(key);
    u64 idx             = hash & (ht->capacity - 1); // Fast modulo for power of 2
    u64 first_tombstone = ht->capacity; // Track first tombstone for reuse

    for (u64 i = 0; i < ht->capacity; i++) {
        u64 probe_idx = (idx + i) & (ht->capacity - 1);

        // Key already exists - update value
        if (ht->entries[probe_idx].key == key) {
            ht->entries[probe_idx].value = value;
            return value;
        }

        // Track first tombstone we encounter
        if (ht->entries[probe_idx].key == HT_TOMBSTONE && first_tombstone == ht->capacity) {
            first_tombstone = probe_idx;
        }

        // Found empty slot
        if (ht->entries[probe_idx].key == HT_EMPTY) {
            // If we saw a tombstone earlier, reuse it
            if (first_tombstone != ht->capacity) {
                ht->entries[first_tombstone].key   = key;
                ht->entries[first_tombstone].value = value;
                ht->tombstones--;
                ht->length++;
                return value;
            }

            // Otherwise use this empty slot
            ht->entries[probe_idx].key   = key;
            ht->entries[probe_idx].value = value;
            ht->length++;
            return value;
        }
    }

    // Should never reach here if resize logic works correctly
    LOG_ERROR("Hash table is full even after resize attempt.");
    return NULL;
}

EMSCRIPTEN_KEEPALIVE void* ht_remove(HashTable* ht, u64 key) {
    if (!ht) {
        LOG_ERROR("Cannot remove from a NULL hash table.");
        return NULL;
    }

    if (key == HT_EMPTY || key == HT_TOMBSTONE) {
        LOG_ERROR("Cannot use reserved key values HT_EMPTY or HT_TOMBSTONE.");
        return NULL;
    }

    u64 hash = ht_hash(key);
    u64 idx  = hash & (ht->capacity - 1); // Fast modulo for power of 2

    for (u64 i = 0; i < ht->capacity; i++) {
        u64 probe_idx = (idx + i) & (ht->capacity - 1);

        if (ht->entries[probe_idx].key == key) {
            void* value = ht->entries[probe_idx].value;

            // Mark as tombstone to preserve probe chains
            ht->entries[probe_idx].key   = HT_TOMBSTONE;
            ht->entries[probe_idx].value = NULL;
            ht->length--;
            ht->tombstones++;

            return value;
        }

        // Stop probing on empty slot
        if (ht->entries[probe_idx].key == HT_EMPTY) {
            return NULL;
        }
    }

    return NULL; // Not found
}
