#!/bin/bash
# Demo script for the Code Metrics Spreadsheet Tool
# Demonstrates all features of the tool non-interactively

echo "╔═══════════════════════════════════════════════════════════════════════════╗"
echo "║                 CODE METRICS SPREADSHEET TOOL DEMO                        ║"
echo "╚═══════════════════════════════════════════════════════════════════════════╝"
echo ""
echo "This demo showcases the spreadsheet interface for analyzing hello.c"
echo ""

# Demo 1: Full Metrics Table
echo "========== Demo 1: Full Metrics Table =========="
printf "1\n0\n" | ./spreadsheet hello.c 2>/dev/null | grep -A 100 "CODE METRICS SPREADSHEET$" | head -40

echo ""
echo "========== Demo 2: Line Counts Only =========="
printf "2\n0\n" | ./spreadsheet hello.c 2>/dev/null | grep -A 100 "LINE COUNT METRICS" | head -20

echo ""
echo "========== Demo 3: Code Structure Only =========="
printf "3\n0\n" | ./spreadsheet hello.c 2>/dev/null | grep -A 100 "CODE STRUCTURE METRICS" | head -20

echo ""
echo "========== Demo 4: Summary Statistics =========="
printf "4\n0\n" | ./spreadsheet hello.c 2>/dev/null | grep -A 100 "SUMMARY STATISTICS" | head -20

echo ""
echo "========== Demo 5: CSV Export =========="
printf "5\n0\n" | ./spreadsheet hello.c 2>/dev/null | grep -A 5 "Metrics exported"
if [ -f hello.c_metrics.csv ]; then
    echo ""
    echo "CSV file contents:"
    echo "---"
    cat hello.c_metrics.csv
    echo "---"
    rm -f hello.c_metrics.csv
fi

echo ""
echo "╔═══════════════════════════════════════════════════════════════════════════╗"
echo "║                         DEMO COMPLETE                                     ║"
echo "╚═══════════════════════════════════════════════════════════════════════════╝"
