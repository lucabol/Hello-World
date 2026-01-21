#!/bin/bash
# Test program behavior in minimal environments
# Tests execution with minimal PATH, empty environment, and restricted settings

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT"

echo "=== Testing Minimal Environment Behavior ==="
echo ""

# Compile the program
rm -f hello_test
gcc -Wall -Wextra -o hello_test hello.c
echo "Program compiled successfully"
echo ""

EXPECTED_OUTPUT="Hello world!"

# Test 1: Run with minimal PATH
echo "Test 1: Testing with minimal PATH..."
OUTPUT=$(PATH=/usr/bin:/bin ./hello_test)
if [[ "$OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
    echo "✗ ERROR: Program failed with minimal PATH"
    echo "  Expected: '$EXPECTED_OUTPUT'"
    echo "  Got: '$OUTPUT'"
    rm -f hello_test
    exit 1
fi
echo "✓ Program works with minimal PATH"
echo ""

# Test 2: Run with empty environment (env -i)
echo "Test 2: Testing with empty environment..."
OUTPUT=$(env -i ./hello_test)
if [[ "$OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
    echo "✗ ERROR: Program failed with empty environment"
    echo "  Expected: '$EXPECTED_OUTPUT'"
    echo "  Got: '$OUTPUT'"
    rm -f hello_test
    exit 1
fi
echo "✓ Program works with empty environment"
echo ""

# Test 3: Run with no HOME variable
echo "Test 3: Testing without HOME variable..."
OUTPUT=$(unset HOME; ./hello_test)
if [[ "$OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
    echo "✗ ERROR: Program failed without HOME"
    echo "  Expected: '$EXPECTED_OUTPUT'"
    echo "  Got: '$OUTPUT'"
    rm -f hello_test
    exit 1
fi
echo "✓ Program works without HOME variable"
echo ""

# Test 4: Run with no USER variable
echo "Test 4: Testing without USER variable..."
OUTPUT=$(unset USER; ./hello_test)
if [[ "$OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
    echo "✗ ERROR: Program failed without USER"
    echo "  Expected: '$EXPECTED_OUTPUT'"
    echo "  Got: '$OUTPUT'"
    rm -f hello_test
    exit 1
fi
echo "✓ Program works without USER variable"
echo ""

# Test 5: Run with minimal environment variables
echo "Test 5: Testing with only essential environment variables..."
OUTPUT=$(env -i PATH=/usr/bin:/bin ./hello_test)
if [[ "$OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
    echo "✗ ERROR: Program failed with minimal environment"
    echo "  Expected: '$EXPECTED_OUTPUT'"
    echo "  Got: '$OUTPUT'"
    rm -f hello_test
    exit 1
fi
echo "✓ Program works with minimal environment variables"
echo ""

# Test 6: Run from different working directory
echo "Test 6: Testing from different working directory..."
TEMP_DIR=$(mktemp -d)
cp hello_test "$TEMP_DIR/"
OUTPUT=$(cd "$TEMP_DIR" && ./hello_test)
rm -rf "$TEMP_DIR"
if [[ "$OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
    echo "✗ ERROR: Program failed from different directory"
    echo "  Expected: '$EXPECTED_OUTPUT'"
    echo "  Got: '$OUTPUT'"
    rm -f hello_test
    exit 1
fi
echo "✓ Program works from different working directory"
echo ""

# Test 7: Test with no terminal (non-interactive)
echo "Test 7: Testing in non-interactive mode..."
OUTPUT=$(echo "" | ./hello_test)
if [[ "$OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
    echo "✗ ERROR: Program failed in non-interactive mode"
    echo "  Expected: '$EXPECTED_OUTPUT'"
    echo "  Got: '$OUTPUT'"
    rm -f hello_test
    exit 1
fi
echo "✓ Program works in non-interactive mode"
echo ""

# Test 8: Test with ulimit restrictions
echo "Test 8: Testing with file descriptor limits..."
# Set a reasonable limit that shouldn't affect the program
# Run in subshell and capture exit status properly
if ! (ulimit -n 32 && ./hello_test > /dev/null && [[ "$(./hello_test)" == "$EXPECTED_OUTPUT" ]]); then
    echo "✗ ERROR: Program failed with file descriptor limits"
    rm -f hello_test
    exit 1
fi
echo "✓ Program works with file descriptor limits"
echo ""

# Clean up
rm -f hello_test

echo "=== All minimal environment tests passed ==="
exit 0
