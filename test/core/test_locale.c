#include "core/locale.h"

#include "core/memory.h"

#include "unity.h"

#include "../util.c"

void setUp(void) {
    mem_init();
    locale_init(get_path("locale/en_us.txt"));
}
void tearDown(void) {
    locale_shutdown();
    mem_shutdown();
}

void test_locale_get(void) {
    char buf[64];
    locale_get("MAIN_MENU_START", buf, 64);
    TEST_ASSERT_EQUAL_STRING("Start Game", buf);
}
