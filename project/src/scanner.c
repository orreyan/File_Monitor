#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

typedef struct {
    char name[256];
    char path[512];
    long size;
    char timestamp[64];
} FileInfo;

int is_txt_file(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (dot && strcmp(dot, ".txt") == 0)
        return 1;
    return 0;
}

void get_timestamp(time_t rawtime, char *buffer) {
    struct tm *tm_info = localtime(&rawtime);
    strftime(buffer, 64, "%Y-%m-%d %H:%M:%S", tm_info);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <directory_path>\n", argv[0]);
        printf("Example: %s /home/user/reyanproject\n", argv[0]);
        return 1;
    }

    char *dir_path = argv[1];
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char full_path[1024];
    
    FileInfo *files = NULL;
    int file_count = 0;
    int max_files = 10;
    
    files = (FileInfo *)malloc(max_files * sizeof(FileInfo));
    if (files == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }
    
    dir = opendir(dir_path);
    if (dir == NULL) {
        printf("Error: Cannot open directory '%s'\n", dir_path);
        free(files);
        return 1;
    }
    
    printf("\n=== Scanning Directory: %s ===\n", dir_path);
    printf("Looking for .txt files...\n\n");
    
    while ((entry = readdir(dir)) != NULL) {
        if (is_txt_file(entry->d_name)) {      
            snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);
            if (stat(full_path, &file_stat) == 0) {
                if (file_count >= max_files) {
                    max_files *= 2;
                    files = (FileInfo *)realloc(files, max_files * sizeof(FileInfo));
                    if (files == NULL) {
                        printf("Memory reallocation failed!\n");
                        closedir(dir);
                        return 1;
                    }
                }
                strcpy(files[file_count].name, entry->d_name);
                strcpy(files[file_count].path, full_path);
                files[file_count].size = file_stat.st_size;
                get_timestamp(file_stat.st_mtime, files[file_count].timestamp);
                printf("[%d] File: %s\n", file_count + 1, entry->d_name);
                printf("    Path: %s\n", full_path);
                printf("    Size: %ld bytes\n", file_stat.st_size);
                printf("    Modified: %s\n", files[file_count].timestamp);
                printf("    ---\n");
                
                file_count++;
            }
        }
    }
    closedir(dir);
    printf("\n=== SUMMARY ===\n");
    printf("Total .txt files found: %d\n", file_count);
    
    if (file_count == 0) {
        printf("No .txt files found in '%s'\n", dir_path);
    }
    free(files);
    
    return 0;
}
