#!/bin/bash
# Test script for plugin system functionality
# Validates plugin registration, execution, and output transformation

set -e

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

print_info "Testing plugin system functionality..."

# Test 1: Verify original functionality is preserved
print_info "Test 1: Verifying backward compatibility..."
make hello >/dev/null 2>&1

# Use the same method as validate.sh to capture output with trailing whitespace
set +e
ORIGINAL_OUTPUT_WITH_SENTINEL=$(./hello 2>&1; printf x)
ORIGINAL_EXIT_CODE=$?
set -e
ORIGINAL_OUTPUT="${ORIGINAL_OUTPUT_WITH_SENTINEL%x}"

EXPECTED_ORIGINAL="Hello world!"$'\n'"Exit code: 0"$'\n'

if [[ "${ORIGINAL_OUTPUT}" == "${EXPECTED_ORIGINAL}" ]] && [[ ${ORIGINAL_EXIT_CODE} -eq 0 ]]; then
    print_success "Original functionality preserved"
else
    print_error "Original functionality broken"
    printf "Expected: '%s'\n" "${EXPECTED_ORIGINAL}"
    printf "Actual: '%s'\n" "${ORIGINAL_OUTPUT}"
    printf "Expected length: %d, Actual length: %d\n" "${#EXPECTED_ORIGINAL}" "${#ORIGINAL_OUTPUT}"
    exit 1
fi

# Test 2: Verify plugin system builds correctly
print_info "Test 2: Building plugin-enabled version..."
if make hello-plugins >/dev/null 2>&1; then
    print_success "Plugin system builds successfully"
else
    print_error "Plugin system build failed"
    exit 1
fi

# Test 3: Verify plugin system executes and transforms output
print_info "Test 3: Testing plugin execution..."
PLUGIN_OUTPUT=$(./hello-plugins 2>&1)

# Check that plugin output contains expected elements
if echo "${PLUGIN_OUTPUT}" | grep -q "Plugin System Active" && \
   echo "${PLUGIN_OUTPUT}" | grep -q "HELLO WORLD!" && \
   echo "${PLUGIN_OUTPUT}" | grep -q "Exit code: 0" && \
   echo "${PLUGIN_OUTPUT}" | grep -q "Plugin execution complete"; then
    print_success "Plugin system executes and transforms output correctly"
else
    print_error "Plugin output incorrect"
    printf "Plugin output:\n%s\n" "${PLUGIN_OUTPUT}"
    exit 1
fi

# Test 4: Verify strict compilation works with plugins
print_info "Test 4: Testing strict compilation with plugins..."
if make strict-plugins >/dev/null 2>&1; then
    print_success "Strict compilation with plugins passes"
else
    print_error "Strict compilation with plugins failed"
    exit 1
fi

# Test 5: Verify plugins don't interfere with exit code
print_info "Test 5: Verifying exit code preservation..."
./hello-plugins >/dev/null 2>&1
PLUGIN_EXIT_CODE=$?

if [[ ${PLUGIN_EXIT_CODE} -eq 0 ]]; then
    print_success "Exit code preserved with plugins"
else
    print_error "Exit code incorrect with plugins (expected 0, got ${PLUGIN_EXIT_CODE})"
    exit 1
fi

# Test 6: Test clang compilation with plugins
print_info "Test 6: Testing clang compilation with plugins..."
if make clang-plugins >/dev/null 2>&1; then
    print_success "Clang compilation with plugins passes"
    
    # Verify clang plugin output  
    CLANG_PLUGIN_OUTPUT=$(./hello_clang_plugins 2>&1)
    if echo "${CLANG_PLUGIN_OUTPUT}" | grep -q "HELLO WORLD!"; then
        print_success "Clang-compiled plugins work correctly"
    else
        print_error "Clang-compiled plugins output incorrect"
        exit 1
    fi
else
    print_error "Clang compilation with plugins failed"
    exit 1
fi

print_success "All plugin tests passed!"

print_info "Summary:"
printf "  - Backward compatibility: PASSED\n"
printf "  - Plugin build system: PASSED\n"  
printf "  - Plugin execution: PASSED\n"
printf "  - Strict compilation: PASSED\n"
printf "  - Exit code preservation: PASSED\n"
printf "  - Clang compatibility: PASSED\n"

# Cleanup
make clean >/dev/null 2>&1

print_info "Plugin test completed successfully"