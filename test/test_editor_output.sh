#!/bin/bash
# Smoke test for the visual editor generated code
# This test verifies that the default generated C code compiles successfully

set -e  # Exit on error
set -u  # Exit on undefined variable
set -o pipefail  # Exit on pipeline failure

echo "🧪 Testing visual editor code generation..."

# Create a test directory
TEST_DIR=$(mktemp -d)
trap "cd / && rm -rf '$TEST_DIR'" EXIT  # Ensure cleanup on exit

cd "$TEST_DIR"

# Generate the default program code (what the editor produces on load)
cat > test_generated.c << 'EOF'
#include <stdio.h>

int main(){
    printf("Hello world!");
}
EOF

echo "✓ Created test C file with default editor output"

# Test compilation with strict flags
echo "🔨 Compiling with GCC (strict mode)..."
if ! gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -o test_hello test_generated.c 2>&1; then
    echo "✗ Compilation failed"
    exit 1
fi
echo "✓ Compilation successful"

# Test execution with timeout
echo "▶️  Running compiled program..."
if ! OUTPUT=$(timeout 5 ./test_hello 2>&1); then
    EXIT_CODE=$?
    if [ $EXIT_CODE -eq 124 ]; then
        echo "✗ Program execution timed out (5 seconds)"
    else
        echo "✗ Program execution failed with exit code $EXIT_CODE"
    fi
    exit 1
fi

# Verify output
if [ "$OUTPUT" = "Hello world!" ]; then
    echo "✓ Program output is correct"
else
    echo "✗ Program output mismatch"
    echo "Expected: 'Hello world!'"
    echo "Got: '$OUTPUT'"
    exit 1
fi

echo "✅ All visual editor smoke tests passed!"
