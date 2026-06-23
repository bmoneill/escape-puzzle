#include <stdio.h>
#include <unistd.h>

#define ASSET_DIR_1 "assets/"
#define ASSET_DIR_2 "../assets/"
#define ASSET_DIR_3 "../../assets/"
#define ASSET_DIR_4 "../../../assets/"

static const char* paths[] = {
    ASSET_DIR_1,
    ASSET_DIR_2,
    ASSET_DIR_3,
    ASSET_DIR_4,
};
static char path_buffer[64];

char*       get_path(const char* filename) {
    for (int i = 0; i < 4; i++) {
        sprintf(path_buffer, "%s%s", paths[i], filename);
        if (access(path_buffer, F_OK) == 0) {
            return path_buffer;
        }
    }
    return NULL;
}
