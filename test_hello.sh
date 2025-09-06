#!/bin/bash

# Automated Unit Tests for hello.c
# This script tests the hello.c program functionality across different builds

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test counters
TESTS_RUN=0
TESTS_PASSED=0
TESTS_FAILED=0

# Function to print test status
print_test_result() {
    local test_name="$1"
    local result="$2"
    
    TESTS_RUN=$((TESTS_RUN + 1))
    
    if [ "$result" = "PASS" ]; then
        echo -e "[${GREEN}PASS${NC}] $test_name"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "[${RED}FAIL${NC}] $test_name"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
}

# Function to run a single test
run_test() {
    local test_name="$1"
    local binary_name="$2"
    local expected_output="$3"
    local expected_exit_code="$4"
    
    if [ ! -f "$binary_name" ]; then
        print_test_result "$test_name - Binary exists" "FAIL"
        return 1
    fi
    
    # Run the program and capture output and exit code
    local actual_output
    local actual_exit_code
    
    actual_output=$(./"$binary_name" 2>&1)
    actual_exit_code=$?
    
    # Test output
    if [ "$actual_output" = "$expected_output" ]; then
        print_test_result "$test_name - Output correct" "PASS"
    else
        print_test_result "$test_name - Output correct (expected: '$expected_output', got: '$actual_output')" "FAIL"
    fi
    
    # Test exit code
    if [ "$actual_exit_code" -eq "$expected_exit_code" ]; then
        print_test_result "$test_name - Exit code correct" "PASS"
    else
        print_test_result "$test_name - Exit code correct (expected: $expected_exit_code, got: $actual_exit_code)" "FAIL"
    fi
}

# Function to compile and test
compile_and_test() {
    local test_name="$1"
    local compiler="$2"
    local flags="$3"
    local binary_name="$4"
    
    echo -e "\n${YELLOW}=== $test_name ===${NC}"
    
    # Clean up previous binary
    rm -f "$binary_name"
    
    # Compile
    if $compiler $flags -o "$binary_name" hello.c 2>&1; then
        print_test_result "$test_name - Compilation" "PASS"
        
        # Run the test
        run_test "$test_name" "$binary_name" "Ciao, Mondo!" 0
    else
        print_test_result "$test_name - Compilation" "FAIL"
    fi
    
    # Clean up
    rm -f "$binary_name"
}

# Main test execution
echo "Starting automated tests for hello.c"
echo "======================================"

# Check if hello.c exists
if [ ! -f "hello.c" ]; then
    echo -e "${RED}ERROR: hello.c not found!${NC}"
    exit 1
fi

# Test 1: Basic GCC compilation
compile_and_test "Basic GCC Build" "gcc" "" "hello_test1"

# Test 2: GCC with warnings
compile_and_test "GCC with Warnings" "gcc" "-Wall -Wextra" "hello_test2"

# Test 3: GCC debug build
compile_and_test "GCC Debug Build" "gcc" "-g -Wall -Wextra" "hello_test3"

# Test 4: GCC optimized build
compile_and_test "GCC Optimized Build" "gcc" "-O2 -Wall -Wextra" "hello_test4"

# Test 5: GCC strict compilation
compile_and_test "GCC Strict Build" "gcc" "-Wall -Wextra -Wpedantic -Wformat=2" "hello_test5"

# Test 6: Clang compilation (if available)
if command -v clang >/dev/null 2>&1; then
    compile_and_test "Clang Build" "clang" "" "hello_test6"
    compile_and_test "Clang with Warnings" "clang" "-Wall -Wextra" "hello_test7"
else
    echo -e "\n${YELLOW}=== Clang Build ===${NC}"
    echo -e "[${YELLOW}SKIP${NC}] Clang not available"
fi

# Final results
echo ""
echo "======================================"
echo "Test Results Summary:"
echo "  Tests Run:    $TESTS_RUN"
echo "  Tests Passed: $TESTS_PASSED"
echo "  Tests Failed: $TESTS_FAILED"

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed!${NC}"
    exit 1
fi