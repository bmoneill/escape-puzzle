/**
 * @file core/ht.h
 * @brief Hash table implementation for mapping 64-bit keys to void* values.
 */
#ifndef CORE_HT_H
#define CORE_HT_H

#include "memory.h"
#include "types.h"

// Special marker values for hash table entries
#define HT_EMPTY     0
#define HT_TOMBSTONE 1

/**
 * @brief A single entry in the hash table, consisting of a 64-bit key and a void* value.
 */
typedef struct {
    u64   key; //!< The key for this entry (HT_EMPTY or HT_TOMBSTONE if slot is empty or deleted)
    void* value; //!< The value associated with the key, or NULL if slot is empty or deleted
} HashEntry;

/**
 * @brief A hash table implementation that maps 64-bit keys to void* values.
 *
 * Uses linear probing for collision resolution and automatically resizes when
 * load factor exceeds 0.75. Supports deletion via tombstone markers.
 *
 * Note: Keys HT_EMPTY (0) and HT_TOMBSTONE (1) are reserved and cannot be used.
 */
typedef struct {
    MemoryTag  tag; //!< Memory tag for allocations
    HashEntry* entries; //!< Array of hash entries (size = capacity)
    u64        capacity; //!< Total capacity of the entries array (always a power of 2)
    u64        length; // Number of active entries (excludes tombstones)
    u64        tombstones; // Number of tombstone entries
} HashTable;

/**
 * @brief Initialize a hash table with the given capacity
 *
 * @param tag Memory tag for allocations
 * @param capacity Initial capacity (will be rounded up to next power of 2)
 * @return Pointer to initialized hash table, or NULL on failure
 */
HashTable* ht_init(MemoryTag tag, u64 capacity);

/**
 * @brief Deinitialize and free a hash table
 *
 * @param ht The hash table to deinitialize
 * @return true on success, false on failure
 */
bool ht_deinit(HashTable* ht);

/**
 * @brief Get a value from the hash table
 *
 * @param ht The hash table
 * @param key The key to look up (cannot be HT_EMPTY or HT_TOMBSTONE)
 * @return The value associated with the key, or NULL if not found
 */
void* ht_get(HashTable* ht, u64 key);

/**
 * @brief Set a key-value pair in the hash table
 *
 * If the key already exists, updates the value. Otherwise inserts a new entry.
 * Automatically resizes the table if load factor exceeds 0.75.
 *
 * @param ht The hash table
 * @param key The key to set (cannot be HT_EMPTY or HT_TOMBSTONE)
 * @param value The value to associate with the key
 * @return The value that was set, or NULL on failure
 */
void* ht_set(HashTable* ht, u64 key, void* value);

/**
 * @brief Get the next key-value pair in the hash table for iteration
 *
 * This function can be used to iterate over all key-value pairs in the hash table.
 *
 * @param ht The hash table
 * @param iter The current iterator position (pass NULL to start iteration)
 * @param found The number of entries found so far (pass 0 to start iteration)
 */
HashEntry* ht_next(HashTable* ht, HashEntry* iter, u64* found);

/**
 * @brief Remove a key-value pair from the hash table
 *
 * @param ht The hash table
 * @param key The key to remove
 * @return The value that was removed, or NULL if key was not found
 */
void* ht_remove(HashTable* ht, u64 key);

#endif
