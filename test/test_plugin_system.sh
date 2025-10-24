#!/bin/bash
# Test script for plugin system
# Verifies that the plugin system works correctly

set -e
set -u

echo "Testing Plugin System..."
echo ""

# Test 1: Basic hello still works
echo "Test 1: Basic hello output"
make clean > /dev/null 2>&1
make hello > /dev/null 2>&1
OUTPUT=$(./hello)
EXPECTED="Hello world!"
if [ "$OUTPUT" = "$EXPECTED" ]; then
    echo "  ✓ PASS: Basic hello works"
else
    echo "  ✗ FAIL: Expected '$EXPECTED', got '$OUTPUT'"
    exit 1
fi

# Test 2: Plugin demo compiles and runs
echo "Test 2: Plugin demo"
make plugin_demo > /dev/null 2>&1
if ./plugin_demo > /dev/null 2>&1; then
    echo "  ✓ PASS: Plugin demo runs successfully"
else
    echo "  ✗ FAIL: Plugin demo failed"
    exit 1
fi

# Test 3: Hello with plugins compiles and runs
echo "Test 3: Hello with plugins"
make hello-plugins > /dev/null 2>&1
if ./hello_with_plugins > /dev/null 2>&1; then
    echo "  ✓ PASS: Hello with plugins runs successfully"
else
    echo "  ✗ FAIL: Hello with plugins failed"
    exit 1
fi

# Test 4: Strict compilation
echo "Test 4: Strict compilation"
if make strict > /dev/null 2>&1; then
    echo "  ✓ PASS: Strict compilation successful"
else
    echo "  ✗ FAIL: Strict compilation failed"
    exit 1
fi

# Test 5: All plugins compile with strict flags
echo "Test 5: Strict compilation of all components"
if gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 \
    -o test_all plugin_demo.c plugin.c plugins_examples.c > /dev/null 2>&1; then
    echo "  ✓ PASS: All components compile with strict flags"
    rm -f test_all
else
    echo "  ✗ FAIL: Strict compilation failed"
    exit 1
fi

echo ""
echo "All plugin system tests passed!"
