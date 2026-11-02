#!/bin/bash

# Welcome message
echo "==============================================="
echo "Automated Peak Processing with ROOT (Interactive Mode)"
echo "==============================================="

# Prompt for log file name
echo "Enter the name of the log file (e.g., O_tag0030.txt):"
read log_file

# Start the logging session and ROOT process
echo "Starting logging session. All output will be saved to $log_file."
script -q "$log_file" <<EOF
root -l
.L Peak_Sources_Finding.C
Run1()
EOF

# Inform the user that the ROOT process is running interactively
echo "==============================================="
echo "ROOT is running interactively. Complete the fitting process as prompted."
echo "Enter fit ranges, background ranges, and let the script complete."
echo "Once done, type '.q' to exit ROOT and save the log."
echo "==============================================="

# Start ROOT interactively
root -l

# Completion message
echo "==============================================="
echo "ROOT session completed. Log saved to $log_file."
echo "==============================================="

