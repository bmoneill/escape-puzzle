/**
 * @file core/log.c
 * @brief Logging functions.
 */
#include "core/log.h"
#include "core/config.h"

#include <stdarg.h>
#include <stdio.h>

EMSCRIPTEN_KEEPALIVE void log_error_f(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "[ERROR] ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}

EMSCRIPTEN_KEEPALIVE void log_info_f(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stdout, "[INFO] ");
    vfprintf(stdout, fmt, args);
    fprintf(stdout, "\n");
    va_end(args);
}

EMSCRIPTEN_KEEPALIVE void log_warning_f(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "[WARNING] ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}
