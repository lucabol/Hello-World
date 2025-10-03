#!/bin/bash
# Test script for metrics analyzer
# Tests various edge cases and validates output

set -e  # Exit on error
set -u  # Exit on undefined variable

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

TESTS_PASSED=0
TESTS_FAILED=0

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
}

print_info() {
    echo -e "${YELLOW}ℹ $1${NC}"
}

# Function to run a test
run_test() {
    local test_name="$1"
    local command="$2"
    local expected_exit_code="${3:-0}"
    
    print_info "Running: $test_name"
    
    if eval "$command" > /tmp/test_output.txt 2>&1; then
        actual_exit_code=0
    else
        actual_exit_code=$?
    fi
    
    if [ $actual_exit_code -eq $expected_exit_code ]; then
        print_success "$test_name"
        TESTS_PASSED=$((TESTS_PASSED + 1))
        return 0
    else
        print_error "$test_name (exit code: expected $expected_exit_code, got $actual_exit_code)"
        cat /tmp/test_output.txt
        TESTS_FAILED=$((TESTS_FAILED + 1))
        return 1
    fi
}

# Function to check if output contains expected text
check_output_contains() {
    local test_name="$1"
    local command="$2"
    local expected_text="$3"
    
    print_info "Checking: $test_name"
    
    if eval "$command" 2>&1 | grep -q "$expected_text"; then
        print_success "$test_name"
        TESTS_PASSED=$((TESTS_PASSED + 1))
        return 0
    else
        print_error "$test_name (output doesn't contain: $expected_text)"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        return 1
    fi
}

# Function to check metrics values
check_metric_value() {
    local test_name="$1"
    local file="$2"
    local metric_name="$3"
    local expected_value="$4"
    
    print_info "Checking: $test_name"
    
    local output=$(./metrics "$file" 2>&1)
    # Extract value from table row: "| Metric Name | Value |"
    local actual_value=$(echo "$output" | grep "| $metric_name" | awk -F'|' '{print $3}' | tr -d ' ')
    
    if [ "$actual_value" = "$expected_value" ]; then
        print_success "$test_name (value: $actual_value)"
        TESTS_PASSED=$((TESTS_PASSED + 1))
        return 0
    else
        print_error "$test_name (expected: $expected_value, got: '$actual_value')"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        return 1
    fi
}

echo "================================================================================"
echo "                    METRICS ANALYZER TEST SUITE"
echo "================================================================================"
echo ""

# Build the metrics tool
print_info "Building metrics analyzer with strict flags..."
if gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -o metrics metrics.c 2>&1; then
    print_success "Build successful with strict flags"
else
    print_error "Build failed"
    exit 1
fi

echo ""
echo "--- Basic Functionality Tests ---"

# Test 1: Basic execution with hello.c
run_test "Execute metrics on hello.c" "./metrics hello.c"

# Test 2: Help option
run_test "Help option works" "./metrics --help"

# Test 3: Line analysis option
run_test "Line analysis option works" "./metrics --lines hello.c"

# Test 4: CSV export option
run_test "CSV export option works" "./metrics --csv hello.c"

# Test 5: Non-existent file (should fail)
run_test "Non-existent file returns error" "./metrics nonexistent.c" 1

echo ""
echo "--- Edge Case Tests ---"

# Test 6: File with only blank lines
check_metric_value "Blank lines file" "test/metrics_tests/blank_lines.c" "Blank Lines" "5"

# Test 7: File with only comments
check_metric_value "Comments only file" "test/metrics_tests/only_comments.c" "Comment Lines" "4"

# Test 8: File with semicolons in strings
run_test "Semicolons in strings" "./metrics test/metrics_tests/semicolons_in_strings.c"

# Test 9: File without trailing newline
run_test "No trailing newline" "./metrics test/metrics_tests/no_trailing_newline.c"

echo ""
echo "--- CSV Export Tests ---"

# Test 10: CSV file is created
print_info "Checking: CSV file creation"
./metrics --csv hello.c > /dev/null 2>&1
if [ -f "hello.c_metrics.csv" ]; then
    print_success "CSV file created"
    TESTS_PASSED=$((TESTS_PASSED + 1))
    
    # Test 11: CSV has proper header
    if head -1 hello.c_metrics.csv | grep -q "Metric,Value"; then
        print_success "CSV has correct header"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        print_error "CSV header incorrect"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
    
    # Test 12: CSV contains line data
    if grep -q "Line,Type,Chars" hello.c_metrics.csv; then
        print_success "CSV contains line analysis data"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        print_error "CSV missing line analysis data"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
    
    rm -f hello.c_metrics.csv
else
    print_error "CSV file not created"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi

# Test 13: CSV escaping with quotes and commas
print_info "Checking: CSV escaping"
./metrics --csv test/metrics_tests/csv_escape_test.c > /dev/null 2>&1
if [ -f "test/metrics_tests/csv_escape_test.c_metrics.csv" ]; then
    # Check that quotes in content are properly escaped
    if grep -q '""' test/metrics_tests/csv_escape_test.c_metrics.csv; then
        print_success "CSV properly escapes quotes"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        print_error "CSV quote escaping may be incorrect"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
    rm -f test/metrics_tests/csv_escape_test.c_metrics.csv
else
    print_error "CSV file not created for escape test"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi

echo ""
echo "--- Output Format Tests ---"

# Test 14: Summary table format
check_output_contains "Summary table present" "./metrics hello.c" "CODE METRICS ANALYSIS SPREADSHEET"

# Test 15: Metrics table borders
check_output_contains "Table borders present" "./metrics hello.c" "+---"

# Test 16: Line analysis table
check_output_contains "Line analysis table" "./metrics --lines hello.c" "LINE-BY-LINE ANALYSIS"

echo ""
echo "--- Value Accuracy Tests ---"

# Test hello.c metrics (we know the exact values)
check_metric_value "hello.c total lines" "hello.c" "Total Lines" "5"
check_metric_value "hello.c code lines" "hello.c" "Code Lines" "4"
check_metric_value "hello.c blank lines" "hello.c" "Blank Lines" "1"
check_metric_value "hello.c includes" "hello.c" "Include Directives" "1"

echo ""
echo "================================================================================"
echo "                          TEST SUMMARY"
echo "================================================================================"
echo "Tests Passed: $TESTS_PASSED"
echo "Tests Failed: $TESTS_FAILED"
echo ""

if [ $TESTS_FAILED -eq 0 ]; then
    print_success "ALL TESTS PASSED!"
    exit 0
else
    print_error "SOME TESTS FAILED"
    exit 1
fi
