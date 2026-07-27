/**
 * @file core/random.c
 *
 * @brief Random number generation functions for various data types, using the C standard library's rand() function.
 */
#include "core/random.h"
#include "core/config.h"
#include "core/log.h"
#include "core/memory.h"

#include <stdlib.h>

u64*                      seed_ptr = NULL;

EMSCRIPTEN_KEEPALIVE void random_init(void) {
    if (seed_ptr) {
        LOG_WARNING("Random generator already initialized.");
        return;
    }

    seed_ptr = MEM_PERM(sizeof(u64));
    random_seed(12345);
}

EMSCRIPTEN_KEEPALIVE void random_shutdown(void) {
    if (!seed_ptr) {
        LOG_WARNING("Random generator not initialized.");
        return;
    }

    seed_ptr = NULL;
}

EMSCRIPTEN_KEEPALIVE void random_seed(u64 seed) {
    if (!seed_ptr) {
        LOG_WARNING("Random generator not initialized.");
        return;
    }

    *seed_ptr = seed;
    srand((unsigned int) seed);
}

EMSCRIPTEN_KEEPALIVE u8 random_u8(void) { return (u8) (rand() % U8_MAX) + U8_MIN; }

EMSCRIPTEN_KEEPALIVE u8 random_u8_range(u8 min, u8 max) {
    if (min > max) {
        log_error_f("Invalid range: min (%u) is greater than max (%u)", min, max);
        return 0;
    }
    return min + (u8) (random_u8() % ((u8) (max - min) + 1));
}

EMSCRIPTEN_KEEPALIVE u16 random_u16(void) { return (u16) (rand() % U16_MAX) + U16_MIN; }

EMSCRIPTEN_KEEPALIVE u16 random_u16_range(u16 min, u16 max) {
    if (min > max) {
        log_error_f("Invalid range: min (%u) is greater than max (%u)", min, max);
        return 0;
    }
    return min + (u16) (random_u16() % ((u16) (max - min) + 1));
}

EMSCRIPTEN_KEEPALIVE u32 random_u32(void) {
    // RAND_MAX may be as low as 32767, so we need to combine two calls to rand() to get a full 32-bit value.
    u16 high = (u16) (rand() % U16_MAX);
    u16 low  = (u16) (rand() % U16_MAX);
    return (((u32) high) << 16) | low;
}

EMSCRIPTEN_KEEPALIVE u32 random_u32_range(u32 min, u32 max) {
    if (min > max) {
        log_error_f("Invalid range: min (%u) is greater than max (%u)", min, max);
        return 0;
    }
    return min + (random_u32() % ((u32) (max - min) + 1));
}

EMSCRIPTEN_KEEPALIVE u64 random_u64(void) {
    // Combine four calls to rand() to get a full 64-bit value.
    u32 high = random_u32();
    u32 low  = random_u32();
    return (((u64) high) << 32) | low;
}

EMSCRIPTEN_KEEPALIVE u64 random_u64_range(u64 min, u64 max) {
    if (min > max) {
        log_error_f("Invalid range: min (%llu) is greater than max (%llu)", min, max);
        return 0;
    }
    return min + (random_u64() % ((u64) (max - min) + 1));
}

EMSCRIPTEN_KEEPALIVE i8 random_i8(void) { return (i8) (rand() % (I8_MAX * 2 + 2)) + I8_MIN; }

EMSCRIPTEN_KEEPALIVE i8 random_i8_range(i8 min, i8 max) {
    if (min > max) {
        log_error_f("Invalid range: min (%d) is greater than max (%d)", min, max);
        return 0;
    }
    return min + (i8) (random_u8() % ((u8) (max - min) + 1));
}

EMSCRIPTEN_KEEPALIVE i16 random_i16(void) { return (i16) (rand() % (I16_MAX * 2 + 2)) + I16_MIN; }

EMSCRIPTEN_KEEPALIVE i16 random_i16_range(i16 min, i16 max) {
    if (min > max) {
        log_error_f("Invalid range: min (%d) is greater than max (%d)", min, max);
        return 0;
    }
    return min + (i16) (random_u16() % ((u16) (max - min) + 1));
}

EMSCRIPTEN_KEEPALIVE i32 random_i32(void) {
    i16 high = (i16) random_i16();
    i16 low  = (i16) random_i16();
    return (((i32) high) << 16) | low;
}

EMSCRIPTEN_KEEPALIVE i32 random_i32_range(i32 min, i32 max) {
    if (min > max) {
        log_error_f("Invalid range: min (%d) is greater than max (%d)", min, max);
        return 0;
    }
    return min + (i32) (random_u32() % ((u32) (max - min) + 1));
}

EMSCRIPTEN_KEEPALIVE i64 random_i64(void) {
    i32 high = random_i32();
    i32 low  = random_i32();
    return (((i64) high) << 32) | low;
}

EMSCRIPTEN_KEEPALIVE i64 random_i64_range(i64 min, i64 max) {
    if (min > max) {
        log_error_f("Invalid range: min (%lld) is greater than max (%lld)", min, max);
        return 0;
    }
    return min + (i64) (random_u64() % ((u64) (max - min) + 1));
}

EMSCRIPTEN_KEEPALIVE f32 random_f32(void) { return (f32) rand() / (f32) RAND_MAX; }

EMSCRIPTEN_KEEPALIVE f32 random_f32_range(f32 min, f32 max) {
    if (min > max) {
        log_error_f("Invalid range: min (%f) is greater than max (%f)", min, max);
        return 0.0f;
    }
    return random_f32() * (max - min) + min;
}

EMSCRIPTEN_KEEPALIVE f64 random_f64(void) { return (f64) rand() / (f64) RAND_MAX; }

EMSCRIPTEN_KEEPALIVE f64 random_f64_range(f64 min, f64 max) {
    if (min > max) {
        log_error_f("Invalid range: min (%lf) is greater than max (%lf)", min, max);
        return 0.0;
    }
    return random_f64() * (max - min) + min;
}
