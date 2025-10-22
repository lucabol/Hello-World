#!/bin/bash
# Smoke test for the visual editor generated code
# This test verifies that the default generated C code compiles successfully

set -e

echo "🧪 Testing visual editor code generation..."

# Create a test directory
TEST_DIR=$(mktemp -d)
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
if gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -o test_hello test_generated.c; then
    echo "✓ Compilation successful"
else
    echo "✗ Compilation failed"
    exit 1
fi

# Test execution
echo "▶️  Running compiled program..."
OUTPUT=$(./test_hello)
if [ "$OUTPUT" = "Hello world!" ]; then
    echo "✓ Program output is correct"
else
    echo "✗ Program output mismatch"
    echo "Expected: 'Hello world!'"
    echo "Got: '$OUTPUT'"
    exit 1
fi

# Cleanup
cd /
rm -rf "$TEST_DIR"

echo "✅ All visual editor smoke tests passed!"
