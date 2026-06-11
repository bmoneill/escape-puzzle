#include "core/memory.h"
#include "core/random.h"

#include "unity.h"

#include <time.h>

void setUp(void) {
    mem_init();

    random_init();
    random_seed((u64) time(NULL));
}

void tearDown(void) {
    random_shutdown();
    mem_shutdown();
}

void test_random_u8(void) {
    for (int i = 0; i < 1000; i++) {
        u8 value = random_u8();
        TEST_ASSERT_TRUE(value >= U8_MIN && value <= U8_MAX);
    }
}

void test_random_u16(void) {
    for (int i = 0; i < 1000; i++) {
        u8 value = random_u8();
        TEST_ASSERT_TRUE(value >= U8_MIN && value <= U8_MAX);
    }
}

void test_random_u32(void) {
    for (int i = 0; i < 1000; i++) {
        u32 value = random_u32();
        TEST_ASSERT_TRUE(value >= U32_MIN && value <= U32_MAX);
    }
}

void test_random_u64(void) {
    for (int i = 0; i < 1000; i++) {
        u64 value = random_u64();
        TEST_ASSERT_TRUE(value >= U64_MIN && value <= U64_MAX);
    }
}

void test_random_i8(void) {
    for (int i = 0; i < 1000; i++) {
        i8 value = random_i8();
        TEST_ASSERT_TRUE(value >= I8_MIN && value <= I8_MAX);
    }
}

void test_random_i16(void) {
    for (int i = 0; i < 1000; i++) {
        i16 value = random_i16();
        TEST_ASSERT_TRUE(value >= I16_MIN && value <= I16_MAX);
    }
}

void test_random_i32(void) {
    for (int i = 0; i < 1000; i++) {
        i32 value = random_i32();
        TEST_ASSERT_TRUE(value >= I32_MIN && value <= I32_MAX);
    }
}

void test_random_i64(void) {
    for (int i = 0; i < 1000; i++) {
        i64 value = random_i64();
        TEST_ASSERT_TRUE(value >= I64_MIN && value <= I64_MAX);
    }
}

void test_random_f32(void) {
    for (int i = 0; i < 1000; i++) {
        f32 value = random_f32();
        TEST_ASSERT_TRUE(value >= 0.0f && value < 1.0f);
    }
}

void test_random_f64(void) {
    for (int i = 0; i < 1000; i++) {
        f64 value = random_f64();
        TEST_ASSERT_TRUE(value >= 0.0 && value < 1.0);
    }
}
