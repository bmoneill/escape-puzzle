#include "strings.h"

#include "core/config.h"

#include <ctype.h>

EMSCRIPTEN_KEEPALIVE void s_tolower(char* s) {
    while (*s != '\0') {
        *s = tolower(*s);
        s++;
    }
}
