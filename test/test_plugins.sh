#!/bin/bash
# Test script for plugin system validation
# Tests that plugins work correctly and can be chained

set -e

echo "Testing Plugin System..."
echo ""

# Test 1: Build without plugins
echo "Test 1: Build without plugins"
gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -DSILENT_PLUGINS -o hello_test1 hello.c plugin.c
OUTPUT1=$(./hello_test1)
if [ "$OUTPUT1" = "Hello world!" ]; then
    echo "✓ No plugins: PASSED (output: $OUTPUT1)"
else
    echo "✗ No plugins: FAILED (expected 'Hello world!', got '$OUTPUT1')"
    exit 1
fi
rm -f hello_test1
echo ""

# Test 2: Build with uppercase plugin
echo "Test 2: Build with uppercase plugin"
gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -DSILENT_PLUGINS -o hello_test2 hello.c plugin.c plugins/example_plugin.c
OUTPUT2=$(./hello_test2)
if [ "$OUTPUT2" = "HELLO WORLD!" ]; then
    echo "✓ Uppercase plugin: PASSED (output: $OUTPUT2)"
else
    echo "✗ Uppercase plugin: FAILED (expected 'HELLO WORLD!', got '$OUTPUT2')"
    exit 1
fi
rm -f hello_test2
echo ""

# Test 3: Build with prefix plugin
echo "Test 3: Build with prefix plugin"
gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -DSILENT_PLUGINS -o hello_test3 hello.c plugin.c plugins/prefix_plugin.c
OUTPUT3=$(./hello_test3)
if [ "$OUTPUT3" = "*** Hello world!" ]; then
    echo "✓ Prefix plugin: PASSED (output: $OUTPUT3)"
else
    echo "✗ Prefix plugin: FAILED (expected '*** Hello world!', got '$OUTPUT3')"
    exit 1
fi
rm -f hello_test3
echo ""

# Test 4: Build with chained plugins (prefix then uppercase)
echo "Test 4: Build with chained plugins (prefix then uppercase)"
gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -DSILENT_PLUGINS -o hello_test4 hello.c plugin.c plugins/prefix_plugin.c plugins/example_plugin.c
OUTPUT4=$(./hello_test4)
if [ "$OUTPUT4" = "*** HELLO WORLD!" ]; then
    echo "✓ Chained plugins: PASSED (output: $OUTPUT4)"
else
    echo "✗ Chained plugins: FAILED (expected '*** HELLO WORLD!', got '$OUTPUT4')"
    exit 1
fi
rm -f hello_test4
echo ""

# Test 5: Build with chained plugins in reverse order (uppercase then prefix)
echo "Test 5: Build with plugins in reverse order (uppercase then prefix)"
gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -DSILENT_PLUGINS -o hello_test5 hello.c plugin.c plugins/example_plugin.c plugins/prefix_plugin.c
OUTPUT5=$(./hello_test5)
if [ "$OUTPUT5" = "*** HELLO WORLD!" ]; then
    echo "✓ Reverse order plugins: PASSED (output: $OUTPUT5)"
else
    echo "✗ Reverse order plugins: FAILED (expected '*** HELLO WORLD!', got '$OUTPUT5')"
    exit 1
fi
rm -f hello_test5
echo ""

echo "=========================================="
echo "All Plugin System Tests: PASSED"
echo "=========================================="
