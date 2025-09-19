#!/bin/bash

# test_output.sh - Automated test for Hello World program output and exit code
# This script validates that the program produces the expected output and exit code

set -e  # Exit on any error

# Expected output (exactly what the program should print)
EXPECTED_OUTPUT="Hello world!"
EXPECTED_EXIT_CODE=0

# Make sure we're in the right directory (where the compiled binary should be)
if [ ! -f "./hello" ]; then
    echo "ERROR: ./hello binary not found. Make sure to compile the program first with:"
    echo "  gcc -o hello hello.c"
    exit 1
fi

echo "Running Hello World output test..."

# Capture both stdout and stderr, and the exit code
# Temporarily disable exit-on-error to capture the actual exit code
set +e
ACTUAL_OUTPUT=$(./hello 2>&1)
ACTUAL_EXIT_CODE=$?
set -e

echo "Expected output: '$EXPECTED_OUTPUT'"
echo "Actual output:   '$ACTUAL_OUTPUT'"
echo "Expected exit code: $EXPECTED_EXIT_CODE"
echo "Actual exit code:   $ACTUAL_EXIT_CODE"

# Check if output matches exactly
if [ "$ACTUAL_OUTPUT" != "$EXPECTED_OUTPUT" ]; then
    echo "FAIL: Output mismatch!"
    echo "Expected: '$EXPECTED_OUTPUT'"
    echo "Actual:   '$ACTUAL_OUTPUT'"
    exit 1
fi

# Check if exit code matches
if [ "$ACTUAL_EXIT_CODE" -ne "$EXPECTED_EXIT_CODE" ]; then
    echo "FAIL: Exit code mismatch!"
    echo "Expected: $EXPECTED_EXIT_CODE"
    echo "Actual:   $ACTUAL_EXIT_CODE"
    exit 1
fi

echo "PASS: All tests passed!"
echo "✓ Output matches expected: '$EXPECTED_OUTPUT'"
echo "✓ Exit code matches expected: $EXPECTED_EXIT_CODE"