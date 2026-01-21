#!/bin/bash
# Validation script for hello.c program
# This script validates that the hello program:
# 1. Compiles successfully with strict warnings
# 2. Outputs "Hello world!" with a trailing newline
# 3. Exits with code 0

set -e

# Parse command line arguments
QUIET=0
if [[ "$1" == "--quiet" ]]; then
    QUIET=1
fi

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

log() {
    if [[ $QUIET -eq 0 ]]; then
        echo -e "$1"
    fi
}

log "${YELLOW}Running validation tests for hello.c...${NC}"

# Test 1: Compile with strict warnings
log "\n${YELLOW}Test 1: Compiling with strict warnings...${NC}"
gcc -Wall -Wextra -Wpedantic -Werror -o hello hello.c
if [[ $? -eq 0 ]]; then
    log "${GREEN}✓ Compilation successful${NC}"
else
    log "${RED}✗ Compilation failed${NC}"
    exit 1
fi

# Test 2: Check output format
log "\n${YELLOW}Test 2: Checking output format...${NC}"
OUTPUT=$(./hello)
EXPECTED="Hello world!"

if [[ "$OUTPUT" == "$EXPECTED" ]]; then
    log "${GREEN}✓ Output matches expected: '$EXPECTED'${NC}"
else
    log "${RED}✗ Output mismatch${NC}"
    log "${RED}  Expected: '$EXPECTED'${NC}"
    log "${RED}  Got:      '$OUTPUT'${NC}"
    exit 1
fi

# Test 3: Check exit code
log "\n${YELLOW}Test 3: Checking exit code...${NC}"
./hello > /dev/null
EXIT_CODE=$?
if [[ $EXIT_CODE -eq 0 ]]; then
    log "${GREEN}✓ Exit code is 0${NC}"
else
    log "${RED}✗ Exit code is $EXIT_CODE (expected 0)${NC}"
    exit 1
fi

# Test 4: Verify newline character is present
log "\n${YELLOW}Test 4: Verifying newline character in output...${NC}"
# Use hexdump to check for newline at the end
OUTPUT_HEX=$(./hello | od -An -tx1)
# Expected hex: "Hello world!\n" = 48 65 6c 6c 6f 20 77 6f 72 6c 64 21 0a
if echo "$OUTPUT_HEX" | grep -q "0a"; then
    log "${GREEN}✓ Newline character (0x0a) found in output${NC}"
else
    log "${RED}✗ Newline character not found in output${NC}"
    log "${RED}  Hex dump: $OUTPUT_HEX${NC}"
    exit 1
fi

# Test 5: Byte-level output verification
log "\n${YELLOW}Test 5: Byte-level output verification...${NC}"
EXPECTED_HEX="48656c6c6f20776f726c64210a"
ACTUAL_HEX=$(./hello | xxd -p | tr -d '\n')
if [[ "$ACTUAL_HEX" == "$EXPECTED_HEX" ]]; then
    log "${GREEN}✓ Byte-level output matches expected${NC}"
else
    log "${RED}✗ Byte-level output mismatch${NC}"
    log "${RED}  Expected: $EXPECTED_HEX${NC}"
    log "${RED}  Got:      $ACTUAL_HEX${NC}"
    exit 1
fi

log "\n${GREEN}All validation tests passed!${NC}"
exit 0
