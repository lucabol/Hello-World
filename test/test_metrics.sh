#!/bin/bash
# Test script for metrics_spreadsheet tool

set -e

echo "Building metrics_spreadsheet with strict flags..."
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -o metrics_spreadsheet metrics_spreadsheet.c metrics.c

echo ""
echo "=== Testing Basic Functionality ==="
echo ""

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

echo ""
echo "Testing help output..."
HELP_OUTPUT=$(./metrics_spreadsheet --help)

if echo "$HELP_OUTPUT" | grep -q "Usage:"; then
    echo "✓ Help output works"
else
    echo "✗ Help output missing"
    exit 1
fi

echo ""
echo "=== Testing Edge Cases ==="
echo ""

# Test multiline comments
echo "Testing multiline comments..."
MC_OUTPUT=$(./metrics_spreadsheet test/inputs/multiline_comments.c)
if echo "$MC_OUTPUT" | grep -q "Comment Lines"; then
    echo "✓ Multiline comments file processed"
else
    echo "✗ Multiline comments file failed"
    exit 1
fi

# Test strings with comment-like sequences
echo "Testing strings with comment-like sequences..."
SC_OUTPUT=$(./metrics_spreadsheet test/inputs/strings_with_comments.c)
if echo "$SC_OUTPUT" | grep -q "Total Lines"; then
    echo "✓ Strings with comments file processed"
else
    echo "✗ Strings with comments file failed"
    exit 1
fi

# Test empty file
echo "Testing empty file..."
EMPTY_OUTPUT=$(./metrics_spreadsheet test/inputs/empty.c)
if echo "$EMPTY_OUTPUT" | grep -q "Total Lines.*1"; then
    echo "✓ Empty file processed"
else
    echo "✗ Empty file failed"
    exit 1
fi

# Test only comments
echo "Testing file with only comments..."
OC_OUTPUT=$(./metrics_spreadsheet test/inputs/only_comments.c)
if echo "$OC_OUTPUT" | grep -q "Comment Lines"; then
    echo "✓ Only comments file processed"
else
    echo "✗ Only comments file failed"
    exit 1
fi

# Test multiple functions
echo "Testing file with multiple functions..."
MF_OUTPUT=$(./metrics_spreadsheet test/inputs/multiple_functions.c)
if echo "$MF_OUTPUT" | grep -q "Function Count"; then
    FUNC_COUNT=$(echo "$MF_OUTPUT" | grep "Function Count" | grep -oP '\d+' | head -1)
    if [ "$FUNC_COUNT" -ge 3 ]; then
        echo "✓ Multiple functions detected (count: $FUNC_COUNT)"
    else
        echo "✗ Not enough functions detected (count: $FUNC_COUNT)"
        exit 1
    fi
else
    echo "✗ Multiple functions file failed"
    exit 1
fi

echo ""
echo "=== Testing Error Handling ==="
echo ""

# Test nonexistent file
echo "Testing error handling for nonexistent file..."
if ./metrics_spreadsheet nonexistent.c 2>&1 | grep -q "Error"; then
    echo "✓ Error handling works for nonexistent file"
else
    echo "✗ Error handling missing"
    exit 1
fi

echo ""
echo "All tests passed!"
echo ""
