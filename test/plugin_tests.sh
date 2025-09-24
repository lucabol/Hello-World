#!/bin/bash
# Comprehensive test suite for Hello World plugin system
# Tests plugin loading, registration, pipeline behavior, and failure modes

set -e
set -u
set -o pipefail

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

TEST_DIR="$(dirname "$0")"
PROJECT_ROOT="$(cd "$TEST_DIR/.." && pwd)"
PLUGINS_DIR="$PROJECT_ROOT/plugins"
TEST_PLUGINS_DIR="$PROJECT_ROOT/test_plugins"

# Test counters
TESTS_PASSED=0
TESTS_FAILED=0

print_test_header() {
    printf "\n%b=== %s ===%b\n" "$BLUE" "$1" "$NC"
}

print_success() {
    printf "%b✓ %s%b\n" "$GREEN" "$1" "$NC"
    ((TESTS_PASSED++))
}

print_failure() {
    printf "%b✗ %s%b\n" "$RED" "$1" "$NC"
    ((TESTS_FAILED++))
}

print_info() {
    printf "%b• %s%b\n" "$YELLOW" "$1" "$NC"
}

# Setup test environment
setup_tests() {
    cd "$PROJECT_ROOT"
    
    # Create test plugins directory
    mkdir -p "$TEST_PLUGINS_DIR"
    
    # Build main executable
    print_info "Building main executable..."
    make clean > /dev/null 2>&1 || true
    if make all > build.log 2>&1; then
        print_success "Main executable built successfully"
    else
        print_failure "Failed to build main executable"
        cat build.log
        exit 1
    fi
    
    # Build standard plugins
    print_info "Building standard plugins..."
    if make plugins > build_plugins.log 2>&1; then
        print_success "Standard plugins built successfully"
    else
        print_info "No standard plugins to build or build failed (this is OK for basic tests)"
        # This is not a fatal error - we can still test without plugins
    fi
}

# Cleanup test environment
cleanup_tests() {
    cd "$PROJECT_ROOT"
    rm -rf "$TEST_PLUGINS_DIR"
    rm -f build.log build_plugins.log test_output.txt
    rm -f hello_test malformed_plugin.so
}

