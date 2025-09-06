#!/bin/bash

# Demo script for the metrics analyzer
echo "=== Code Metrics Analyzer Demo ==="
echo ""
echo "Analyzing hello.c file..."
echo ""

# Show the file being analyzed
echo "Contents of hello.c:"
echo "===================="
cat hello.c
echo ""
echo "===================="
echo ""

# Run metrics in non-interactive mode by automatically sending 'q' after a brief delay
echo "Starting interactive metrics analyzer..."
echo "The tool will show a spreadsheet-like interface with:"
echo "- Summary view with overall metrics"
echo "- Functions view with detected functions"
echo "- Includes view with include statements"
echo ""
echo "Use TAB to switch between views, arrow keys to navigate, 'q' to quit"
echo ""
echo "Press Enter to start the analyzer..."
read -r

# Run the metrics tool
./metrics hello.c