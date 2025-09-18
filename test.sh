#!/bin/bash

# Automated Testing Script for Hello World C Program
# This script automates compilation, execution, and verification steps
# as outlined in the README.md for consistent testing of the program.

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counters
TESTS_RUN=0
TESTS_PASSED=0
TESTS_FAILED=0

# Expected output
EXPECTED_OUTPUT="Hello world!"

# Print colored output
print_status() {
    local color=$1
    local message=$2
    echo -e "${color}${message}${NC}"
}

# Print test header
print_test_header() {
    local test_name=$1
    echo
    print_status "$BLUE" "=== $test_name ==="
    TESTS_RUN=$((TESTS_RUN + 1))
}

# Check test result
check_result() {
    local result=$1
    local test_name=$2
    if [ $result -eq 0 ]; then
        print_status "$GREEN" "✓ $test_name PASSED"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        print_status "$RED" "✗ $test_name FAILED"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
}

# Verify program output and exit code
verify_execution() {
    local binary=$1
    local test_name=$2
    
    print_status "$YELLOW" "Running $binary..."
    
    # Capture output and exit code
    local output
    local exit_code
    output=$(./$binary 2>&1)
    exit_code=$?
    
    # Check exit code
    if [ $exit_code -ne 0 ]; then
        print_status "$RED" "✗ $test_name: Program exited with code $exit_code (expected 0)"
        return 1
    fi
    
    # Check output
    if [ "$output" != "$EXPECTED_OUTPUT" ]; then
        print_status "$RED" "✗ $test_name: Unexpected output"
        print_status "$RED" "  Expected: '$EXPECTED_OUTPUT'"
        print_status "$RED" "  Got:      '$output'"
        return 1
    fi
    
    print_status "$GREEN" "✓ $test_name: Output and exit code correct"
    return 0
}

# Clean previous builds
clean_builds() {
    print_status "$YELLOW" "Cleaning previous builds..."
    make clean > /dev/null 2>&1 || true
}

# Main testing function
main() {
    print_status "$BLUE" "Starting automated testing for Hello World C Program"
    print_status "$BLUE" "======================================================="
    
    # Clean environment
    clean_builds
    
    # Test 1: Standard build
    print_test_header "Standard Build Test"
    if make > /dev/null 2>&1; then
        verify_execution "hello" "Standard Build"
        check_result $? "Standard Build"
    else
        print_status "$RED" "✗ Standard Build: Compilation failed"
        check_result 1 "Standard Build"
    fi
    
    # Test 2: Development build with warnings
    print_test_header "Development Build Test (with warnings)"
    if make dev > /dev/null 2>&1; then
        verify_execution "hello_dev" "Development Build"
        check_result $? "Development Build"
    else
        print_status "$RED" "✗ Development Build: Compilation failed"
        check_result 1 "Development Build"
    fi
    
    # Test 3: Optimized build
    print_test_header "Optimized Build Test"
    if make optimized > /dev/null 2>&1; then
        verify_execution "hello_optimized" "Optimized Build"
        check_result $? "Optimized Build"
    else
        print_status "$RED" "✗ Optimized Build: Compilation failed"
        check_result 1 "Optimized Build"
    fi
    
    # Test 4: Strict compilation test
    print_test_header "Strict Compilation Test"
    if make strict > /dev/null 2>&1; then
        verify_execution "hello_strict" "Strict Compilation"
        check_result $? "Strict Compilation"
    else
        print_status "$RED" "✗ Strict Compilation: Compilation failed"
        check_result 1 "Strict Compilation"
    fi
    
    # Test 5: Debug build
    print_test_header "Debug Build Test"
    if make debug > /dev/null 2>&1; then
        verify_execution "hello_debug" "Debug Build"
        check_result $? "Debug Build"
    else
        print_status "$RED" "✗ Debug Build: Compilation failed"
        check_result 1 "Debug Build"
    fi
    
    # Test 6: Clang compilation (if available)
    print_test_header "Clang Compilation Test"
    if command -v clang > /dev/null 2>&1; then
        if clang -o hello_clang hello.c > /dev/null 2>&1; then
            verify_execution "hello_clang" "Clang Compilation"
            check_result $? "Clang Compilation"
        else
            print_status "$RED" "✗ Clang Compilation: Compilation failed"
            check_result 1 "Clang Compilation"
        fi
    else
        print_status "$YELLOW" "⚠ Clang not available, skipping Clang test"
    fi
    
    # Test 7: Makefile test target
    print_test_header "Makefile Test Target"
    clean_builds
    local test_output
    test_output=$(make test 2>&1)
    local make_test_result=$?
    
    if [ $make_test_result -eq 0 ] && echo "$test_output" | grep -q "$EXPECTED_OUTPUT"; then
        print_status "$GREEN" "✓ Makefile test target works correctly"
        check_result 0 "Makefile Test Target"
    else
        print_status "$RED" "✗ Makefile test target failed"
        check_result 1 "Makefile Test Target"
    fi
    
    # Final cleanup
    clean_builds
    
    # Print summary
    echo
    print_status "$BLUE" "======================================================="
    print_status "$BLUE" "Test Summary"
    print_status "$BLUE" "======================================================="
    echo "Tests Run:    $TESTS_RUN"
    print_status "$GREEN" "Tests Passed: $TESTS_PASSED"
    
    if [ $TESTS_FAILED -gt 0 ]; then
        print_status "$RED" "Tests Failed: $TESTS_FAILED"
        echo
        print_status "$RED" "❌ TESTING FAILED"
        exit 1
    else
        echo
        print_status "$GREEN" "✅ ALL TESTS PASSED"
        exit 0
    fi
}

# Handle script arguments
if [ "$1" = "--help" ] || [ "$1" = "-h" ]; then
    echo "Automated Testing Script for Hello World C Program"
    echo
    echo "Usage: $0 [--help|-h]"
    echo
    echo "This script performs comprehensive testing including:"
    echo "  - Standard compilation and execution"
    echo "  - Development build with warnings"
    echo "  - Optimized build testing"
    echo "  - Strict compilation with maximum warnings"
    echo "  - Debug build testing"
    echo "  - Clang compilation (if available)"
    echo "  - Makefile test target validation"
    echo "  - Output and exit code verification"
    echo
    echo "The script validates that all builds compile successfully,"
    echo "produce the expected output, and exit with code 0."
    exit 0
fi

# Run main function
main