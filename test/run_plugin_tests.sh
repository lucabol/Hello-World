#!/bin/bash
# Unit test runner for plugin system
# Compiles and runs the plugin unit tests

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

print_info "Building plugin unit tests..."

# Compile the unit tests with strict flags
STRICT_FLAGS="-Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99"

if gcc ${STRICT_FLAGS} -I. -o test_plugin_runner test/test_plugin.c plugin.c plugins/uppercase.c plugins/reverse.c plugins/prefix.c 2>&1; then
    print_success "Plugin unit tests compiled successfully"
else
    print_error "Failed to compile plugin unit tests"
    exit 1
fi

print_info "Running plugin unit tests..."
echo ""

# Run the tests
if ./test_plugin_runner; then
    print_success "Plugin unit tests completed successfully"
else
    print_error "Plugin unit tests failed"
    exit 1
fi

print_info "Building plugin edge case tests..."

if gcc ${STRICT_FLAGS} -I. -o test_plugin_edge_runner test/test_plugin_edge_cases.c plugin.c plugins/uppercase.c plugins/reverse.c plugins/prefix.c 2>&1; then
    print_success "Plugin edge case tests compiled successfully"
else
    print_error "Failed to compile plugin edge case tests"
    exit 1
fi

print_info "Running plugin edge case tests..."
echo ""

# Run the edge case tests
if ./test_plugin_edge_runner; then
    print_success "Plugin edge case tests completed successfully"
    echo ""
    print_success "All plugin tests passed!"
    exit 0
else
    print_error "Plugin edge case tests failed"
    exit 1
fi
