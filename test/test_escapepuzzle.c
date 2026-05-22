#include "escapepuzzle.h"

#include "unity.h"

void setUp(void) {}

void tearDown(void) {}

void test_start_game(void) { TEST_ASSERT_EQUAL_INT(0, start_game()); }

void test_escapepuzzle_version(void) {
    TEST_ASSERT_EQUAL_STRING(ESCAPEPUZZLE_VERSION, escapepuzzle_version());
}
