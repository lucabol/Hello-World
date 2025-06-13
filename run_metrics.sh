#!/bin/bash
# Code Metrics Spreadsheet Tool Launcher
# Simple launcher script for the code metrics analyzer

echo "Code Metrics Spreadsheet Tool"
echo "=============================="
echo ""
echo "This tool analyzes hello.c and displays code metrics in a spreadsheet-like format."
echo ""

# Check if hello.c exists
if [ ! -f "hello.c" ]; then
    echo "Error: hello.c not found in current directory."
    exit 1
fi

# Check if Python 3 is available
if ! command -v python3 &> /dev/null; then
    echo "Error: Python 3 is required to run this tool."
    exit 1
fi

# Run the code metrics tool
python3 code_metrics.py