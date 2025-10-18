#!/bin/bash
# Simple test script for the metrics tool
# Tests that the tool compiles and runs without errors

set -e

echo "Testing Code Metrics Spreadsheet Tool..."
echo ""

# Test 1: Compilation
echo "Test 1: Compiling metrics.c..."
gcc -Wall -Wextra -o metrics_test metrics.c
echo "✓ Compilation successful"
echo ""

# Test 2: Run with all menu options
echo "Test 2: Running all menu options..."
if echo -e "1\n2\n3\n4\n5" | ./metrics_test > /dev/null 2>&1; then
    echo "✓ All menu options executed successfully"
else
    echo "✗ Failed to run menu options"
    exit 1
fi
echo ""

# Test 3: Check that output contains expected content
echo "Test 3: Checking output content..."
OUTPUT=$(echo -e "1\n5" | ./metrics_test 2>&1)

if echo "$OUTPUT" | grep -q "METRICS SPREADSHEET VIEW"; then
    echo "✓ Spreadsheet view found in output"
else
    echo "✗ Spreadsheet view not found"
    exit 1
fi

if echo "$OUTPUT" | grep -q "Total Lines"; then
    echo "✓ Line metrics found in output"
else
    echo "✗ Line metrics not found"
    exit 1
fi

# Check function detection by looking at function count
if echo "$OUTPUT" | grep -q "Functions.*1"; then
    echo "✓ Function detection working"
else
    echo "✗ Function detection failed"
    exit 1
fi
echo ""

# Cleanup
rm -f metrics_test

echo "All tests passed!"
echo ""
