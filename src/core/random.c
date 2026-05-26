/**
 * @file core/random.c
 *
 * @brief Random number generation functions for various data types, using the C standard library's rand() function.
 */
#include "random.h"

#include "log.h"
#include "memory.h"

#include <stdlib.h>

u64* seed_ptr = NULL;

void random_init(void) {
    if (seed_ptr) {
        LOG_WARNING("Random generator already initialized.");
        return;
    }

    seed_ptr = MEM_PERM(sizeof(u64));
    random_seed(12345);
}

void random_shutdown(void) {
    if (!seed_ptr) {
        LOG_WARNING("Random generator not initialized.");
        return;
    }

    seed_ptr = NULL;
}

void random_seed(u64 seed) {
    if (!seed_ptr) {
        LOG_WARNING("Random generator not initialized.");
        return;
    }

    *seed_ptr = seed;
    srand((unsigned int) seed);
}

u8  random_u8(void) { return (u8) (rand() % U8_MAX) + U8_MIN; }

u16 random_u16(void) { return (u16) (rand() % U16_MAX) + U16_MIN; }

u32 random_u32(void) {
    // RAND_MAX may be as low as 32767, so we need to combine two calls to rand() to get a full 32-bit value.
    u16 high = (u16) (rand() % U16_MAX);
    u16 low  = (u16) (rand() % U16_MAX);
    return (((u32) high) << 16) | low;
}

u64 random_u64(void) {
    // Combine four calls to rand() to get a full 64-bit value.
    u32 high = random_u32();
    u32 low  = random_u32();
    return (((u64) high) << 32) | low;
}

i8  random_i8(void) { return (i8) (rand() % (I8_MAX)) + I8_MIN; }

i16 random_i16(void) { return (i16) (rand() % (I16_MAX)) + I16_MIN; }

i32 random_i32(void) {
    i16 high = (i16) random_i16();
    i16 low  = (i16) random_i16();
    return (((i32) high) << 16) | low;
}

i64 random_i64(void) {
    i32 high = random_i32();
    i32 low  = random_i32();
    return (((i64) high) << 32) | low;
}

f32 random_f32(void) { return (f32) rand() / (f32) RAND_MAX; }

f64 random_f64(void) { return (f64) rand() / (f64) RAND_MAX; }
