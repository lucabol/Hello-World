#!/bin/bash
# Test script for plugin system
# Verifies that plugins work correctly and core behavior is preserved

set -e

echo "=== Plugin System Test Suite ==="
echo ""

# Test 1: Core hello.c without plugins
echo "Test 1: Building and running without plugins..."
make clean > /dev/null 2>&1
make > /dev/null 2>&1
OUTPUT=$(./hello)
if [ "$OUTPUT" = "Hello world!" ]; then
    echo "✓ PASS: Core hello.c works without plugins"
else
    echo "✗ FAIL: Expected 'Hello world!', got '$OUTPUT'"
    exit 1
fi

# Test 2: Uppercase plugin
echo "Test 2: Building and running with uppercase plugin..."
make uppercase > /dev/null 2>&1
OUTPUT=$(./hello_uppercase)
if [ "$OUTPUT" = "HELLO WORLD!" ]; then
    echo "✓ PASS: Uppercase plugin transforms correctly"
else
    echo "✗ FAIL: Expected 'HELLO WORLD!', got '$OUTPUT'"
    exit 1
fi

# Test 3: Decorator plugin
echo "Test 3: Building and running with decorator plugin..."
make decorator > /dev/null 2>&1
OUTPUT=$(./hello_decorator)
EXPECTED="=== Plugin Output Start ===
*** Hello world! ***
=== Plugin Output End ==="
if [ "$OUTPUT" = "$EXPECTED" ]; then
    echo "✓ PASS: Decorator plugin works correctly"
else
    echo "✗ FAIL: Decorator plugin output mismatch"
    echo "Expected:"
    echo "$EXPECTED"
    echo "Got:"
    echo "$OUTPUT"
    exit 1
fi

# Test 4: Multiple plugins
echo "Test 4: Building and running with multiple plugins..."
make with-plugins > /dev/null 2>&1
OUTPUT=$(./hello_with_plugins)
EXPECTED="=== Plugin Output Start ===
*** HELLO WORLD! ***
=== Plugin Output End ==="
if [ "$OUTPUT" = "$EXPECTED" ]; then
    echo "✓ PASS: Multiple plugins work together"
else
    echo "✗ FAIL: Multiple plugins output mismatch"
    echo "Expected:"
    echo "$EXPECTED"
    echo "Got:"
    echo "$OUTPUT"
    exit 1
fi

# Test 5: Compilation with strict flags
echo "Test 5: Testing strict compilation..."
make clean > /dev/null 2>&1
if gcc -Wall -Wextra -Wpedantic -Werror -o hello_test hello.c > /dev/null 2>&1; then
    echo "✓ PASS: Core hello.c compiles with strict flags"
else
    echo "✗ FAIL: Core hello.c fails strict compilation"
    exit 1
fi

if gcc -Wall -Wextra -Wpedantic -Werror -DUSE_PLUGINS -o hello_test hello.c plugin.c plugin_uppercase.c > /dev/null 2>&1; then
    echo "✓ PASS: Plugin code compiles with strict flags"
else
    echo "✗ FAIL: Plugin code fails strict compilation"
    exit 1
fi

# Cleanup
rm -f hello_test

echo ""
echo "=== All Plugin System Tests Passed ==="
