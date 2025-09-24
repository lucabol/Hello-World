#!/bin/bash
# Simple test for validate-binary.sh to exercise success and failure paths
# This ensures the validator itself works correctly

set -e
set -u

echo "=== Testing validate-binary.sh ==="

# Create test binaries
echo "Creating test binaries..."

# Good binary (matches expected output)
cat > test_good.c << 'EOF'
#include <stdio.h>
int main(void) {
    printf("Hello world!\n");
    return 0;
}
EOF

# Bad exit code binary
cat > test_bad_exit.c << 'EOF'
#include <stdio.h>
int main(void) {
    printf("Hello world!\n");
    return 42;
}
EOF

# Bad output binary (missing newline)
cat > test_bad_output.c << 'EOF'
#include <stdio.h>
int main(void) {
    printf("Hello world!");
    return 0;
}
EOF

# Compile test binaries
gcc -o test_good test_good.c
gcc -o test_bad_exit test_bad_exit.c
gcc -o test_bad_output test_bad_output.c

echo "Testing good binary (should pass)..."
if bash test/validate-binary.sh ./test_good --quiet; then
    echo "✓ Good binary test PASSED"
else
    echo "✗ Good binary test FAILED"
    exit 1
fi

echo "Testing bad exit code binary (should fail)..."
if bash test/validate-binary.sh ./test_bad_exit --quiet; then
    echo "✗ Bad exit code test should have FAILED but PASSED"
    exit 1
else
    echo "✓ Bad exit code test FAILED as expected"
fi

echo "Testing bad output binary (should fail)..."
if bash test/validate-binary.sh ./test_bad_output --quiet; then
    echo "✗ Bad output test should have FAILED but PASSED"
    exit 1
else
    echo "✓ Bad output test FAILED as expected"
fi

# Cleanup
rm -f test_good test_bad_exit test_bad_output test_good.c test_bad_exit.c test_bad_output.c

echo "=== All validator tests PASSED ==="