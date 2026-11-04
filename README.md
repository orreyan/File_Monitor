# File Monitor and Backup System

A Unix systems programming project written in **C**. It scans a directory for text files and backs them up using a parent and child process pair that communicate through a pipe. It demonstrates the core operating system concepts behind real backup and monitoring tools: process creation, inter-process communication (IPC), signal handling, and low-level file I/O.

---

## Technology

| Area | Technology |
| --- | --- |
| Language | C |
| Platform | Linux / Unix (POSIX) |
| Build | GNU Make, GCC |
| Automation | Bash script for building, running, and logging |

---

## How It Works

```
               pipe()
 Parent process ───────────> Child process
 - opens the target directory      - reads file names from the pipe
 - finds .txt files                - calls the backup module
 - writes each name to the pipe    - copies each file into backup/
 - waits for the child to finish
```

1. `monitor` creates a pipe, then calls `fork()` to start a child process.
2. The **parent** walks the target directory with `opendir()` and `readdir()`, and sends the name of each `.txt` file through the pipe.
3. The **child** reads each file name from the pipe and passes it to the **backup module**, which copies the file in binary-safe chunks into a `backup/` directory.
4. A `SIGINT` handler shuts the program down cleanly when Ctrl+C is pressed. The parent calls `wait()` so that the child is never left as a zombie process.

A standalone `scanner` utility also reports each text file's path, size, and last-modified time. It uses `stat()` and `strftime()`, and stores results in a dynamic array that grows with `realloc()`.

---

## Concepts Demonstrated

- **Process management:** `fork()`, `wait()`, and the separate roles of parent and child processes
- **Inter-process communication:** unidirectional pipes, with unused pipe ends closed in each process
- **Signal handling:** a custom `SIGINT` handler for graceful shutdown
- **File system APIs:** `opendir`, `readdir`, `stat`, `mkdir`
- **Buffered file I/O:** chunked binary copying with `fread` and `fwrite`
- **Memory management:** dynamic allocation with `malloc` and `realloc`
- **Modular design:** the backup logic is a separate module with its own header file
- **Build automation:** a Makefile with dependency tracking, plus a shell script that checks exit codes and logs output

---

## Project Structure

```
project/
├── src/
│   ├── monitor.c       # Entry point: pipe, fork, directory scan, IPC
│   ├── backup.c        # Backup module: file copy logic
│   ├── backup.h        # Shared definitions and module interface
│   └── scanner.c       # Standalone file metadata scanner
├── script/
│   └── run_system.sh   # Builds, runs, and logs to logs/report.txt
└── Makefile
```

---

## Build and Run

**Requirements:** Linux or macOS, GCC, and Make.

```bash
cd project
make -C src -f ../Makefile        # builds src/monitor
cd src
./monitor /path/to/dir            # backs up the .txt files in that directory to ./backup
make -f ../Makefile clean         # removes build output and backups
```

To build and run the standalone scanner:

```bash
gcc -o scanner scanner.c
./scanner /path/to/dir
```

---

## Author

**Reyan Fethi**
System-level programming course project.
