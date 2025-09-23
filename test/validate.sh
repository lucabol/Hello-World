#!/bin/bash
# Validation script for Hello World C program
# Tests that the program builds correctly and produces expected output

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Expected output (without trailing newline)
EXPECTED_OUTPUT="Hello world!"

# Function to print colored messages
print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
}

print_info() {
    echo -e "${YELLOW}ℹ $1${NC}"
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
if gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -o hello_strict hello.c; then
    print_success "Strict compilation passed"
else
    print_error "Strict compilation failed"
    exit 1
fi

# Step 2: Check if binary exists
if [[ ! -f hello_strict ]]; then
    print_error "Binary hello_strict was not created"
    exit 1
fi

# Step 3: Run the program and capture output and exit code
print_info "Running program and capturing output..."
# Use a more robust method to capture output that preserves trailing whitespace
OUTPUT_WITH_NEWLINE=$(./hello_strict 2>&1; echo x)
OUTPUT="${OUTPUT_WITH_NEWLINE%x}"
EXIT_CODE=$?

# Step 4: Verify exit code
if [[ $EXIT_CODE -ne 0 ]]; then
    print_error "Program exited with code $EXIT_CODE, expected 0"
    exit 1
fi
print_success "Program exited with correct exit code (0)"

# Step 5: Verify output format
if [[ "$OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
    print_error "Output mismatch!"
    echo "Expected: '$EXPECTED_OUTPUT'"
    echo "Actual:   '$OUTPUT'"
    echo "Expected length: ${#EXPECTED_OUTPUT}"
    echo "Actual length:   ${#OUTPUT}"
    exit 1
fi
print_success "Output format is correct"

# Step 6: Additional checks - verify no trailing newline
if [[ "$OUTPUT" == *$'\n' ]]; then
    print_error "Output contains unexpected trailing newline"
    exit 1
fi
print_success "No trailing newline confirmed"

print_success "All validation checks passed!"
echo ""
print_info "Summary:"
echo "  - Strict compilation: PASSED"
echo "  - Exit code (0): PASSED"
echo "  - Output format: PASSED"
echo "  - No trailing newline: PASSED"