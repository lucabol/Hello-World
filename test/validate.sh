#!/bin/bash
<<<<<<< HEAD
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
=======
# Validation script for hello.c program
# This script validates that the hello program:
# 1. Compiles successfully with strict warnings
# 2. Outputs "Hello world!" with a trailing newline
# 3. Exits with code 0

set -e

# Parse command line arguments
QUIET=0
if [[ "$1" == "--quiet" ]]; then
    QUIET=1
fi

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

log() {
    if [[ $QUIET -eq 0 ]]; then
        echo -e "$1"
    fi
}

log "${YELLOW}Running validation tests for hello.c...${NC}"

# Test 1: Compile with strict warnings
log "\n${YELLOW}Test 1: Compiling with strict warnings...${NC}"
if gcc -Wall -Wextra -Wpedantic -Werror -o hello hello.c; then
    log "${GREEN}✓ Compilation successful${NC}"
else
    log "${RED}✗ Compilation failed${NC}"
    exit 1
fi

# Test 2: Check output format
log "\n${YELLOW}Test 2: Checking output format...${NC}"
OUTPUT=$(./hello)
EXPECTED="Hello world!"

if [[ "$OUTPUT" == "$EXPECTED" ]]; then
    log "${GREEN}✓ Output matches expected: '$EXPECTED'${NC}"
else
    log "${RED}✗ Output mismatch${NC}"
    log "${RED}  Expected: '$EXPECTED'${NC}"
    log "${RED}  Got:      '$OUTPUT'${NC}"
    exit 1
fi

# Test 3: Check exit code
log "\n${YELLOW}Test 3: Checking exit code...${NC}"
./hello > /dev/null
EXIT_CODE=$?
if [[ $EXIT_CODE -eq 0 ]]; then
    log "${GREEN}✓ Exit code is 0${NC}"
else
    log "${RED}✗ Exit code is $EXIT_CODE (expected 0)${NC}"
    exit 1
fi

# Test 4: Verify newline character is present
log "\n${YELLOW}Test 4: Verifying newline character in output...${NC}"
# Use hexdump to check for newline at the end
OUTPUT_HEX=$(./hello | od -An -tx1 | tr -d ' \n')
# Expected hex ends with "0a" (newline character)
# "Hello world!\n" = 48656c6c6f20776f726c64210a
if [[ "$OUTPUT_HEX" =~ 0a$ ]]; then
    log "${GREEN}✓ Newline character (0x0a) found at end of output${NC}"
else
    log "${RED}✗ Newline character not found at end of output${NC}"
    log "${RED}  Hex dump: $OUTPUT_HEX${NC}"
    exit 1
fi

# Test 5: Byte-level output verification
log "\n${YELLOW}Test 5: Byte-level output verification...${NC}"
EXPECTED_HEX="48656c6c6f20776f726c64210a"
ACTUAL_HEX=$(./hello | xxd -p | tr -d '\n')
if [[ "$ACTUAL_HEX" == "$EXPECTED_HEX" ]]; then
    log "${GREEN}✓ Byte-level output matches expected${NC}"
else
    log "${RED}✗ Byte-level output mismatch${NC}"
    log "${RED}  Expected: $EXPECTED_HEX${NC}"
    log "${RED}  Got:      $ACTUAL_HEX${NC}"
    exit 1
fi

log "\n${GREEN}All validation tests passed!${NC}"
exit 0
>>>>>>> main