# Test 1: Run hello without plugins
test_no_plugins() {
    print_test_header "Test 1: No Plugins Behavior"
    
    # Remove all plugin files
    rm -f "$PLUGINS_DIR"/*.so
    
    # Run hello and capture output
    if output=$(./hello 2>&1); then
        if [[ "$output" == "Hello world!" ]]; then
            print_success "Correct output without plugins: '$output'"
        else
            print_failure "Incorrect output without plugins: '$output' (expected 'Hello world!')"
        fi
    else
        print_failure "Program failed to run without plugins"
    fi
}

# Test 2: Individual plugins
test_individual_plugins() {
    print_test_header "Test 2: Individual Plugin Behavior"
    
    # Test uppercase plugin only
    rm -f "$PLUGINS_DIR"/*.so
    make "$PLUGINS_DIR/uppercase.so" > /dev/null 2>&1
    if output=$(./hello 2>&1); then
        expected="HELLO WORLD!"
        if [[ "$output" == "$expected" ]]; then
            print_success "Uppercase plugin works: '$output'"
        else
            print_failure "Uppercase plugin failed: '$output' (expected '$expected')"
        fi
    else
        print_failure "Program failed with uppercase plugin"
    fi
    
    # Test reverse plugin only  
    rm -f "$PLUGINS_DIR"/*.so
    make "$PLUGINS_DIR/reverse.so" > /dev/null 2>&1
    if output=$(./hello 2>&1); then
        expected="!dlrow olleH"
        if [[ "$output" == "$expected" ]]; then
            print_success "Reverse plugin works: '$output'"
        else
            print_failure "Reverse plugin failed: '$output' (expected '$expected')"
        fi
    else
        print_failure "Program failed with reverse plugin"
    fi
    
    # Test exclamation plugin only
    rm -f "$PLUGINS_DIR"/*.so
    make "$PLUGINS_DIR/exclamation.so" > /dev/null 2>&1
    if output=$(./hello 2>&1); then
        expected="Hello world!!!"
        if [[ "$output" == "$expected" ]]; then
            print_success "Exclamation plugin works: '$output'"
        else
            print_failure "Exclamation plugin failed: '$output' (expected '$expected')"
        fi
    else
        print_failure "Program failed with exclamation plugin"
    fi
}

# Test 3: Plugin combinations
test_plugin_combinations() {
    print_test_header "Test 3: Plugin Combination Behavior"
    
    # Test uppercase + exclamation
    rm -f "$PLUGINS_DIR"/*.so
    make "$PLUGINS_DIR/uppercase.so" "$PLUGINS_DIR/exclamation.so" > /dev/null 2>&1
    if output=$(./hello 2>&1); then
        expected="HELLO WORLD!!!"
        if [[ "$output" == "$expected" ]]; then
            print_success "Uppercase + exclamation works: '$output'"
        else
            print_failure "Uppercase + exclamation failed: '$output' (expected '$expected')"
        fi
    else
        print_failure "Program failed with uppercase + exclamation plugins"
    fi
    
    # Test all plugins
    rm -f "$PLUGINS_DIR"/*.so  
    make plugins > /dev/null 2>&1
    if output=$(./hello 2>&1); then
        # Output should be transformed by all plugins in sequence
        # The exact output depends on plugin loading order, so just verify it's not original
        if [[ "$output" != "Hello world!" ]] && [[ -n "$output" ]]; then
            print_success "All plugins transform output: '$output'"
        else
            print_failure "All plugins failed to transform output: '$output'"
        fi
    else
        print_failure "Program failed with all plugins"
    fi
}

# Test 4: Malformed plugin handling
test_malformed_plugins() {
    print_test_header "Test 4: Malformed Plugin Handling"
    
    # Create a malformed plugin (missing plugin_init)
    cat > "$TEST_PLUGINS_DIR/malformed.c" << 'EOF'
// Malformed plugin - missing plugin_init function
int some_function(void) {
    return 42;
}
EOF
    
    gcc -fPIC -shared -o "$PLUGINS_DIR/malformed.so" "$TEST_PLUGINS_DIR/malformed.c" 2>/dev/null || true
    
    # Program should handle malformed plugin gracefully
    if output=$(./hello 2>&1); then
        if [[ "$output" == "Hello world!" ]]; then
            print_success "Gracefully handled malformed plugin"
        else
            print_failure "Unexpected output with malformed plugin: '$output'"
        fi
    else
        print_failure "Program crashed with malformed plugin"
    fi
    
    # Remove malformed plugin
    rm -f "$PLUGINS_DIR/malformed.so"
}

# Test 5: Plugin with wrong ABI version
test_abi_version() {
    print_test_header "Test 5: ABI Version Checking"
    
    # Create plugin with wrong ABI version
    cat > "$TEST_PLUGINS_DIR/wrong_abi.c" << 'EOF'
#include "../plugin.h"

static int dummy_transform(const char* input, char* output, size_t output_size) {
    if (strlen(input) >= output_size) return -1;
    strcpy(output, input);
    return 0;
}

plugin_info_t plugin_init(void) {
    plugin_info_t info = {
        .name = "wrong_abi",
        .version = 999,  // Wrong version
        .description = "Plugin with wrong ABI version"
    };
    
    register_transformer("wrong_abi", dummy_transform);
    return info;
}
EOF
    
    # Build the wrong ABI plugin
    if gcc -fPIC -shared -I. -o "$PLUGINS_DIR/wrong_abi.so" "$TEST_PLUGINS_DIR/wrong_abi.c" 2>/dev/null; then
        print_success "Wrong ABI plugin compiled successfully"
    else
        print_failure "Failed to compile wrong ABI plugin"
        return
    fi
    
    # Program should reject plugin with wrong ABI version  
    if output=$(./hello 2>&1); then
        if [[ "$output" == "Hello world!" ]]; then
            print_success "Correctly rejected plugin with wrong ABI version"
            # Check that error message was printed to stderr
            if ./hello 2>&1 | grep -q "incompatible ABI version"; then
                print_success "Correct error message for ABI version mismatch"
            else
                print_failure "Missing error message for ABI version mismatch"
            fi
        else
            print_failure "Incorrectly accepted plugin with wrong ABI version: '$output'"
        fi
    else
        print_failure "Program crashed with wrong ABI version plugin"
    fi
    
    # Remove wrong ABI plugin
    rm -f "$PLUGINS_DIR/wrong_abi.so"
}

# Test 6: Exit code validation
test_exit_codes() {
    print_test_header "Test 6: Exit Code Validation"
    
    # Test without plugins
    rm -f "$PLUGINS_DIR"/*.so
    if ./hello > /dev/null 2>&1; then
        exit_code=$?
        if [[ $exit_code -eq 0 ]]; then
            print_success "Correct exit code without plugins: $exit_code"
        else
            print_failure "Wrong exit code without plugins: $exit_code (expected 0)"
        fi
    else
        exit_code=$?
        print_failure "Program failed, exit code: $exit_code"
    fi
    
    # Test with plugins
    make plugins > /dev/null 2>&1
    if ./hello > /dev/null 2>&1; then
        exit_code=$?
        if [[ $exit_code -eq 0 ]]; then
            print_success "Correct exit code with plugins: $exit_code"
        else
            print_failure "Wrong exit code with plugins: $exit_code (expected 0)"
        fi
    else
        exit_code=$?
        print_failure "Program failed with plugins, exit code: $exit_code"
    fi
}

# Test 7: Directory handling
test_directory_handling() {
    print_test_header "Test 7: Directory Handling"
    
    # Test with non-existent plugins directory
    rm -rf "$PLUGINS_DIR"
    if output=$(./hello 2>&1); then
        if [[ "$output" == "Hello world!" ]]; then
            print_success "Handled non-existent plugins directory"
        else
            print_failure "Failed with non-existent plugins directory: '$output'"
        fi
    else
        print_failure "Program crashed with non-existent plugins directory"
    fi
    
    # Recreate plugins directory for other tests
    mkdir -p "$PLUGINS_DIR"
}

# Main test execution
main() {
    print_test_header "Hello World Plugin System Test Suite"
    
    # Setup
    setup_tests
    
    # Run all tests
    test_no_plugins
    test_individual_plugins  
    test_plugin_combinations
    test_malformed_plugins
    test_abi_version
    test_exit_codes
    test_directory_handling
    
    # Cleanup
    cleanup_tests
    
    # Summary
    printf "\n%b=== Test Summary ===%b\n" "$BLUE" "$NC"
    printf "Tests passed: %b%d%b\n" "$GREEN" "$TESTS_PASSED" "$NC"
    printf "Tests failed: %b%d%b\n" "$RED" "$TESTS_FAILED" "$NC"
    
    if [[ $TESTS_FAILED -eq 0 ]]; then
        printf "\n%b🎉 All tests passed!%b\n" "$GREEN" "$NC"
        exit 0
    else
        printf "\n%b❌ Some tests failed!%b\n" "$RED" "$NC"
        exit 1
    fi
}

# Run main function
main "$@"