#!/bin/bash
# Test script for metrics tool
# Tests various edge cases and functionality

set -e  # Exit on any error

echo "=== Metrics Tool Test Suite ==="

# Create test files
cat > test_empty.c << 'EOF'
EOF

cat > test_complex.c << 'EOF'
/* Multi-line comment
   with multiple lines
   and nested content */
#include <stdio.h>
# include <stdlib.h>

// Single line comment
int main() {
    // Another comment
    printf("String with { braces; and semicolons }");
    if (1) {
        for (int i = 0; i < 10; i++) {
            printf("Loop %d\n", i);
        }
    }
    return 0;
}

/* Another function */
void test_function(void) {
    int x = 5;
    /* Inline comment */ x++;
}
EOF

cat > test_comments.c << 'EOF'
// This is a comment only file
/* This is a multi-line
   comment that spans
   several lines */

// More comments
EOF

# Test 1: Build the tool
echo "Test 1: Building metrics tool..."
make metrics > /dev/null
echo "✓ Build successful"

# Test 2: Basic functionality with hello.c
echo "Test 2: Basic analysis of hello.c..."
output=$(./metrics hello.c)
if echo "$output" | grep -q "hello.c"; then
    echo "✓ Basic analysis successful"
else
    echo "✗ Basic analysis failed"
    exit 1
fi

# Test 3: CSV output
echo "Test 3: CSV output format..."
csv_output=$(./metrics --csv hello.c)
if echo "$csv_output" | grep -q "Filename,Total Lines"; then
    echo "✓ CSV output successful"
else
    echo "✗ CSV output failed"
    exit 1
fi

# Test 4: Summary output
echo "Test 4: Summary output format..."
summary_output=$(./metrics --summary hello.c)
if echo "$summary_output" | grep -q "DETAILED ANALYSIS SUMMARY"; then
    echo "✓ Summary output successful"
else
    echo "✗ Summary output failed"
    exit 1
fi

# Test 5: Help output
echo "Test 5: Help output..."
help_output=$(./metrics --help)
if echo "$help_output" | grep -q "INTERACTIVE FEATURES HELP"; then
    echo "✓ Help output successful"
else
    echo "✗ Help output failed"
    exit 1
fi

# Test 6: Error handling - non-existent file
echo "Test 6: Error handling for non-existent file..."
if ./metrics nonexistent.c > /dev/null 2>&1; then
    echo "✗ Should have failed for non-existent file"
    exit 1
else
    echo "✓ Properly handles non-existent file"
fi

# Test 7: Complex file analysis
echo "Test 7: Complex file analysis..."
complex_output=$(./metrics test_complex.c)
if echo "$complex_output" | grep -q "test_complex.c"; then
    echo "✓ Complex file analysis successful"
else
    echo "✗ Complex file analysis failed"
    exit 1
fi

# Test 8: Comments-only file
echo "Test 8: Comments-only file analysis..."
comments_output=$(./metrics test_comments.c)
if echo "$comments_output" | grep -q "0.00%"; then
    echo "✓ Comments-only file analysis successful"
else
    echo "✗ Comments-only file analysis failed"
    exit 1
fi

# Test 9: Empty file handling
echo "Test 9: Empty file handling..."
if ./metrics test_empty.c > /dev/null 2>&1; then
    echo "✗ Should have failed for empty file"
    exit 1
else
    echo "✓ Properly handles empty file"
fi

# Test 10: Strict compilation
echo "Test 10: Strict compilation..."
make metrics-strict > /dev/null
echo "✓ Strict compilation successful"

echo
echo "=== All Tests Passed! ==="
echo

# Cleanup test files
rm -f test_empty.c test_complex.c test_comments.c

echo "Test artifacts cleaned up."