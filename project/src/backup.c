#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "backup.h"

void backup_files(FileRecord *files, int count) {
    mkdir("backup", 0777);

    for (int i = 0; i < count; i++) {
        char destination[MAX_PATH * 2];
        snprintf(destination, sizeof(destination), "backup/%s", files[i].name);

        FILE *src = fopen(files[i].path, "rb");
        FILE *dest = fopen(destination, "wb");

        if (src && dest) {
            char buffer[1024];
            size_t bytes;
            while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0)
                fwrite(buffer, 1, bytes, dest);
            printf("[Backup Module] Successfully backed up: %s\n", files[i].name);
        } else {
            printf("[Backup Module] Error: could not open %s\n", files[i].name);
        }

        if (src) fclose(src);
        if (dest) fclose(dest);
    }
}