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

# Create object files: hello_lib.o (without main), simple_test.o (test framework)
# Then link with the test file
if gcc ${STRICT_FLAGS} -I. -c -o hello_lib.o hello.c -DUNIT_TEST 2>&1 && \
   gcc ${STRICT_FLAGS} -I. -c -o simple_test.o test/simple_test.c 2>&1 && \
   gcc ${STRICT_FLAGS} -I. -o test_hello_runner test/test_hello.c hello_lib.o simple_test.o 2>&1; then
    print_success "Unit tests compiled successfully"
    rm -f hello_lib.o simple_test.o
else
    print_error "Failed to compile unit tests"
    rm -f hello_lib.o simple_test.o
    exit 1
fi

print_info "Running unit tests..."
echo ""

# Run the tests
if ./test_hello_runner; then
    print_success "Unit tests completed successfully"
    exit 0
else
    print_error "Unit tests failed"
    exit 1
fi
