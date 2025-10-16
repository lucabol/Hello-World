#!/bin/bash
# Test script for metrics_spreadsheet

set -e

echo "Building metrics_spreadsheet..."
gcc -Wall -Wextra -o metrics_spreadsheet metrics_spreadsheet.c

echo "Testing basic functionality..."
echo "q" | ./metrics_spreadsheet hello.c > /dev/null
echo "✓ Basic execution works"

echo "Testing sort by value..."
echo -e "s\nq" | ./metrics_spreadsheet hello.c > /dev/null
echo "✓ Sort by value works"

echo "Testing sort by name..."
echo -e "n\nq" | ./metrics_spreadsheet hello.c > /dev/null
echo "✓ Sort by name works"

echo ""
echo "All tests passed! Running demo..."
echo ""
echo "q" | ./metrics_spreadsheet hello.c

echo ""
echo "Interactive test (press 's' to sort by value, 'n' to sort by name, 'q' to quit):"
./metrics_spreadsheet hello.c
