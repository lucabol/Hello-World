#!/bin/bash
# Test script for plugin system
# Validates that plugins work correctly

set -e

echo "Testing Plugin System..."
echo ""

# Build examples
echo "Building plugin examples..."
make clean > /dev/null 2>&1
make examples > /dev/null 2>&1
echo "✓ Build successful"

# Test default behavior
echo "Testing default behavior (no plugins)..."
make hello > /dev/null 2>&1
OUTPUT=$(./hello)
EXPECTED="Hello world!"
if [ "$OUTPUT" != "$EXPECTED" ]; then
    echo "✗ Default output incorrect"
    echo "  Expected: $EXPECTED"
    echo "  Got: $OUTPUT"
    exit 1
fi
echo "✓ Default behavior correct"

# Test uppercase plugin
echo "Testing uppercase plugin..."
OUTPUT=$(./hello-with-uppercase)
EXPECTED="HELLO WORLD!"
if [ "$OUTPUT" != "$EXPECTED" ]; then
    echo "✗ Uppercase plugin output incorrect"
    echo "  Expected: $EXPECTED"
    echo "  Got: $OUTPUT"
    exit 1
fi
echo "✓ Uppercase plugin works"

# Test reverse plugin
echo "Testing reverse plugin..."
OUTPUT=$(./hello-with-reverse)
EXPECTED="!dlrow olleH"
if [ "$OUTPUT" != "$EXPECTED" ]; then
    echo "✗ Reverse plugin output incorrect"
    echo "  Expected: $EXPECTED"
    echo "  Got: $OUTPUT"
    exit 1
fi
echo "✓ Reverse plugin works"

# Test chained plugins
echo "Testing chained plugins..."
OUTPUT=$(./hello-with-chain)
EXPECTED="!DLROW OLLEH"
if [ "$OUTPUT" != "$EXPECTED" ]; then
    echo "✗ Chained plugins output incorrect"
    echo "  Expected: $EXPECTED"
    echo "  Got: $OUTPUT"
    exit 1
fi
echo "✓ Chained plugins work"

echo ""
echo "All plugin tests passed!"
