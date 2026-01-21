#!/bin/bash
# Validation script for hello.c
# Ensures the program outputs exactly "Hello world!" with no trailing newline

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

QUIET=false
if [[ "$1" == "--quiet" ]]; then
    QUIET=true
fi

log() {
    if [ "$QUIET" = false ]; then
        echo "$@"
    fi
}

log "Starting validation tests for hello.c..."

# Build the program with consistent flags matching Makefile
log "Building hello.c..."
gcc -Wall -Wextra -Wpedantic -Wformat=2 -std=c99 -o hello hello.c

# Test 1: Check basic output
log "Test 1: Checking basic output..."
OUTPUT=$(./hello)
EXPECTED="Hello world!"
if [ "$OUTPUT" = "$EXPECTED" ]; then
    log -e "${GREEN}✓${NC} Output is correct"
else
    echo -e "${RED}✗${NC} Output mismatch!"
    echo "Expected: '$EXPECTED'"
    echo "Got: '$OUTPUT'"
    exit 1
fi

# Test 2: Check exit code
log "Test 2: Checking exit code..."
./hello > /dev/null
EXIT_CODE=$?
if [ $EXIT_CODE -eq 0 ]; then
    log -e "${GREEN}✓${NC} Exit code is correct (0)"
else
    echo -e "${RED}✗${NC} Exit code mismatch!"
    echo "Expected: 0"
    echo "Got: $EXIT_CODE"
    exit 1
fi

# Test 3: Check no trailing newline
log "Test 3: Checking for no trailing newline..."
OUTPUT_HEX=$(./hello | xxd -p)
EXPECTED_HEX="48656c6c6f20776f726c6421"
if [ "$OUTPUT_HEX" = "$EXPECTED_HEX" ]; then
    log -e "${GREEN}✓${NC} No trailing newline (correct)"
else
    echo -e "${RED}✗${NC} Output has unexpected bytes!"
    echo "Expected hex: $EXPECTED_HEX"
    echo "Got hex: $OUTPUT_HEX"
    exit 1
fi

log -e "${GREEN}All validation tests passed!${NC}"
