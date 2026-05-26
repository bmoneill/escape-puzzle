/**
 * @file core/log.c
 * @brief Logging functions.
 */
#include "log.h"

#include <stdarg.h>
#include <stdio.h>

void log_error_f(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "[ERROR] ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}

void log_info_f(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stdout, "[INFO] ");
    vfprintf(stdout, fmt, args);
    fprintf(stdout, "\n");
    va_end(args);
}

void log_warning_f(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "[WARNING] ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}
