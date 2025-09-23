#!/bin/bash
# Validation script for Hello World C program
# Tests that the program builds correctly and produces expected output

set -e  # Exit on any error
set -o pipefail  # Exit on pipeline failures

# Compiler configuration - use same CC as Makefile, default to gcc
CC=${CC:-gcc}

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Expected output (without trailing newline)
EXPECTED_OUTPUT="Hello world!"

# Function to print colored messages using safer printf formatting
print_success() {
    printf '%b' "${GREEN}✓ $1${NC}\n"
}

print_error() {
    printf '%b' "${RED}✗ $1${NC}\n"
}

print_info() {
    printf '%b' "${YELLOW}ℹ $1${NC}\n"
}

# Clean up function
cleanup() {
    if [[ -f hello_strict ]]; then
        rm -f hello_strict
    fi
}

# Set up cleanup trap
trap cleanup EXIT

print_info "Starting validation of Hello World program..."

# Step 1: Build with strict flags  
print_info "Building with strict compilation flags..."
# Capture both stdout and stderr for better diagnostics
if COMPILE_OUTPUT=$("$CC" -Wall -Wextra -std=c99 -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -o hello_strict hello.c 2>&1); then
    print_success "Strict compilation passed"
else
    COMPILE_EXIT_CODE=$?
    print_error "Strict compilation failed (exit code: $COMPILE_EXIT_CODE)"
    printf "Compiler output:\n%s\n" "$COMPILE_OUTPUT"
    exit 1
fi

# Step 2: Check if binary exists
if [[ ! -f hello_strict ]]; then
    print_error "Binary hello_strict was not created"
    exit 1
fi

# Step 3: Run the program and capture output and exit code
print_info "Running program and capturing output..."
# Temporarily disable set -e to capture exit code properly
set +e
# Use a method that preserves trailing whitespace by adding a sentinel
OUTPUT_WITH_SENTINEL=$(./hello_strict 2>&1; printf x)
PROGRAM_EXIT_CODE=$?
set -e

# Remove the sentinel to get the exact output
OUTPUT="${OUTPUT_WITH_SENTINEL%x}"

# Step 4: Verify exit code
if [[ $PROGRAM_EXIT_CODE -ne 0 ]]; then
    print_error "Program exited with code $PROGRAM_EXIT_CODE, expected 0"
    printf "Program output:\n%s\n" "$OUTPUT"
    exit 1
fi
print_success "Program exited with correct exit code (0)"

# Step 5: Verify exact output format
if [[ "$OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
    print_error "Output mismatch!"
    printf "Expected: '%s'\n" "$EXPECTED_OUTPUT"
    printf "Actual:   '%s'\n" "$OUTPUT"
    printf "Expected length: %d\n" "${#EXPECTED_OUTPUT}"
    printf "Actual length:   %d\n" "${#OUTPUT}"
    # Show hex dump for detailed analysis
    printf "Expected (hex): "
    printf '%s' "$EXPECTED_OUTPUT" | hexdump -C | head -1
    printf "Actual (hex):   "
    printf '%s' "$OUTPUT" | hexdump -C | head -1
    exit 1
fi
print_success "Output format is correct"

# Step 6: Explicit trailing newline check using byte-level analysis
# Check if output ends with newline by examining the last character
if [[ -n "$OUTPUT" && "${OUTPUT: -1}" == $'\n' ]]; then
    print_error "Output contains unexpected trailing newline"
    printf "Output ends with newline character (hex 0a)\n"
    printf "Raw output (hex): "
    printf '%s' "$OUTPUT" | hexdump -C | head -1
    exit 1
fi
print_success "No trailing newline confirmed"

print_success "All validation checks passed!"
printf "\n"
print_info "Summary:"
printf "  - Strict compilation: PASSED\n"
printf "  - Exit code (0): PASSED\n"
printf "  - Output format: PASSED\n"
printf "  - No trailing newline: PASSED\n"