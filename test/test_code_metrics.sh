#!/bin/bash
# Test script for code_metrics analyzer
# Validates that the tool builds and produces expected output

set -e  # Exit on any error
set -u  # Exit on unset variables
set -o pipefail  # Exit on pipeline failures

# Set locale for hermetic testing and Unicode support
export LANG=C.UTF-8
export LC_ALL=C.UTF-8

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Print colored messages
print_info() {
    printf '%b' "${YELLOW}ℹ $1${NC}\n"
}

print_success() {
    printf '%b' "${GREEN}✓ $1${NC}\n"
}

print_error() {
    printf '%b' "${RED}✗ $1${NC}\n"
}

# Change to repository root
cd "$(dirname "$0")/.."

print_info "Testing Code Metrics Analyzer..."
echo ""

# Test 1: Build the code_metrics tool
print_info "Test 1: Building code_metrics..."
if gcc -Wall -Wextra -o code_metrics code_metrics.c 2>&1; then
    print_success "code_metrics compiled successfully"
else
    print_error "Failed to compile code_metrics"
    exit 1
fi

# Test 2: Run with default argument (hello.c)
print_info "Test 2: Running with default argument (hello.c)..."
if OUTPUT=$(./code_metrics 2>&1); then
    if echo "${OUTPUT}" | grep -q "CODE METRICS ANALYZER" && \
       echo "${OUTPUT}" | grep -q "hello.c" && \
       echo "${OUTPUT}" | grep -q "Total Lines" && \
       echo "${OUTPUT}" | grep -q "Code Lines"; then
        print_success "Default execution works correctly"
    else
        print_error "Output missing expected content"
        echo "Output: ${OUTPUT}"
        exit 1
    fi
else
    print_error "Failed to run code_metrics with default argument"
    exit 1
fi

# Test 3: Run with explicit file argument
print_info "Test 3: Running with explicit file argument..."
if OUTPUT=$(./code_metrics hello.c 2>&1); then
    if echo "${OUTPUT}" | grep -q "hello.c" && \
       echo "${OUTPUT}" | grep -q "Total Lines"; then
        print_success "Explicit file argument works correctly"
    else
        print_error "Output missing expected content"
        exit 1
    fi
else
    print_error "Failed to run code_metrics with explicit argument"
    exit 1
fi

# Test 4: Run help option
print_info "Test 4: Testing help option..."
if OUTPUT=$(./code_metrics --help 2>&1); then
    if echo "${OUTPUT}" | grep -q "Usage:" && \
       echo "${OUTPUT}" | grep -q "Displays code metrics"; then
        print_success "Help option works correctly"
    else
        print_error "Help output missing expected content"
        exit 1
    fi
else
    print_error "Failed to run code_metrics --help"
    exit 1
fi

# Test 5: Verify metrics accuracy for hello.c
print_info "Test 5: Verifying metrics accuracy for hello.c..."
OUTPUT=$(./code_metrics hello.c)

# Extract the total lines count (should be 5 for hello.c)
TOTAL_LINES=$(echo "${OUTPUT}" | grep "Total Lines" | grep -o '[0-9]\+' | head -1)
if [ "${TOTAL_LINES}" -eq 5 ]; then
    print_success "Total lines count is correct (5)"
else
    print_error "Total lines count is incorrect. Expected 5, got ${TOTAL_LINES}"
    exit 1
fi

# Verify function count (should be 1 for hello.c with main())
FUNCTION_COUNT=$(echo "${OUTPUT}" | grep "Function Definitions" | grep -o '[0-9]\+' | head -1)
if [ "${FUNCTION_COUNT}" -eq 1 ]; then
    print_success "Function count is correct (1)"
else
    print_error "Function count is incorrect. Expected 1, got ${FUNCTION_COUNT}"
    exit 1
fi

# Test 6: Test with non-existent file (should fail gracefully)
print_info "Test 6: Testing error handling with non-existent file..."
set +e  # Temporarily disable exit on error
OUTPUT=$(./code_metrics nonexistent.c 2>&1)
EXIT_CODE=$?
set -e  # Re-enable exit on error
if [ ${EXIT_CODE} -ne 0 ] && echo "${OUTPUT}" | grep -q "Cannot open file"; then
    print_success "Error handling works correctly"
