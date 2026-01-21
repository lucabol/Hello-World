#!/bin/bash
# Basic validation script for hello.c
# Tests compilation and basic output

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT"

# Parse command line arguments
QUIET=0
if [[ "$1" == "--quiet" ]]; then
    QUIET=1
fi

log() {
    if [[ $QUIET -eq 0 ]]; then
        echo "$@"
    fi
}

log "=== Basic Validation Tests for hello.c ==="
log ""

# Clean up any existing binaries
rm -f hello

# Test 1: Strict compilation (matches CI)
log "Test 1: Compiling with strict flags..."
gcc -Wall -Wextra -Wpedantic -Werror -o hello hello.c
log "✓ Compilation successful with strict flags"
log ""

# Test 2: Verify output
log "Test 2: Verifying program output..."
OUTPUT=$(./hello)
EXPECTED="Hello world!"

if [[ "$OUTPUT" != "$EXPECTED" ]]; then
    echo "ERROR: Output mismatch"
    echo "Expected: '$EXPECTED'"
    echo "Got: '$OUTPUT'"
    exit 1
fi
log "✓ Output matches expected: '$EXPECTED'"
log ""

# Test 3: Verify exit code
log "Test 3: Verifying exit code..."
./hello > /dev/null
EXIT_CODE=$?
if [[ $EXIT_CODE -ne 0 ]]; then
    echo "ERROR: Expected exit code 0, got $EXIT_CODE"
    exit 1
fi
log "✓ Exit code is 0"
log ""

# Test 4: Verify no trailing newline (byte-level check)
log "Test 4: Verifying no trailing newline..."
OUTPUT_HEX=$(./hello | od -A n -t x1 | tr -d ' \n')
EXPECTED_HEX="48656c6c6f20776f726c6421"  # "Hello world!" in hex

if [[ "$OUTPUT_HEX" != "$EXPECTED_HEX" ]]; then
    echo "ERROR: Output has incorrect bytes (possible trailing newline)"
    echo "Expected hex: $EXPECTED_HEX"
    echo "Got hex:      $OUTPUT_HEX"
    exit 1
fi
log "✓ No trailing newline (exact byte match)"
log ""

# Clean up
rm -f hello

log "=== All basic validation tests passed ==="
exit 0
