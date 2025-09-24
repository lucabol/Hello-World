#!/bin/bash
# Binary validation script for Hello World C program
# Tests that a pre-built binary produces expected output and exit code
# 
# Requirements: Basic Unix tools (standard on ubuntu-latest)
# - hexdump (preferred), xxd, or od for hex output (fallback provided)
# 
# Usage: ./test/validate-binary.sh <binary_path> [--quiet]
#        binary_path: Path to the binary to validate
#        --quiet: Reduce output verbosity for CI environments
#
# Note: This script expects text output only and validates byte-level
# output format including trailing newlines

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

# Colors for output (disabled in quiet mode, non-TTY, or NO_COLOR env var)
if [[ "${QUIET_MODE}" == "false" && -t 1 && -z "${NO_COLOR:-}" ]]; then
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

# Function to create hex dump with fallback options
create_hex_dump() {
    local file_path="$1"
    # Try hexdump first (most common), then xxd, then od as fallback
    # Using files directly avoids issues with printf and binary data
    if command -v hexdump >/dev/null 2>&1; then
        hexdump -C "${file_path}" | head -n 3
    elif command -v xxd >/dev/null 2>&1; then
        xxd "${file_path}" | head -n 3
    elif command -v od >/dev/null 2>&1; then
        od -c "${file_path}" | head -n 3
    else
        printf "Raw bytes (length: %d): " "$(wc -c < "${file_path}")"
        cat -v "${file_path}"
        printf "\n(Install hexdump/xxd/od for detailed hex output)\n"
    fi
}

# Cleanup function to remove temp files
cleanup() {
    if [[ -n "${OUTPUT_FILE:-}" && -f "${OUTPUT_FILE}" ]]; then
        rm -f "${OUTPUT_FILE}"
    fi
    if [[ -n "${EXPECTED_FILE:-}" && -f "${EXPECTED_FILE}" ]]; then
        rm -f "${EXPECTED_FILE}"
    fi
}

# Set up cleanup trap
trap cleanup EXIT

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

# Step 2: Run the program and capture output and exit code using temp file
print_info "Running program and capturing output..."
# Create temporary file for output capture (preserves all bytes including trailing whitespace)
OUTPUT_FILE=$(mktemp)
"${BINARY_PATH}" >"${OUTPUT_FILE}" 2>&1
PROGRAM_EXIT_CODE=$?

# Read output from temp file preserving trailing whitespace using a different method
# We cannot use OUTPUT=$(<"${OUTPUT_FILE}") as it strips trailing newlines
# Instead, we'll add a sentinel and remove it
printf "x" >> "${OUTPUT_FILE}"
OUTPUT_WITH_SENTINEL=$(<"${OUTPUT_FILE}")
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
    # Show hex dump for detailed analysis using temp files
    EXPECTED_FILE=$(mktemp)
    printf '%s' "${EXPECTED_OUTPUT}" > "${EXPECTED_FILE}"
    printf "Expected (hex): "
    create_hex_dump "${EXPECTED_FILE}"
    printf "Actual (hex):   "
    create_hex_dump "${OUTPUT_FILE}"
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
    create_hex_dump "${OUTPUT_FILE}"
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
