/**
 * @file core/log.h
 * @brief Logging function declarations.
 */
#ifndef CORE_LOG_H
#define CORE_LOG_H

/**
 * @brief Logs an error message with the function name as a prefix.
 */
#define LOG_ERROR(s)                                                                               \
    do {                                                                                           \
        log_error_f("%s: " s, __func__);                                                           \
    } while (0)

/**
 * @brief Logs an informational message with the function name as a prefix.
 */
#define LOG_INFO(s)                                                                                \
    do {                                                                                           \
        log_info_f("%s: " s, __func__);                                                            \
    } while (0)

/**
 * @brief Logs a warning message with the function name as a prefix.
 */
#define LOG_WARNING(s)                                                                             \
    do {                                                                                           \
        log_warning_f("%s: " s, __func__);                                                         \
    } while (0)

/**
 * @brief Logs a formatted error message to the console.
 * @param fmt The format string
 * @param ... Additional arguments for the format string
 */
void log_error_f(const char* fmt, ...);

/**
 * @brief Logs a formatted informational message to the console.
 * @param fmt The format string
 * @param ... Additional arguments for the format string
 */
void log_info_f(const char* fmt, ...);

/**
 * @brief Logs a formatted warning message to the console.
 * @param fmt The format string
 * @param ... Additional arguments for the format string
 */
void log_warning_f(const char* fmt, ...);

#endif
