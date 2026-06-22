#include "strings.h"

#include <ctype.h>

void s_tolower(char* s) {
    while (*s != '\0') {
        *s = tolower(*s);
        s++;
    }
}
