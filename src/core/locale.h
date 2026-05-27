/**
 * @file core/locale.h
 * @brief Localization system for managing localized strings in the game.
 */
#ifndef CORE_LOCALE_H
#define CORE_LOCALE_H

#include "types.h"

#ifndef DEFAULT_LOCALE_DIR
#define DEFAULT_LOCALE_DIR "assets/locale"
#endif

#ifndef LOCALE_NULL_STRING
#define LOCALE_NULL_STRING "(null)"
#endif

/**
 * @brief Represents a single localized string entry, consisting of a key and its corresponding localized value.
 */
typedef struct {
    const char* key;
    const char* value; //!< Localized string value
} LocaleEntry;

/**
 * @brief Represents a collection of localized strings for a specific locale.
 */
typedef struct {
    LocaleEntry* entries; //!< Array of locale entries
    u64          count; //!< Number of entries in the locale
} Locale;

/**
 * @brief Initializes the locale system by loading localized strings from the specified file.
 *
 * The locale file should be in the format of key=value pairs, one per line.
 *
 * If locale_path is NULL, it will default to "LOCALE_PATH/en_us.txt"
 *
 * @param locale_path The path to the locale file to load.
 */
void locale_init(const char* locale_path);

/**
 * @brief Shuts down the locale system, freeing any allocated resources.
 */
void locale_shutdown(void);

/**
 * @brief Retrieves a localized string for the given key and copies it into the provided buffer.
 *
 * If the key is not found, the buffer will contain a default null string defined by LOCALE_NULL_STRING,
 * and the function will return NULL. Otherwise, it returns a pointer to the buffer containing the localized string.
 *
 * @param key The key for the localized string to retrieve.
 * @param buffer The buffer to copy the localized string into.
 * @param buffer_size The size of the buffer.
 *
 * @return pointer to buffer containing the localized string, or NULL if the key is not found.
 */
char* locale_get(const char* key, char* buffer, u64 buffer_size);

// =============================================================================
// Globals
// =============================================================================

extern Locale* glocale; //!< Global locale instance containing loaded localized strings.

#endif
