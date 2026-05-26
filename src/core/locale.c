/**
 * @file core/locale.c
 * @brief Localization system for managing localized strings in the game.
 */
#include "locale.h"

#include "core/config.h"
#include "core/log.h"
#include "core/memory.h"

#include <stdio.h>
#include <string.h>

Locale* glocale = NULL;

void    locale_init(const char* locale_path) {
    bool path_alloced = false;
    if (!locale_path) {
        path_alloced = true;
        char* path   = MEM_TEMP(50);
        strcpy(path, DEFAULT_LOCALE_DIR);
        strcat(path, "/");
        strcat(path, DEFAULT_LOCALE);
        locale_path = path;
    }

    FILE* locale = fopen(locale_path, "r");
    if (!locale) {
        log_error_f("Failed to open locale file: %s", locale_path);
        return;
    }

    glocale = (Locale*) MEM_PERM(sizeof(Locale));

    // Initial pass to get entry count
    while (!feof(locale)) {
        char line[256];
        if (fgets(line, sizeof(line), locale)) {
            glocale->count++;
        }
    }

    // Allocate entries
    glocale->entries = (LocaleEntry*) MEM_PERM(sizeof(LocaleEntry) * glocale->count);

    // Second pass to read entries
    fseek(locale, 0, SEEK_SET);
    u64 index = 0;
    while (!feof(locale) && index < glocale->count) {
        char line[256];
        if (fgets(line, sizeof(line), locale)) {
            // Assume format: key=value
            char* equals_pos = strchr(line, '=');
            if (equals_pos) {
                *equals_pos     = '\0';
                char* key_str   = line;
                char* value_str = equals_pos + 1;

                // Trim newline from value
                char* newline_pos = strchr(value_str, '\n');
                if (newline_pos) {
                    *newline_pos = '\0';
                }

                // Store the value in the entries array
                glocale->entries[index].key = MEM_PERM(strlen(key_str) + 1);
                strcpy((char*) glocale->entries[index].key, key_str);

                glocale->entries[index].value = MEM_PERM(strlen(value_str) + 1);
                strcpy((char*) glocale->entries[index].value, value_str);
            }
        }
        index++;
    }

    if (path_alloced) {
        mem_free(MEM_TAG_TEMP, (char*) locale_path);
    }
}

void locale_shutdown(void) {
    if (!glocale) {
        LOG_WARNING("Locale not initialized");
        return;
    }

    for (u64 i = 0; i < glocale->count; i++) {
        mem_free(MEM_TAG_PERMANENT, (void*) glocale->entries[i].key);
        mem_free(MEM_TAG_PERMANENT, (void*) glocale->entries[i].value);
    }
    mem_free(MEM_TAG_PERMANENT, glocale->entries);
    glocale = NULL;
}

char* locale_get(const char* key, char* buffer, u64 buffer_size) {
    if (glocale) {
        // TODO optimize with hash map or binary search
        for (u64 i = 0; i < glocale->count; i++) {
            if (strcmp(glocale->entries[i].key, (char*) key) == 0) {
                strncpy(buffer, glocale->entries[i].value, buffer_size);
                return buffer;
            }
        }
    } else {
        LOG_WARNING("Locale not initialized");
    }

    strncpy(buffer, LOCALE_NULL_STRING, buffer_size);
    return NULL;
}
