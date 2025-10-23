#!/bin/bash
# Test script for code_metrics analyzer
# Runs code_metrics on test fixtures and validates output

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

TESTS_PASSED=0
TESTS_FAILED=0
TESTS_DIR="$(cd "$(dirname "$0")" && pwd)"
FIXTURES_DIR="$TESTS_DIR/fixtures"
BINARY="$(cd "$TESTS_DIR/.." && pwd)/code_metrics"

# Check if code_metrics binary exists
if [ ! -f "$BINARY" ]; then
    echo -e "${RED}Error: code_metrics binary not found at $BINARY${NC}"
    echo "Please build it first with: make code_metrics"
    exit 1
fi

echo "Running code_metrics tests..."
echo "========================================"
echo

# Helper function to extract metric from output
extract_metric() {
    local output="$1"
    local metric="$2"
    
    case "$metric" in
        "total_lines")
            echo "$output" | grep "Total Lines:" | sed 's/.*Total Lines:[^0-9]*\([0-9]*\).*/\1/'
            ;;
        "code_lines")
            echo "$output" | grep "Code Lines:" | sed 's/.*Code Lines:[^0-9]*\([0-9]*\).*/\1/'
            ;;
        "blank_lines")
            echo "$output" | grep "Blank Lines:" | sed 's/.*Blank Lines:[^0-9]*\([0-9]*\).*/\1/'
            ;;
        "comment_lines")
            echo "$output" | grep "Comment Lines:" | sed 's/.*Comment Lines:[^0-9]*\([0-9]*\).*/\1/'
            ;;
        "function_count")
            echo "$output" | grep "Function Count:" | sed 's/.*Function Count:[^0-9]*\([0-9]*\).*/\1/'
            ;;
        "include_count")
            echo "$output" | grep "Include Count:" | sed 's/.*Include Count:[^0-9]*\([0-9]*\).*/\1/'
            ;;
    esac
}

# Run test on a fixture file
run_test() {
    local fixture="$1"
    local expected_file="$2"
    local test_name=$(basename "$fixture" .c)
    
    echo -n "Testing $test_name... "
    
    # Run code_metrics
    local output=$($BINARY "$fixture" 2>&1)
    local exit_code=$?
    
    if [ $exit_code -ne 0 ]; then
        echo -e "${RED}FAILED${NC}"
        echo "  code_metrics exited with code $exit_code"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        return 1
    fi
    
    # If expected file doesn't exist, just report success (manual verification needed)
    if [ ! -f "$expected_file" ]; then
        echo -e "${YELLOW}SKIP${NC} (no expected output file)"
        return 0
    fi
    
    # Check each metric
    local failed=0
    while IFS=: read -r metric expected_value; do
        local actual_value=$(extract_metric "$output" "$metric")
        if [ "$actual_value" != "$expected_value" ]; then
            if [ $failed -eq 0 ]; then
                echo -e "${RED}FAILED${NC}"
                failed=1
            fi
            echo "  $metric: expected $expected_value, got $actual_value"
        fi
    done < "$expected_file"
    
    if [ $failed -eq 0 ]; then
        echo -e "${GREEN}PASSED${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
}

# Run tests on all fixtures
for fixture in "$FIXTURES_DIR"/*.c; do
    if [ -f "$fixture" ]; then
        expected="${fixture%.c}.expected"
        run_test "$fixture" "$expected"
    fi
done

echo
echo "========================================"
echo "Test Results:"
echo "  Passed: $TESTS_PASSED"
echo "  Failed: $TESTS_FAILED"
echo

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed!${NC}"
    exit 1
fi
