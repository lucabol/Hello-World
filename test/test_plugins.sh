#!/bin/bash
# Test script for plugin system validation
# Tests that plugins work correctly and can be chained
# Also tests error handling and memory safety

set -e

echo "Testing Plugin System..."
echo ""

# Detect if AddressSanitizer is available
ASAN_FLAGS=""
if gcc -fsanitize=address -x c -c - -o /dev/null 2>/dev/null <<< "int main(){return 0;}"; then
    ASAN_FLAGS="-fsanitize=address -fno-omit-frame-pointer"
    echo "✓ AddressSanitizer available, using ASAN for memory safety checks"
    rm -f a.out
else
    echo "ℹ AddressSanitizer not available, skipping memory safety checks"
fi
echo ""

# Test 1: Build without plugins
echo "Test 1: Build without plugins"
gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -DSILENT_PLUGINS $ASAN_FLAGS -o hello_test1 hello.c plugin.c 2>&1
OUTPUT1=$(./hello_test1)
EXIT1=$?
if [ "$OUTPUT1" = "Hello world!" ] && [ $EXIT1 -eq 0 ]; then
    echo "✓ No plugins: PASSED (output: $OUTPUT1, exit: $EXIT1)"
else
    echo "✗ No plugins: FAILED (expected 'Hello world!' exit 0, got '$OUTPUT1' exit $EXIT1)"
    exit 1
fi
rm -f hello_test1
echo ""

# Test 2: Build with uppercase plugin
echo "Test 2: Build with uppercase plugin"
gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -DSILENT_PLUGINS $ASAN_FLAGS -o hello_test2 hello.c plugin.c plugins/example_plugin.c 2>&1
OUTPUT2=$(./hello_test2)
EXIT2=$?
if [ "$OUTPUT2" = "HELLO WORLD!" ] && [ $EXIT2 -eq 0 ]; then
    echo "✓ Uppercase plugin: PASSED (output: $OUTPUT2)"
else
    echo "✗ Uppercase plugin: FAILED (expected 'HELLO WORLD!' exit 0, got '$OUTPUT2' exit $EXIT2)"
    exit 1
fi
rm -f hello_test2
echo ""

# Test 3: Build with prefix plugin
echo "Test 3: Build with prefix plugin"
gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -DSILENT_PLUGINS $ASAN_FLAGS -o hello_test3 hello.c plugin.c plugins/prefix_plugin.c 2>&1
OUTPUT3=$(./hello_test3)
EXIT3=$?
if [ "$OUTPUT3" = "*** Hello world!" ] && [ $EXIT3 -eq 0 ]; then
    echo "✓ Prefix plugin: PASSED (output: $OUTPUT3)"
else
    echo "✗ Prefix plugin: FAILED (expected '*** Hello world!' exit 0, got '$OUTPUT3' exit $EXIT3)"
    exit 1
fi
rm -f hello_test3
echo ""

# Test 4: Build with chained plugins (prefix then uppercase)
echo "Test 4: Build with chained plugins (prefix then uppercase)"
gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -DSILENT_PLUGINS $ASAN_FLAGS -o hello_test4 hello.c plugin.c plugins/prefix_plugin.c plugins/example_plugin.c 2>&1
OUTPUT4=$(./hello_test4)
EXIT4=$?
if [ "$OUTPUT4" = "*** HELLO WORLD!" ] && [ $EXIT4 -eq 0 ]; then
    echo "✓ Chained plugins: PASSED (output: $OUTPUT4)"
else
    echo "✗ Chained plugins: FAILED (expected '*** HELLO WORLD!' exit 0, got '$OUTPUT4' exit $EXIT4)"
    exit 1
fi
rm -f hello_test4
echo ""

# Test 5: Build with plugins in reverse order (uppercase then prefix)
echo "Test 5: Build with plugins in reverse order (uppercase then prefix)"
gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -DSILENT_PLUGINS $ASAN_FLAGS -o hello_test5 hello.c plugin.c plugins/example_plugin.c plugins/prefix_plugin.c 2>&1
OUTPUT5=$(./hello_test5)
EXIT5=$?
if [ "$OUTPUT5" = "*** HELLO WORLD!" ] && [ $EXIT5 -eq 0 ]; then
    echo "✓ Reverse order plugins: PASSED (output: $OUTPUT5)"
else
    echo "✗ Reverse order plugins: FAILED (expected '*** HELLO WORLD!' exit 0, got '$OUTPUT5' exit $EXIT5)"
    exit 1
fi
rm -f hello_test5
echo ""

# Test 6: Plugin that returns NULL (conditional transformation)
echo "Test 6: Plugin that returns NULL (conditional transformation)"
gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -DSILENT_PLUGINS $ASAN_FLAGS -o hello_test6 hello.c plugin.c plugins/null_test_plugin.c 2>&1
OUTPUT6=$(./hello_test6)
EXIT6=$?
# null_test_plugin only transforms messages containing "test", so "Hello world!" should pass through unchanged
if [ "$OUTPUT6" = "Hello world!" ] && [ $EXIT6 -eq 0 ]; then
    echo "✓ NULL return (pass-through): PASSED (output: $OUTPUT6)"
else
    echo "✗ NULL return (pass-through): FAILED (expected 'Hello world!' exit 0, got '$OUTPUT6' exit $EXIT6)"
    exit 1
fi
rm -f hello_test6
echo ""

# Test 7: Chained with NULL-returning plugin
echo "Test 7: Chained with NULL-returning plugin"
gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -DSILENT_PLUGINS $ASAN_FLAGS -o hello_test7 hello.c plugin.c plugins/null_test_plugin.c plugins/example_plugin.c 2>&1
OUTPUT7=$(./hello_test7)
EXIT7=$?
# null_test_plugin returns NULL for "Hello world!", so uppercase should still apply
if [ "$OUTPUT7" = "HELLO WORLD!" ] && [ $EXIT7 -eq 0 ]; then
    echo "✓ NULL plugin in chain: PASSED (output: $OUTPUT7)"
else
    echo "✗ NULL plugin in chain: FAILED (expected 'HELLO WORLD!' exit 0, got '$OUTPUT7' exit $EXIT7)"
    exit 1
fi
rm -f hello_test7
echo ""

echo "=========================================="
echo "All Plugin System Tests: PASSED"
if [ -n "$ASAN_FLAGS" ]; then
    echo "Memory safety: VERIFIED (AddressSanitizer)"
else
    echo "Memory safety: NOT CHECKED (ASAN unavailable)"
    echo "To check for memory leaks, install GCC with ASAN support"
fi
echo "=========================================="
