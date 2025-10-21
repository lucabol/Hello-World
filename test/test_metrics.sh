#!/bin/bash
# Unit test runner for metrics analysis
# Compiles and runs the metrics unit tests

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

print_info "Building metrics unit tests..."

# Compile the metrics tests with strict flags
STRICT_FLAGS="-Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99"
POSIX_FLAGS="-D_POSIX_C_SOURCE=200809L"

# Compile metrics.c and test_metrics.c separately, then link
if gcc ${POSIX_FLAGS} ${STRICT_FLAGS} -I. -c -o metrics_test.o metrics.c 2>&1 && \
   gcc ${POSIX_FLAGS} ${STRICT_FLAGS} -I. -o test/test_metrics_runner test/test_metrics.c metrics_test.o 2>&1; then
    print_success "Metrics unit tests compiled successfully"
    rm -f metrics_test.o
else
    print_error "Failed to compile metrics unit tests"
    rm -f metrics_test.o
    exit 1
fi

print_info "Running metrics unit tests..."
echo ""

# Run the tests
if test/test_metrics_runner; then
    print_success "Metrics unit tests completed successfully"
    exit 0
else
    print_error "Metrics unit tests failed"
    exit 1
fi
