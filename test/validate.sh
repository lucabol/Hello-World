#!/bin/bash
# Validation script for hello.c program
# Tests that the program compiles and runs correctly

set -e

# Parse arguments
QUIET=0
if [[ "$1" == "--quiet" ]]; then
    QUIET=1
fi

log() {
    if [[ $QUIET -eq 0 ]]; then
        echo "$@"
    fi
}

# Clean up any existing build
rm -f hello

log "=== Validation Test Suite ==="
log ""

# Test 1: Compile with strict flags (matching CI)
log "Test 1: Compiling with strict flags..."
gcc -Wall -Wextra -Wpedantic -Werror -o hello hello.c
log "✓ Compilation successful"

# Test 2: Run the program and check output
log ""
log "Test 2: Running program and validating output..."
OUTPUT=$(./hello)
EXPECTED="Hello world!"

if [[ "$OUTPUT" != "$EXPECTED" ]]; then
    echo "ERROR: Output mismatch"
    echo "Expected: '$EXPECTED'"
    echo "Got: '$OUTPUT'"
    exit 1
fi
log "✓ Output matches expected value"

# Test 3: Check exit code
log ""
log "Test 3: Checking exit code..."
./hello > /dev/null
EXIT_CODE=$?
if [[ $EXIT_CODE -ne 0 ]]; then
    echo "ERROR: Program exited with code $EXIT_CODE (expected 0)"
    exit 1
fi
log "✓ Exit code is 0"

# Test 4: Verify output has no trailing newline
log ""
log "Test 4: Verifying no trailing newline..."
OUTPUT_HEX=$(./hello | xxd -p | tr -d '\n')
EXPECTED_HEX=$(echo -n "Hello world!" | xxd -p | tr -d '\n')
if [[ "$OUTPUT_HEX" != "$EXPECTED_HEX" ]]; then
    echo "ERROR: Output bytes mismatch (possible trailing newline)"
    echo "Expected hex: $EXPECTED_HEX"
    echo "Got hex: $OUTPUT_HEX"
    exit 1
fi
log "✓ Output has no trailing newline"

log ""
log "=== All validation tests passed! ==="

# Clean up
rm -f hello
