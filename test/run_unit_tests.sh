#!/bin/bash
# Unit test runner for hello.c
# Compiles and runs the unit tests using a lightweight testing framework

set -e  # Exit on any error
set -u  # Exit on unset variables
set -o pipefail  # Exit on pipeline failures

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

print_info "Building unit tests..."

# Compile the unit tests with strict flags
STRICT_FLAGS="-Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99"

# Create a temporary object file without main() by compiling with -c flag
# Then link with the test file
if gcc ${STRICT_FLAGS} -I. -c -o hello_lib.o hello.c -DUNIT_TEST 2>&1 && \
   gcc ${STRICT_FLAGS} -I. -o test_hello_runner test/test_hello.c hello_lib.o 2>&1; then
    print_success "Unit tests compiled successfully"
    rm -f hello_lib.o
else
    print_error "Failed to compile unit tests"
    rm -f hello_lib.o
    exit 1
fi

# Compile the test framework compile check
print_info "Building test framework compile check..."
if gcc ${STRICT_FLAGS} -I. -o test_framework_compile test/test_simple_test_compile.c 2>&1; then
    print_success "Test framework compile check built successfully"
else
    print_error "Failed to compile test framework compile check"
    exit 1
fi

print_info "Running unit tests..."
echo ""

# Run the main unit tests
if ./test_hello_runner; then
    print_success "Main unit tests completed successfully"
else
    print_error "Main unit tests failed"
    exit 1
fi

echo ""
print_info "Running test framework compile check..."
# Run the framework compile test
if ./test_framework_compile; then
    print_success "Test framework compile check passed"
    exit 0
else
    print_error "Test framework compile check failed"
    exit 1
fi
