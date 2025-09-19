#!/bin/bash
# Automated test script for Hello World program
# Validates exact output and exit code behavior

set -e

echo "Running Hello World validation tests..."

# Clean any existing binary
rm -f hello

# Compile and test the program
echo "1. Compiling with warnings enabled..."
gcc -Wall -Wextra -o hello hello.c
if [ $? -ne 0 ]; then
    echo "FAIL: Compilation failed"
    exit 1
fi
echo "   ✓ Compilation successful"

# Capture output and exit code
echo "2. Testing program output..."
output=$(./hello)
exit_code=$?

# Verify exact output (12 bytes, no newline)
expected="Hello world!"
if [ "$output" = "$expected" ] && [ $exit_code -eq 0 ]; then
    echo "   ✓ Output matches expected: '$expected'"
    echo "   ✓ Exit code is 0"
    echo "PASS: All tests successful"
    exit 0
else
    echo "FAIL: Expected '$expected' with exit code 0"
    echo "      Got '$output' with exit code $exit_code"
    exit 1
fi