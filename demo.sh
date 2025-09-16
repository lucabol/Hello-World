#!/bin/bash

# Code Metrics Spreadsheet Demo
# This script demonstrates the spreadsheet-like interface for code metrics

echo "=== Code Metrics Analyzer Demo ==="
echo
echo "1. Building the metrics analyzer..."
gcc -Wall -Wextra -o metrics metrics.c

if [ $? -eq 0 ]; then
    echo "✓ Build successful!"
else
    echo "✗ Build failed!"
    exit 1
fi

echo
echo "2. Analyzing hello.c in table format:"
echo "-----------------------------------"
./metrics hello.c

echo
echo "3. Analyzing hello.c in CSV format (for spreadsheet import):"
echo "-----------------------------------------------------------"
./metrics --csv hello.c

echo
echo "4. Analyzing multiple files:"
echo "---------------------------"
./metrics hello.c metrics.c

echo
echo "5. CSV output for multiple files (ready for spreadsheet):"
echo "--------------------------------------------------------"
./metrics --csv hello.c metrics.c

echo
echo "=== Demo Complete ==="
echo
echo "To use the metrics analyzer:"
echo "  ./metrics <file1> [file2] ...           # Table format"
echo "  ./metrics --csv <file1> [file2] ...     # CSV format for spreadsheets"
echo
echo "The CSV output can be imported into Excel, Google Sheets, or any spreadsheet application."