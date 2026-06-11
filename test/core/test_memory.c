#include "unity.h"

#include "core/memory.h"

#define ALLOCSIZE 64

void setUp(void) { mem_init(); }

void tearDown(void) { mem_shutdown(); }

void test_mem_alloc_permanent(void) {
    void* ptr = mem_alloc(MEM_TAG_PERMANENT, ALLOCSIZE);
    TEST_ASSERT_NOT_NULL(ptr);
    TEST_ASSERT_EQUAL(1, gmem->stats.allocations[MEM_TAG_PERMANENT]);
    TEST_ASSERT_EQUAL(ALLOCSIZE, gmem->stats.used[MEM_TAG_PERMANENT]);
    TEST_ASSERT_EQUAL(ALLOCSIZE, gmem->stats.peak[MEM_TAG_PERMANENT]);
}

void test_mem_alloc_level(void) {
    void* ptr = mem_alloc(MEM_TAG_LEVEL, ALLOCSIZE);
    TEST_ASSERT_NOT_NULL(ptr);
    TEST_ASSERT_EQUAL(1, gmem->stats.allocations[MEM_TAG_LEVEL]);
    TEST_ASSERT_EQUAL(ALLOCSIZE, gmem->stats.used[MEM_TAG_LEVEL]);
    TEST_ASSERT_EQUAL(ALLOCSIZE, gmem->stats.peak[MEM_TAG_LEVEL]);
}

void test_mem_alloc_frame(void) {
    void* ptr = mem_alloc(MEM_TAG_FRAME, ALLOCSIZE);
    TEST_ASSERT_NOT_NULL(ptr);
    TEST_ASSERT_EQUAL(1, gmem->stats.allocations[MEM_TAG_FRAME]);
    TEST_ASSERT_EQUAL(ALLOCSIZE, gmem->stats.used[MEM_TAG_FRAME]);
    TEST_ASSERT_EQUAL(ALLOCSIZE, gmem->stats.peak[MEM_TAG_FRAME]);
}

void test_mem_alloc_temp(void) {
    void* ptr = mem_alloc(MEM_TAG_TEMP, ALLOCSIZE);
    TEST_ASSERT_NOT_NULL(ptr);
    TEST_ASSERT_EQUAL(1, gmem->stats.allocations[MEM_TAG_TEMP]);
    TEST_ASSERT_EQUAL(ALLOCSIZE, gmem->stats.used[MEM_TAG_TEMP]);
    TEST_ASSERT_EQUAL(ALLOCSIZE, gmem->stats.peak[MEM_TAG_TEMP]);
}

void test_memory_reset_frame(void) {
    test_mem_alloc_frame();
    mem_reset_frame();
    TEST_ASSERT_EQUAL(0, gmem->stats.allocations[MEM_TAG_FRAME]);
    TEST_ASSERT_EQUAL(ALLOCSIZE, gmem->stats.peak[MEM_TAG_FRAME]);
}

void test_memory_reset_level(void) {
    test_mem_alloc_level();
    mem_reset_level();
    TEST_ASSERT_EQUAL(0, gmem->stats.allocations[MEM_TAG_LEVEL]);
    TEST_ASSERT_EQUAL(ALLOCSIZE, gmem->stats.peak[MEM_TAG_LEVEL]);
}

void test_memory_reset_temp(void) {
    test_mem_alloc_temp();
    mem_reset_temp();
    TEST_ASSERT_EQUAL(0, gmem->stats.allocations[MEM_TAG_TEMP]);
    TEST_ASSERT_EQUAL(ALLOCSIZE, gmem->stats.peak[MEM_TAG_TEMP]);
}

void test_peak_tracking(void) {
    void* ptr1 = mem_alloc(MEM_TAG_TEMP, ALLOCSIZE);
    void* ptr2 = mem_alloc(MEM_TAG_TEMP, ALLOCSIZE);
    TEST_ASSERT_EQUAL(2, gmem->stats.allocations[MEM_TAG_TEMP]);
    TEST_ASSERT_EQUAL(ALLOCSIZE * 2, gmem->stats.used[MEM_TAG_TEMP]);
    TEST_ASSERT_EQUAL(ALLOCSIZE * 2, gmem->stats.peak[MEM_TAG_TEMP]);

    mem_free(MEM_TAG_TEMP, ptr2);
    TEST_ASSERT_EQUAL(ALLOCSIZE * 2, gmem->stats.peak[MEM_TAG_TEMP]);
}
