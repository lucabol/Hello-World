#!/bin/bash
# Test script to validate Hello World C program
# Usage: ./validate.sh <compiler> [executable_name]
# Example: ./validate.sh gcc hello
#          ./validate.sh clang hello_clang

set -e  # Exit on any error

COMPILER=${1:-gcc}
EXECUTABLE=${2:-hello}
SOURCE="hello.c"

echo "Testing with $COMPILER..."

# Define strict compilation flags
STRICT_FLAGS="-Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion"

# Compile with strict flags
echo "Compiling with strict flags: $COMPILER $STRICT_FLAGS -o $EXECUTABLE $SOURCE"
$COMPILER $STRICT_FLAGS -o $EXECUTABLE $SOURCE

# Check if compilation succeeded
if [ ! -f "$EXECUTABLE" ]; then
    echo "ERROR: Compilation failed - executable $EXECUTABLE not found"
    exit 1
fi

echo "Compilation successful!"

# Run the program and capture output
echo "Running ./$EXECUTABLE..."
OUTPUT=$(./$EXECUTABLE)
EXIT_CODE=$?

# Validate exit code
if [ $EXIT_CODE -ne 0 ]; then
    echo "ERROR: Program exited with code $EXIT_CODE, expected 0"
    exit 1
fi

echo "Exit code validation: PASSED (exit code: $EXIT_CODE)"

# Validate output content
EXPECTED_OUTPUT="Hello world!"
if [ "$OUTPUT" != "$EXPECTED_OUTPUT" ]; then
    echo "ERROR: Output mismatch"
    echo "Expected: '$EXPECTED_OUTPUT'"
    echo "Got:      '$OUTPUT'"
    exit 1
fi

echo "Output validation: PASSED"
echo "Program output: '$OUTPUT'"

echo "All tests passed for $COMPILER!"