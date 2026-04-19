#!/bin/bash

# Variables
EXECUTABLE="./monitor"
LOG_DIR="logs"
LOG_FILE="$LOG_DIR/report.txt"

# Create log directory if missing
if [ ! -d "$LOG_DIR" ]; then
    mkdir "$LOG_DIR"
fi

# Compile using make, redirect output to log
make > "$LOG_FILE" 2>&1

# Check if compile succeeded
if [ $? -ne 0 ]; then
    echo "ERROR: Compilation failed. See $LOG_FILE"
    exit 1
fi

# Run executable, append output to log
$EXECUTABLE >> "$LOG_FILE" 2>&1

# Check if run succeeded
if [ $? -ne 0 ]; then
    echo "ERROR: Program failed. See $LOG_FILE"
    exit 2
fi

# Success message
echo "SUCCESS: Output saved to $LOG_FILE"