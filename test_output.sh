#!/bin/bash

# Unit test script for Hello World program
# Tests output validation and exit code

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test configuration
SOURCE_FILE="hello.c"
BINARY_NAME="hello"
EXPECTED_OUTPUT="Hello world!"
EXPECTED_EXIT_CODE=0

echo -e "${YELLOW}Starting Hello World output validation tests...${NC}"
echo

# Test 1: Compilation test
echo -e "${YELLOW}Test 1: Compilation test${NC}"
if gcc -Wall -Wextra -o "$BINARY_NAME" "$SOURCE_FILE"; then
    echo -e "${GREEN}✓ Compilation successful${NC}"
else
    echo -e "${RED}✗ Compilation failed${NC}"
    exit 1
fi
echo

# Test 2: Output validation test
echo -e "${YELLOW}Test 2: Output validation test${NC}"
ACTUAL_OUTPUT=$(./hello)
if [ "$ACTUAL_OUTPUT" = "$EXPECTED_OUTPUT" ]; then
    echo -e "${GREEN}✓ Output matches expected: '$EXPECTED_OUTPUT'${NC}"
else
    echo -e "${RED}✗ Output mismatch${NC}"
    echo -e "${RED}  Expected: '$EXPECTED_OUTPUT'${NC}"
    echo -e "${RED}  Actual:   '$ACTUAL_OUTPUT'${NC}"
    exit 1
fi
echo

# Test 3: Exit code validation test
echo -e "${YELLOW}Test 3: Exit code validation test${NC}"
./hello > /dev/null
ACTUAL_EXIT_CODE=$?
if [ $ACTUAL_EXIT_CODE -eq $EXPECTED_EXIT_CODE ]; then
    echo -e "${GREEN}✓ Exit code matches expected: $EXPECTED_EXIT_CODE${NC}"
else
    echo -e "${RED}✗ Exit code mismatch${NC}"
    echo -e "${RED}  Expected: $EXPECTED_EXIT_CODE${NC}"
    echo -e "${RED}  Actual:   $ACTUAL_EXIT_CODE${NC}"
    exit 1
fi
echo

# Test 4: Strict compilation test (no warnings)
echo -e "${YELLOW}Test 4: Strict compilation test${NC}"
if gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello_strict "$SOURCE_FILE" 2>&1; then
    echo -e "${GREEN}✓ Strict compilation successful (no warnings)${NC}"
    rm -f hello_strict
else
    echo -e "${RED}✗ Strict compilation failed or produced warnings${NC}"
    exit 1
fi
echo

# Test 5: Alternative compiler test (clang)
echo -e "${YELLOW}Test 5: Alternative compiler test (clang)${NC}"
if command -v clang >/dev/null 2>&1; then
    if clang -o hello_clang "$SOURCE_FILE"; then
        CLANG_OUTPUT=$(./hello_clang)
        if [ "$CLANG_OUTPUT" = "$EXPECTED_OUTPUT" ]; then
            echo -e "${GREEN}✓ Clang compilation and output validation successful${NC}"
            rm -f hello_clang
        else
            echo -e "${RED}✗ Clang output mismatch${NC}"
            echo -e "${RED}  Expected: '$EXPECTED_OUTPUT'${NC}"
            echo -e "${RED}  Actual:   '$CLANG_OUTPUT'${NC}"
            rm -f hello_clang
            exit 1
        fi
    else
        echo -e "${RED}✗ Clang compilation failed${NC}"
        exit 1
    fi
else
    echo -e "${YELLOW}⚠ Clang not available, skipping test${NC}"
fi
echo

# Cleanup
rm -f "$BINARY_NAME"

echo -e "${GREEN}All tests passed! ✓${NC}"
echo -e "${GREEN}Output validation complete.${NC}"