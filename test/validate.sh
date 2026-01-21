#!/bin/bash
# Validation script for Hello World program
# Ensures the program compiles and produces correct output

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
SOURCE_FILE="$REPO_ROOT/hello.c"

# Parse arguments
QUIET=false
if [[ "$1" == "--quiet" ]]; then
    QUIET=true
fi

# Logging functions
log_info() {
    if [ "$QUIET" = false ]; then
        echo "[INFO] $1"
    fi
}

log_success() {
    if [ "$QUIET" = false ]; then
        echo "[SUCCESS] $1"
    fi
}

log_error() {
    echo "[ERROR] $1" >&2
}

# Test compilation with strict flags
test_compilation() {
    log_info "Testing strict compilation..."
    
    cd "$REPO_ROOT"
    rm -f hello_test 2>/dev/null || true
    
    if gcc -Wall -Wextra -Wpedantic -Werror -o hello_test "$SOURCE_FILE" 2>&1; then
        log_success "Compilation successful"
    else
        log_error "Compilation failed"
        return 1
    fi
}

# Test program output
test_output() {
    log_info "Testing program output..."
    
    cd "$REPO_ROOT"
    
    local output=$(./hello_test)
    local exit_code=$?
    
    # Check exit code
    if [ $exit_code -ne 0 ]; then
        log_error "Program exited with code $exit_code (expected 0)"
        return 1
    fi
    
    # Check output (must be exactly "Hello world!" with no trailing newline)
    if [ "$output" != "Hello world!" ]; then
        log_error "Program output incorrect. Expected: 'Hello world!' Got: '$output'"
        return 1
    fi
    
    log_success "Program output correct"
}

# Byte-level output verification
test_output_bytes() {
    log_info "Testing byte-level output..."
    
    cd "$REPO_ROOT"
    
    # Capture output to file
    ./hello_test > /tmp/hello_output.txt
    
    # Expected: "Hello world!" (12 bytes, no newline)
    local size=$(stat -f%z /tmp/hello_output.txt 2>/dev/null || stat -c%s /tmp/hello_output.txt)
    
    if [ "$size" -ne 12 ]; then
        log_error "Output size incorrect. Expected: 12 bytes, Got: $size bytes"
        rm -f /tmp/hello_output.txt
        return 1
    fi
    
    log_success "Byte-level output correct"
    rm -f /tmp/hello_output.txt
}

# Main validation
main() {
    if [ "$QUIET" = false ]; then
        echo "==================================="
        echo "Hello World Validation Tests"
        echo "==================================="
        echo ""
    fi
    
    # Run tests
    test_compilation || exit 1
    test_output || exit 1
    test_output_bytes || exit 1
    
    # Cleanup
    cd "$REPO_ROOT"
    rm -f hello_test 2>/dev/null || true
    
    if [ "$QUIET" = false ]; then
        echo ""
        echo "==================================="
        echo "All validation tests passed!"
        echo "==================================="
    fi
}

main
