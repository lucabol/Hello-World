#!/bin/bash

# validate_output.sh - Validates program output format
# Usage: ./validate_output.sh <executable_path> [build_type]

set -e

EXECUTABLE="$1"
BUILD_TYPE="${2:-build}"

if [[ ! -f "$EXECUTABLE" ]]; then
    echo "ERROR: Executable '$EXECUTABLE' not found"
    exit 1
fi

if [[ ! -x "$EXECUTABLE" ]]; then
    echo "ERROR: '$EXECUTABLE' is not executable"
    exit 1
fi

echo "Validating output for $BUILD_TYPE..."

# Capture actual output
"$EXECUTABLE" > actual_output.txt
EXIT_CODE=$?

# Check exit code
if [[ $EXIT_CODE -ne 0 ]]; then
    echo "ERROR: Program exited with code $EXIT_CODE, expected 0"
    rm -f actual_output.txt
    exit 1
fi

# Generate expected output using printf for precise control
printf "Hello world!" > expected_output.txt

# Compare outputs
if ! cmp -s actual_output.txt expected_output.txt; then
    echo "ERROR: Output does not match expected format for $BUILD_TYPE"
    echo "Expected: 'Hello world!' (12 bytes, no trailing newline)"
    echo -n "Actual: '"
    cat actual_output.txt
    echo "' ($(wc -c < actual_output.txt) bytes)"
    echo
    echo "Byte-level comparison:"
    echo "Expected:" && xxd expected_output.txt
    echo "Actual:" && xxd actual_output.txt
    rm -f actual_output.txt expected_output.txt
    exit 1
fi

echo "✓ Output validation passed for $BUILD_TYPE"
rm -f actual_output.txt expected_output.txt