#!/bin/bash
# Binary validation script for Hello World C program
# Tests that a pre-built binary produces expected output and exit code
# 
# Usage: ./test/validate-binary.sh <binary_path> [--quiet]
#        binary_path: Path to the binary to validate
#        --quiet: Reduce output verbosity for CI environments

set -e  # Exit on any error
set -u  # Exit on unset variables
set -o pipefail  # Exit on pipeline failures

# Parse command line arguments
if [[ ${#} -eq 0 ]]; then
    echo "Usage: $0 <binary_path> [--quiet]"
    exit 1
fi

BINARY_PATH="${1}"
QUIET_MODE=false
if [[ ${#} -gt 1 && "${2}" == "--quiet" ]]; then
    QUIET_MODE=true
fi

# Colors for output (disabled in quiet mode)
if [[ "${QUIET_MODE}" == "false" ]]; then
    RED='\033[0;31m'
    GREEN='\033[0;32m'
    YELLOW='\033[1;33m'
    NC='\033[0m' # No Color
else
    RED=''
    GREEN=''
    YELLOW=''
    NC=''
fi

# Expected output (with trailing newline)
EXPECTED_OUTPUT="Hello world!"$'\n'

# Function to print colored messages using safer printf formatting
print_success() {
    if [[ "${QUIET_MODE}" == "false" ]]; then
        printf '%b' "${GREEN}✓ $1${NC}\n"
    fi
}

print_error() {
    printf '%b' "${RED}✗ $1${NC}\n"
}

print_info() {
    if [[ "${QUIET_MODE}" == "false" ]]; then
        printf '%b' "${YELLOW}ℹ $1${NC}\n"
    fi
}

print_info "Starting validation of binary: ${BINARY_PATH}"

# Step 1: Check if binary exists and is executable
if [[ ! -f "${BINARY_PATH}" ]]; then
    print_error "Binary '${BINARY_PATH}' does not exist"
    exit 1
fi

if [[ ! -x "${BINARY_PATH}" ]]; then
    print_error "Binary '${BINARY_PATH}' is not executable"
    exit 1
fi

print_success "Binary exists and is executable"

# Step 2: Run the program and capture output and exit code
print_info "Running program and capturing output..."
# Temporarily disable set -e to capture exit code properly
set +e
# Use a method that preserves trailing whitespace by adding a sentinel
OUTPUT_WITH_SENTINEL=$("${BINARY_PATH}" 2>&1; printf x)
PROGRAM_EXIT_CODE=$?
set -e

# Remove the sentinel to get the exact output
OUTPUT="${OUTPUT_WITH_SENTINEL%x}"

# Step 3: Verify exit code
if [[ ${PROGRAM_EXIT_CODE} -ne 0 ]]; then
    print_error "Program exited with code ${PROGRAM_EXIT_CODE}, expected 0"
    printf "Program output:\n%s\n" "${OUTPUT}"
    exit 1
fi
print_success "Program exited with correct exit code (0)"

# Step 4: Verify exact output format (must match exactly: "Hello world!" with trailing newline)
if [[ "${OUTPUT}" != "${EXPECTED_OUTPUT}" ]]; then
    print_error "Output mismatch!"
    printf "Expected: '%s'\n" "${EXPECTED_OUTPUT}"
    printf "Actual:   '%s'\n" "${OUTPUT}"
    printf "Expected length: %d\n" "${#EXPECTED_OUTPUT}"
    printf "Actual length:   %d\n" "${#OUTPUT}"
    # Show hex dump for detailed analysis
    printf "Expected (hex): "
    printf '%s' "${EXPECTED_OUTPUT}" | hexdump -C | head -1
    printf "Actual (hex):   "
    printf '%s' "${OUTPUT}" | hexdump -C | head -1
    exit 1
fi
print_success "Output format is correct"

# Step 5: Explicit trailing newline check using byte-level analysis
# The program is expected to output a trailing newline
# Check if output ends with newline by examining the last character
if [[ -z "${OUTPUT}" || "${OUTPUT: -1}" != $'\n' ]]; then
    print_error "Output missing expected trailing newline"
    printf "Output should end with newline character (hex 0a)\n"
    printf "Raw output (hex): "
    printf '%s' "${OUTPUT}" | hexdump -C | head -1
    exit 1
fi
print_success "Trailing newline confirmed"

print_success "All validation checks passed!"
if [[ "${QUIET_MODE}" == "false" ]]; then
    printf "\n"
    print_info "Summary:"
    printf "  - Binary execution: PASSED\n"
    printf "  - Exit code (0): PASSED\n"
    printf "  - Output format: PASSED\n"
    printf "  - Trailing newline: PASSED\n"
fi

if [[ "${QUIET_MODE}" == "true" ]]; then
    printf "Binary validation: All tests PASSED\n"
fi