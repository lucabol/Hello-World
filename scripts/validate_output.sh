#!/usr/bin/env bash

# validate_output.sh - Validates program output format
# 
# Usage: ./validate_output.sh <executable_path> [build_type]
#
# This script validates that a compiled program outputs exactly "Hello world!" 
# (12 bytes, no trailing newline) and exits with code 0.
#
# Arguments:
#   executable_path - Path to the executable to test
#   build_type      - Optional build type label for error reporting (default: "build")
#
# Exit codes:
#   0 - Validation passed (correct output and exit code)
#   1 - Validation failed (wrong output, non-zero exit, or script error)
#
# Expected output: Exactly "Hello world!" (12 bytes, no trailing newline)
# Expected exit code: 0
#
# The script captures both stdout and stderr, warns about stderr output,
# and provides detailed byte-level comparison on mismatch using xxd.

set -e

EXECUTABLE="$1"
BUILD_TYPE="${2:-build}"

# Create temporary directory for test files
TEMP_DIR=$(mktemp -d)
ACTUAL_OUTPUT="$TEMP_DIR/actual_output.txt"
ACTUAL_STDERR="$TEMP_DIR/actual_stderr.txt"
EXPECTED_OUTPUT="$TEMP_DIR/expected_output.txt"

# Cleanup function
cleanup() {
    rm -rf "$TEMP_DIR"
}

# Set trap to cleanup on exit
trap cleanup EXIT

if [[ ! -f "$EXECUTABLE" ]]; then
    echo "ERROR: Executable '$EXECUTABLE' not found"
    exit 1
fi

if [[ ! -x "$EXECUTABLE" ]]; then
    echo "ERROR: '$EXECUTABLE' is not executable"
    exit 1
fi

echo "Validating output for $BUILD_TYPE..."

# Capture actual output and stderr, but don't let set -e abort on non-zero exit
set +e
"$EXECUTABLE" > "$ACTUAL_OUTPUT" 2> "$ACTUAL_STDERR"
EXIT_CODE=$?
set -e

# Check exit code
if [[ $EXIT_CODE -ne 0 ]]; then
    echo "ERROR: Program exited with code $EXIT_CODE, expected 0"
    echo "Standard output:"
    cat "$ACTUAL_OUTPUT"
    echo "Standard error:"
    cat "$ACTUAL_STDERR"
    exit 1
fi

# Check if there was any stderr output (unexpected)
if [[ -s "$ACTUAL_STDERR" ]]; then
    echo "WARNING: Program produced stderr output:"
    cat "$ACTUAL_STDERR"
fi

# Generate expected output using printf for precise control
printf "Hello world!" > "$EXPECTED_OUTPUT"

# Compare outputs byte-for-byte
if ! cmp -s "$ACTUAL_OUTPUT" "$EXPECTED_OUTPUT"; then
    echo "ERROR: Output does not match expected format for $BUILD_TYPE"
    echo "Expected: 'Hello world!' (12 bytes, no trailing newline)"
    echo -n "Actual: '"
    cat "$ACTUAL_OUTPUT"
    echo "' ($(wc -c < "$ACTUAL_OUTPUT") bytes)"
    echo
    echo "Byte-level comparison:"
    echo "Expected:" && xxd "$EXPECTED_OUTPUT"
    echo "Actual:" && xxd "$ACTUAL_OUTPUT"
    exit 1
fi

echo "✓ Output validation passed for $BUILD_TYPE"
