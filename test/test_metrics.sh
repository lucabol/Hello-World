#!/bin/bash
# Test script for metrics_spreadsheet tool

set -e

echo "Building metrics_spreadsheet..."
gcc -Wall -Wextra -Wpedantic -Werror -o metrics_spreadsheet metrics_spreadsheet.c metrics.c

echo "Testing basic spreadsheet output..."
OUTPUT=$(./metrics_spreadsheet)

# Check if output contains expected elements
if echo "$OUTPUT" | grep -q "CODE METRICS SPREADSHEET"; then
    echo "✓ Spreadsheet header found"
else
    echo "✗ Spreadsheet header missing"
    exit 1
fi

if echo "$OUTPUT" | grep -q "Total Lines"; then
    echo "✓ Total Lines metric found"
else
    echo "✗ Total Lines metric missing"
    exit 1
fi

if echo "$OUTPUT" | grep -q "Code Lines"; then
    echo "✓ Code Lines metric found"
else
    echo "✗ Code Lines metric missing"
    exit 1
fi

if echo "$OUTPUT" | grep -q "Function Count"; then
    echo "✓ Function Count metric found"
else
    echo "✗ Function Count metric missing"
    exit 1
fi

if echo "$OUTPUT" | grep -q "main"; then
    echo "✓ Function name detected"
else
    echo "✗ Function name not detected"
    exit 1
fi

echo "Testing help output..."
HELP_OUTPUT=$(./metrics_spreadsheet --help)

if echo "$HELP_OUTPUT" | grep -q "Usage:"; then
    echo "✓ Help output works"
else
    echo "✗ Help output missing"
    exit 1
fi

echo ""
echo "All tests passed!"
echo ""
