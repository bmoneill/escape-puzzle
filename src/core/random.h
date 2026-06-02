/**
 * @file core/random.h
 * @brief Random number generation functions for various data types, using the C standard library's rand() function.
 */
#ifndef CORE_RANDOM_H
#define CORE_RANDOM_H

#include "types.h"

// =============================================================================
// Random generator state and lifecycle functions
// =============================================================================

/**
 * @brief Initialize the random number generator.
 *
 * This uses a preset seed.
 */
void random_init(void);

/**
 * @brief Shutdown the random number generator
 *
 * This function simply sets the seed_ptr to NULL, as we are
 * using permanent memory here.
 */
void random_shutdown(void);

/**
 * @brief Set the seed for the random number generator.
 *
 * @param seed The seed value to use for random number generation.
 */
void random_seed(u64 seed);

// =============================================================================
// Random value generation functions
// =============================================================================

/**
 * @brief Get a random u8 value.
 *
 * @return A random u8 value in the range [U8_MIN, U8_MAX].
 */
u8 random_u8(void);

/**
 * @brief Get a random u8 value in the specified range.
 *
 * @param min The minimum value (inclusive).
 * @param max The maximum value (inclusive).
 * @return A random u8 value in the range [min, max].
 */
u8 random_u8_range(u8 min, u8 max);

/**
 * @brief Get a random u16 value.
 *
 * @return A random u16 value in the range [U16_MIN, U16_MAX].
 */
u16 random_u16(void);

/**
 * @brief Get a random u16 value in the specified range.
 *
 * @param min The minimum value (inclusive).
 * @param max The maximum value (inclusive).
 * @return A random u16 value in the range [min, max].
 */
u16 random_u16_range(u16 min, u16 max);

/**
 * @brief Get a random u32 value.
 *
 * @return A random u32 value in the range [U32_MIN, U32_MAX].
 */
u32 random_u32(void);

/**
 * @brief Get a random u32 value in the specified range.
 *
 * @param min The minimum value (inclusive).
 * @param max The maximum value (inclusive).
 * @return A random u32 value in the range [min, max].
 */
u32 random_u32_range(u32 min, u32 max);

/**
 * @brief Get a random u64 value.
 *
 * @return A random u64 value in the range [U64_MIN, U64_MAX].
 */
u64 random_u64(void);

/**
 * @brief Get a random u64 value in the specified range.
 *
 * @param min The minimum value (inclusive).
 * @param max The maximum value (inclusive).
 * @return A random u64 value in the range [min, max].
 */
u64 random_u64_range(u64 min, u64 max);

/**
 * @brief Get a random i8 value.
 *
 * @return A random i8 value in the range [I8_MIN, I8_MAX].
 */
i8 random_i8(void);

/**
 * @brief Get a random i8 value in the specified range.
 *
 * @param min The minimum value (inclusive).
 * @param max The maximum value (inclusive).
 * @return A random i8 value in the range [min, max].
 */
i8 random_i8_range(i8 min, i8 max);

/**
 * @brief Get a random i16 value.
 *
 * @return A random i16 value in the range [I16_MIN, I16_MAX].
 */
i16 random_i16(void);

/**
 * @brief Get a random i16 value in the specified range.
 *
 * @param min The minimum value (inclusive).
 * @param max The maximum value (inclusive).
 * @return A random i16 value in the range [min, max].
 */
i16 random_i16_range(i16 min, i16 max);

/**
 * @brief Get a random i32 value.
 *
 * @return A random i32 value in the range [I32_MIN, I32_MAX].
 */
i32 random_i32(void);

/**
 * @brief Get a random i32 value in the specified range.
 *
 * @param min The minimum value (inclusive).
 * @param max The maximum value (inclusive).
 * @return A random i32 value in the range [min, max].
 */
i32 random_i32_range(i32 min, i32 max);

/**
 * @brief Get a random i64 value.
 *
 * @return A random i64 value in the range [I64_MIN, I64_MAX].
 */
i64 random_i64(void);

/**
 * @brief Get a random i64 value in the specified range.
 *
 * @param min The minimum value (inclusive).
 * @param max The maximum value (inclusive).
 * @return A random i64 value in the range [min, max].
 */
i64 random_i64_range(i64 min, i64 max);

/**
 * @brief Get a random f32 value.
 *
 * @return A random f32 value in the range [0.0, 1.0).
 */
f32 random_f32(void);

/**
 * @brief Get a random f32 value in the specified range.
 *
 * @param min The minimum value (inclusive).
 * @param max The maximum value (inclusive).
 * @return A random f32 value in the range [min, max].
 */
f32 random_f32_range(f32 min, f32 max);

/**
 * @brief Get a random f64 value.
 *
 * @return A random f64 value in the range [0.0, 1.0).
 */
f64 random_f64(void);

/**
 * @brief Get a random f64 value in the specified range.
 *
 * @param min The minimum value (inclusive).
 * @param max The maximum value (inclusive).
 * @return A random f64 value in the range [min, max].
 */
f64 random_f64_range(f64 min, f64 max);

#endif
