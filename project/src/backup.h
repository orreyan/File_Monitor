#ifndef BACKUP_H
#define BACKUP_H

#define MAX_PATH 512


typedef struct {
    char name[MAX_PATH];
    char path[MAX_PATH];
} FileRecord;

void backup_files(FileRecord *files, int count);

#endif