#!/usr/bin/env bash

# test_validation.sh - Tests for the validate_output.sh script
# This script tests various scenarios to ensure validate_output.sh works correctly

set -e

SCRIPT_DIR="$(dirname "$0")"
VALIDATE_SCRIPT="$SCRIPT_DIR/validate_output.sh"
TEST_DIR=$(mktemp -d)

# Cleanup function
cleanup() {
    rm -rf "$TEST_DIR"
    # Clean up any test binaries created in current directory
    rm -f test_correct test_wrong test_exit_fail test_newline test_stderr
}

# Set trap to cleanup on exit
trap cleanup EXIT

echo "Testing validate_output.sh script..."
echo "=================================="

# Test 1: Correct output should pass
echo "Test 1: Correct output..."
cat > "$TEST_DIR/correct.c" << 'EOF'
#include <stdio.h>
int main(){
    printf("Hello world!");
    return 0;
}
EOF
gcc -o test_correct "$TEST_DIR/correct.c"
if $VALIDATE_SCRIPT ./test_correct "correct-test" >/dev/null 2>&1; then
    echo "✓ Test 1 passed: Correct output validation works"
else
    echo "✗ Test 1 failed: Correct output should pass validation"
    exit 1
fi

# Test 2: Wrong output should fail
echo "Test 2: Wrong output..."
cat > "$TEST_DIR/wrong.c" << 'EOF'
#include <stdio.h>
int main(){
    printf("Wrong output!");
    return 0;
}
EOF
gcc -o test_wrong "$TEST_DIR/wrong.c"
if ! $VALIDATE_SCRIPT ./test_wrong "wrong-test" >/dev/null 2>&1; then
    echo "✓ Test 2 passed: Wrong output correctly rejected"
else
    echo "✗ Test 2 failed: Wrong output should fail validation"
    exit 1
fi

# Test 3: Non-zero exit code should fail
echo "Test 3: Non-zero exit code..."
cat > "$TEST_DIR/exit_fail.c" << 'EOF'
#include <stdio.h>
int main(){
    printf("Hello world!");
    return 1;
}
EOF
gcc -o test_exit_fail "$TEST_DIR/exit_fail.c"
if ! $VALIDATE_SCRIPT ./test_exit_fail "exit-test" >/dev/null 2>&1; then
    echo "✓ Test 3 passed: Non-zero exit code correctly rejected"
else
    echo "✗ Test 3 failed: Non-zero exit code should fail validation"
    exit 1
fi

# Test 4: Trailing newline should fail
echo "Test 4: Trailing newline..."
cat > "$TEST_DIR/newline.c" << 'EOF'
#include <stdio.h>
int main(){
    printf("Hello world!\n");
    return 0;
}
EOF
gcc -o test_newline "$TEST_DIR/newline.c"
if ! $VALIDATE_SCRIPT ./test_newline "newline-test" >/dev/null 2>&1; then
    echo "✓ Test 4 passed: Trailing newline correctly rejected"
else
    echo "✗ Test 4 failed: Trailing newline should fail validation"
    exit 1
fi

# Test 5: stderr output should warn but pass if output is correct
echo "Test 5: stderr output..."
cat > "$TEST_DIR/stderr.c" << 'EOF'
#include <stdio.h>
int main(){
    printf("Hello world!");
    fprintf(stderr, "Warning message");
    return 0;
}
EOF
gcc -o test_stderr "$TEST_DIR/stderr.c"
if $VALIDATE_SCRIPT ./test_stderr "stderr-test" 2>/dev/null | grep -q "WARNING:" &&
   $VALIDATE_SCRIPT ./test_stderr "stderr-test" >/dev/null 2>&1; then
    echo "✓ Test 5 passed: stderr output produces warning but passes validation"
else
    echo "✗ Test 5 failed: stderr should warn but pass with correct output"
    exit 1
fi

echo "=================================="
echo "All validation script tests passed!"
