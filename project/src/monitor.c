#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <dirent.h>
#include "backup.h"

void handle_sigint(int sig) {
    printf("\n[!] SIGINT received. Shutting down system gracefully ... \n");
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 2){
        fprintf(stderr, "usage: %s <directory_to_scan>\n", argv[0]);
        return 1;
    }
    signal(SIGINT, handle_sigint);

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("Pipe Failed");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid > 0){
        // Parent Process
        close(pipe_fd[0]);

        DIR *d = opendir(argv[1]);
        if (!d) {
            perror("Could not open directory");
            close(pipe_fd[1]);
            return 1;
        }

        struct dirent *dir;
        printf("[Parent] scanning directory: %s\n", argv[1]);

        while ((dir = readdir(d)) != NULL) {
            if (strstr(dir->d_name, ".txt")) {
                printf("[Parent] Found text file: %s. Sending to child via Pipe ... \n", dir->d_name);
                write(pipe_fd[1], dir->d_name, MAX_PATH);
                sleep(1);
            }
        }

        closedir(d);
        close(pipe_fd[1]);
        wait(NULL);
        printf("[Parent] Process complete.\n");

    } else if (pid == 0) {
        // Child Process
        close(pipe_fd[1]);

        char filename[MAX_PATH];
        FileRecord record;

        while (read(pipe_fd[0], filename, MAX_PATH) > 0) {
            printf("[Child] Received: %s. Triggering backup module ... \n", filename);
            strncpy(record.name, filename, MAX_PATH);
            snprintf(record.path, MAX_PATH, "%s/%s", argv[1], filename);

            backup_files(&record, 1);
        }

        close(pipe_fd[0]);
        exit(0);
    }

    return 0;
}