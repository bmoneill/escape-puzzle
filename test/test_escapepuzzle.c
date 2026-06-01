#include "escapepuzzle.h"

#include "unity.h"

void setUp(void) {}

void tearDown(void) {}

void test_escapepuzzle_version(void) {
    TEST_ASSERT_EQUAL_STRING(ESCAPEPUZZLE_VERSION, escapepuzzle_version());
}
