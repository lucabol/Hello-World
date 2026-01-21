#!/bin/bash
# Comprehensive test suite for code_metrics.c error handling
# Tests various file I/O failure scenarios including:
# - Non-existent files
# - Permission errors
# - Invalid file formats
# - Write failures

set -e

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

TEST_PASSED=0
TEST_FAILED=0

log() {
    echo -e "$1"
}

# Function to run a test
run_test() {
    local test_name="$1"
    local command="$2"
    local expected_exit_code="$3"
    local expected_output="$4"
    
    log "\n${YELLOW}Test: $test_name${NC}"
    
    # Run the command and capture output and exit code
    set +e
    output=$(eval "$command" 2>&1)
    actual_exit_code=$?
    set -e
    
    # Check exit code
    if [ "$actual_exit_code" -ne "$expected_exit_code" ]; then
        log "${RED}✗ FAILED: Expected exit code $expected_exit_code, got $actual_exit_code${NC}"
        log "${RED}  Output: $output${NC}"
        TEST_FAILED=$((TEST_FAILED + 1))
        return 1
    fi
    
    # Check if expected output is in actual output (if provided)
    if [ -n "$expected_output" ] && ! echo "$output" | grep -q "$expected_output"; then
        log "${RED}✗ FAILED: Expected output to contain '$expected_output'${NC}"
        log "${RED}  Actual output: $output${NC}"
        TEST_FAILED=$((TEST_FAILED + 1))
        return 1
    fi
    
    log "${GREEN}✓ PASSED${NC}"
    TEST_PASSED=$((TEST_PASSED + 1))
    return 0
}

log "${YELLOW}========================================${NC}"
log "${YELLOW}Code Metrics Error Handling Test Suite${NC}"
log "${YELLOW}========================================${NC}"

# Compile the program first
log "\n${YELLOW}Compiling code_metrics.c...${NC}"
if gcc -Wall -Wextra -Wpedantic -Werror -o code_metrics code_metrics.c; then
    log "${GREEN}✓ Compilation successful${NC}"
else
    log "${RED}✗ Compilation failed${NC}"
    exit 1
fi

# Test 1: Missing command line argument
run_test "Missing command line argument" \
    "./code_metrics" \
    1 \
    "Usage:"

# Test 2: Non-existent file
run_test "Non-existent file" \
    "./code_metrics /nonexistent/file.c" \
    1 \
    "File not found"

# Test 3: Invalid file format (not .c or .h)
run_test "Invalid file format" \
    "./code_metrics README.md" \
    1 \
    "Invalid file format"

# Test 4: Valid file analysis
run_test "Valid file analysis" \
    "./code_metrics hello.c" \
    0 \
    "Code Metrics for:"

# Test 5: Permission denied - read
log "\n${YELLOW}Test: Permission denied - read${NC}"
touch /tmp/test_no_read.c
chmod 000 /tmp/test_no_read.c
run_test "Permission denied on input file" \
    "./code_metrics /tmp/test_no_read.c" \
    1 \
    "Permission denied"
rm -f /tmp/test_no_read.c

# Test 6: Write to output file
run_test "Write metrics to output file" \
    "./code_metrics hello.c /tmp/test_output.txt && cat /tmp/test_output.txt" \
    0 \
    "Code Metrics Report"
rm -f /tmp/test_output.txt

# Test 7: Permission denied - write
log "\n${YELLOW}Test: Permission denied - write${NC}"
touch /tmp/test_no_write.txt
chmod 000 /tmp/test_no_write.txt
run_test "Permission denied on output file" \
    "./code_metrics hello.c /tmp/test_no_write.txt" \
    1 \
    "Permission denied"
rm -f /tmp/test_no_write.txt

# Test 8: Write to read-only directory
run_test "Write to read-only directory" \
    "./code_metrics hello.c /proc/test_output.txt" \
    1 \
    ""

# Test 9: Empty filename
run_test "Empty input file path handling" \
    "./code_metrics ''" \
    1 \
    ""

# Test 10: Analyze code_metrics.c itself (self-analysis)
run_test "Self-analysis of code_metrics.c" \
    "./code_metrics code_metrics.c" \
    0 \
    "Lines:"

# Test 11: Analyze with valid .h file extension
log "\n${YELLOW}Test: Valid .h file extension${NC}"
echo "/* test header */" > /tmp/test.h
run_test "Valid .h file" \
    "./code_metrics /tmp/test.h" \
    0 \
    "Code Metrics for:"
rm -f /tmp/test.h

# Test 12: Multiple successful analyses
log "\n${YELLOW}Test: Multiple analyses in sequence${NC}"
./code_metrics hello.c > /dev/null 2>&1
./code_metrics code_metrics.c > /dev/null 2>&1
if [ $? -eq 0 ]; then
    log "${GREEN}✓ PASSED: Multiple analyses completed successfully${NC}"
    TEST_PASSED=$((TEST_PASSED + 1))
else
    log "${RED}✗ FAILED: Multiple analyses failed${NC}"
    TEST_FAILED=$((TEST_FAILED + 1))
fi

# Summary
log "\n${YELLOW}========================================${NC}"
log "${YELLOW}Test Summary${NC}"
log "${YELLOW}========================================${NC}"
log "${GREEN}Passed: $TEST_PASSED${NC}"
log "${RED}Failed: $TEST_FAILED${NC}"
log "${YELLOW}========================================${NC}"

if [ $TEST_FAILED -eq 0 ]; then
    log "\n${GREEN}All tests passed!${NC}"
    exit 0
else
    log "\n${RED}Some tests failed!${NC}"
    exit 1
fi
