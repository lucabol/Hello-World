#!/bin/bash
# Test script for plugin system
# Validates that plugins work correctly and don't break core functionality

set -e  # Exit on any error
set -u  # Exit on unset variables
set -o pipefail  # Exit on pipeline failures

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

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
    rm -f test_plugin_1 test_plugin_2 test_plugin_3 test_plugin_4
}

trap cleanup EXIT

print_info "Testing Plugin System..."
echo ""

# Test 1: Default hello.c (no plugins)
print_info "Test 1: Default hello.c compilation..."
if gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -o test_plugin_1 hello.c 2>&1; then
    OUTPUT=$(./test_plugin_1)
    if [ "$OUTPUT" = "Hello world!" ]; then
        print_success "Default compilation works correctly"
    else
        print_error "Default output incorrect. Expected 'Hello world!', got '$OUTPUT'"
        exit 1
    fi
else
    print_error "Default compilation failed"
    exit 1
fi

# Test 2: Plugin-enabled without plugins
print_info "Test 2: Plugin-enabled without plugins..."
if gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -o test_plugin_2 hello_plugin_enabled.c plugin.c 2>&1; then
    OUTPUT=$(./test_plugin_2)
    if [ "$OUTPUT" = "Hello world!" ]; then
        print_success "Plugin-enabled (no plugins) works correctly"
    else
        print_error "Plugin-enabled output incorrect. Expected 'Hello world!', got '$OUTPUT'"
        exit 1
    fi
else
    print_error "Plugin-enabled compilation failed"
    exit 1
fi

# Test 3: With uppercase plugin
print_info "Test 3: With uppercase plugin..."
if gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -o test_plugin_3 hello_plugin_enabled.c plugin.c plugins/example_uppercase.c -DLOAD_UPPERCASE 2>&1; then
    OUTPUT=$(./test_plugin_3)
    if [ "$OUTPUT" = "HELLO WORLD!" ]; then
        print_success "Uppercase plugin works correctly"
    else
        print_error "Uppercase plugin output incorrect. Expected 'HELLO WORLD!', got '$OUTPUT'"
        exit 1
    fi
else
    print_error "Uppercase plugin compilation failed"
    exit 1
fi

# Test 4: With both plugins (chaining)
print_info "Test 4: With uppercase + reverse plugins (chaining)..."
if gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -o test_plugin_4 hello_plugin_enabled.c plugin.c plugins/example_uppercase.c plugins/example_reverse.c -DLOAD_UPPERCASE -DLOAD_REVERSE 2>&1; then
    OUTPUT=$(./test_plugin_4)
    if [ "$OUTPUT" = "!DLROW OLLEH" ]; then
        print_success "Plugin chaining works correctly"
    else
        print_error "Plugin chaining output incorrect. Expected '!DLROW OLLEH', got '$OUTPUT'"
        exit 1
    fi
else
    print_error "Plugin chaining compilation failed"
    exit 1
fi

echo ""
print_success "All plugin system tests passed!"