else
    print_error "Error handling failed (exit code: ${EXIT_CODE})"
    exit 1
fi

# Test 7: Analyze code_metrics.c itself (self-analysis)
print_info "Test 7: Self-analysis test (analyze code_metrics.c)..."
if OUTPUT=$(./code_metrics code_metrics.c 2>&1); then
    if echo "${OUTPUT}" | grep -q "code_metrics.c" && \
       echo "${OUTPUT}" | grep -q "Total Lines"; then
        # Verify that it detects multiple functions (at least 5)
        FUNCTIONS=$(echo "${OUTPUT}" | grep "Function Definitions" | grep -o '[0-9]\+' | head -1)
        if [ "${FUNCTIONS}" -ge 5 ]; then
            print_success "Self-analysis works correctly (${FUNCTIONS} functions detected)"
        else
            print_error "Self-analysis detected too few functions: ${FUNCTIONS}"
            exit 1
        fi
    else
        print_error "Self-analysis output missing expected content"
        exit 1
    fi
else
    print_error "Failed to run self-analysis"
    exit 1
fi

# Test 8: Test --version flag
print_info "Test 8: Testing --version flag..."
if OUTPUT=$(./code_metrics --version 2>&1); then
    if echo "${OUTPUT}" | grep -q "version" && \
       echo "${OUTPUT}" | grep -q "1.0.0"; then
        print_success "Version flag works correctly"
    else
        print_error "Version output missing expected content"
        exit 1
    fi
else
    print_error "Failed to run code_metrics --version"
    exit 1
fi

# Test 9: Test --plain/ASCII output mode
print_info "Test 9: Testing --plain/ASCII output mode..."
if OUTPUT=$(./code_metrics --plain hello.c 2>&1); then
    if echo "${OUTPUT}" | grep -q "CODE METRICS ANALYZER" && \
       echo "${OUTPUT}" | grep -q "+" && \
       ! echo "${OUTPUT}" | grep -q "║"; then
        print_success "ASCII output mode works correctly"
    else
        print_error "ASCII output missing expected format"
        exit 1
    fi
else
    print_error "Failed to run code_metrics with --plain"
    exit 1
fi

# Test 10: Test invalid option handling
print_info "Test 10: Testing invalid option handling..."
set +e
OUTPUT=$(./code_metrics --invalid-option 2>&1)
EXIT_CODE=$?
set -e
if [ ${EXIT_CODE} -ne 0 ] && echo "${OUTPUT}" | grep -q "Unknown option"; then
    print_success "Invalid option handling works correctly"
else
    print_error "Invalid option handling failed"
    exit 1
fi

# Test 11: Test edge cases file analysis
print_info "Test 11: Testing edge cases file analysis..."
if OUTPUT=$(./code_metrics test/test_edge_cases.c 2>&1); then
    if echo "${OUTPUT}" | grep -q "test_edge_cases.c" && \
       echo "${OUTPUT}" | grep -q "Total Lines"; then
        # Verify it detects multiple functions (should be 6)
        FUNCTIONS=$(echo "${OUTPUT}" | grep "Function Definitions" | grep -o '[0-9]\+' | head -1)
        if [ "${FUNCTIONS}" -ge 5 ]; then
            print_success "Edge cases analysis works correctly (${FUNCTIONS} functions detected)"
        else
            print_error "Edge cases analysis detected incorrect functions: ${FUNCTIONS}"
            exit 1
        fi
    else
        print_error "Edge cases output missing expected content"
        exit 1
    fi
else
    print_error "Failed to analyze edge cases file"
    exit 1
fi

echo ""
print_success "All code_metrics tests passed!"
echo ""
print_info "Summary:"
printf "  - Compilation: PASSED\n"
printf "  - Default execution: PASSED\n"
printf "  - Explicit file argument: PASSED\n"
printf "  - Help option: PASSED\n"
printf "  - Metrics accuracy: PASSED\n"
printf "  - Error handling: PASSED\n"
printf "  - Self-analysis: PASSED\n"
printf "  - Version flag: PASSED\n"
printf "  - ASCII output mode: PASSED\n"
printf "  - Invalid option handling: PASSED\n"
printf "  - Edge cases analysis: PASSED\n"

exit 0
