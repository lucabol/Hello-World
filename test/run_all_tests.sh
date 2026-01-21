#!/bin/bash
# Master test runner - runs all comprehensive test suites
# This script executes all edge case and boundary condition tests

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT"

echo "=========================================="
echo "  Comprehensive Test Suite for hello.c"
echo "=========================================="
echo ""

# Track test results
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

run_test_suite() {
    local test_script=$1
    local test_name=$2
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo "Running: $test_name"
    echo "----------------------------------------"
    
    if bash "$test_script"; then
        PASSED_TESTS=$((PASSED_TESTS + 1))
        echo "✓ $test_name PASSED"
    else
        FAILED_TESTS=$((FAILED_TESTS + 1))
        echo "✗ $test_name FAILED"
        return 1
    fi
    
    echo ""
}

# Clean up before starting
rm -f hello hello_test hello_debug hello_clang *.o *.exe *.out
rm -f test_output.txt output.txt stdout.txt stderr.txt

# Run all test suites
run_test_suite "$SCRIPT_DIR/validate.sh" "Basic Validation Tests"
run_test_suite "$SCRIPT_DIR/test_optimization_levels.sh" "Optimization Level Tests"
run_test_suite "$SCRIPT_DIR/test_locale_settings.sh" "Locale Settings Tests"
run_test_suite "$SCRIPT_DIR/test_output_streams.sh" "Output Stream Redirection Tests"
run_test_suite "$SCRIPT_DIR/test_buffer_flushing.sh" "Buffer Flushing Tests"
run_test_suite "$SCRIPT_DIR/test_warning_flags.sh" "Warning Flags Tests"
run_test_suite "$SCRIPT_DIR/test_minimal_environment.sh" "Minimal Environment Tests"

# Clean up after tests
rm -f hello hello_test hello_debug hello_clang *.o *.exe *.out
rm -f test_output.txt output.txt stdout.txt stderr.txt

# Print summary
echo "=========================================="
echo "  Test Summary"
echo "=========================================="
echo "Total test suites: $TOTAL_TESTS"
echo "Passed: $PASSED_TESTS"
echo "Failed: $FAILED_TESTS"
echo ""

if [[ $FAILED_TESTS -eq 0 ]]; then
    echo "✓ All test suites passed!"
    exit 0
else
    echo "✗ Some test suites failed!"
    exit 1
fi
