#!/bin/bash
# Automated test script for Hello-World C project
# Builds with GCC and Clang, verifies output and exit codes, reports binary sizes

set -e  # Exit on any error

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Expected output from the program
EXPECTED_OUTPUT="Hello world!"

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[PASS]${NC} $1"
}

print_error() {
    echo -e "${RED}[FAIL]${NC} $1"
}

# Function to test a binary
test_binary() {
    local binary_name="$1"
    local compiler="$2"
    
    print_status "Testing $binary_name (built with $compiler)..."
    
    # Run the binary and capture output and exit code
    local output
    local exit_code
    output=$(./tests/$binary_name 2>&1) || exit_code=$?
    exit_code=${exit_code:-0}
    
    # Check exit code
    if [ $exit_code -eq 0 ]; then
        print_success "Exit code is 0"
    else
        print_error "Exit code is $exit_code, expected 0"
        return 1
    fi
    
    # Check output
    if [ "$output" = "$EXPECTED_OUTPUT" ]; then
        print_success "Output matches expected: '$EXPECTED_OUTPUT'"
    else
        print_error "Output mismatch. Expected: '$EXPECTED_OUTPUT', Got: '$output'"
        return 1
    fi
    
    # Report binary size
    local size_info
    size_info=$(ls -lh tests/$binary_name | awk '{print $5}')
    local size_bytes
    size_bytes=$(stat -c%s tests/$binary_name)
    print_status "Binary size: $size_info ($size_bytes bytes)"
    
    return 0
}

# Main execution
echo "================================================"
echo "Hello-World C Project - Automated Test Suite"
echo "================================================"

# Clean any existing test binaries
rm -f tests/hello_gcc tests/hello_clang

print_status "Building with GCC..."
gcc -Wall -Wextra -O2 -o tests/hello_gcc hello.c
if [ $? -eq 0 ]; then
    print_success "GCC build successful"
else
    print_error "GCC build failed"
    exit 1
fi

print_status "Building with Clang..."
clang -Wall -Wextra -O2 -o tests/hello_clang hello.c
if [ $? -eq 0 ]; then
    print_success "Clang build successful"
else
    print_error "Clang build failed"
    exit 1
fi

echo ""
echo "Running tests..."
echo "=================="

# Test both binaries
test_binary "hello_gcc" "GCC"
echo ""
test_binary "hello_clang" "Clang"

echo ""
echo "================================================"
print_success "All tests passed!"

# Summary of binary sizes
echo ""
echo "Binary Size Summary:"
echo "===================="
ls -lh tests/hello_gcc tests/hello_clang | awk '{print $9 ": " $5 " (" $5 " bytes)"}'

# Optional: Check for size regression (basic threshold)
gcc_size=$(stat -c%s tests/hello_gcc)
clang_size=$(stat -c%s tests/hello_clang)

# Basic size regression check - warn if binaries are unusually large (>50KB)
if [ $gcc_size -gt 51200 ] || [ $clang_size -gt 51200 ]; then
    print_status "Warning: Binary size is larger than expected (>50KB)"
    print_status "GCC: $gcc_size bytes, Clang: $clang_size bytes"
fi

echo ""
print_success "Test suite completed successfully!"