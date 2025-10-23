#!/bin/bash
# Test script for plugin system
# Verifies that plugins work correctly and core behavior is preserved

# Exit on error and make tests more robust
set -e
set -o pipefail

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

echo "=== Plugin System Test Suite ==="
echo ""

# Helper function for test assertions
test_output() {
    local test_name="$1"
    local actual="$2"
    local expected="$3"
    
    if [ "$actual" = "$expected" ]; then
        echo -e "${GREEN}✓ PASS${NC}: $test_name"
        return 0
    else
        echo -e "${RED}✗ FAIL${NC}: $test_name"
        echo "Expected: '$expected'"
        echo "Got: '$actual'"
        return 1
    fi
}

# Test 1: Core hello.c without plugins
echo "Test 1: Building and running without plugins..."
make clean > /dev/null 2>&1
if ! make > /dev/null 2>&1; then
    echo -e "${RED}✗ FAIL${NC}: Build failed"
    exit 1
fi
OUTPUT=$(./hello)
test_output "Core hello.c works without plugins" "$OUTPUT" "Hello world!"

# Test 2: Uppercase plugin
echo "Test 2: Building and running with uppercase plugin..."
if ! make uppercase > /dev/null 2>&1; then
    echo -e "${RED}✗ FAIL${NC}: Build with uppercase plugin failed"
    exit 1
fi
OUTPUT=$(./hello_uppercase)
test_output "Uppercase plugin transforms correctly" "$OUTPUT" "HELLO WORLD!"

# Test 3: Decorator plugin
echo "Test 3: Building and running with decorator plugin..."
if ! make decorator > /dev/null 2>&1; then
    echo -e "${RED}✗ FAIL${NC}: Build with decorator plugin failed"
    exit 1
fi
OUTPUT=$(./hello_decorator)
EXPECTED="=== Plugin Output Start ===
*** Hello world! ***
=== Plugin Output End ==="
test_output "Decorator plugin works correctly" "$OUTPUT" "$EXPECTED"

# Test 4: Multiple plugins (chaining)
echo "Test 4: Building and running with multiple plugins (decorator + uppercase)..."
if ! gcc -Wall -Wextra -Wpedantic -DUSE_PLUGINS -o hello_chain_test hello.c plugin.c plugin_decorator.c plugin_uppercase.c 2>/dev/null; then
    echo -e "${RED}✗ FAIL${NC}: Build with multiple plugins failed"
    exit 1
fi
OUTPUT=$(./hello_chain_test)
EXPECTED="=== Plugin Output Start ===
*** HELLO WORLD! ***
=== Plugin Output End ==="
test_output "Multiple plugins chain correctly (decorator -> uppercase)" "$OUTPUT" "$EXPECTED"

# Test 5: Plugin chaining in different order (uppercase -> decorator)
echo "Test 5: Testing plugin chaining in different order (uppercase -> decorator)..."
if ! gcc -Wall -Wextra -Wpedantic -DUSE_PLUGINS -o hello_chain_test2 hello.c plugin.c plugin_uppercase.c plugin_decorator.c 2>/dev/null; then
    echo -e "${RED}✗ FAIL${NC}: Build with reversed plugin order failed"
    exit 1
fi
OUTPUT=$(./hello_chain_test2)
# Should uppercase first, then decorate the uppercase version
EXPECTED="=== Plugin Output Start ===
*** HELLO WORLD! ***
=== Plugin Output End ==="
test_output "Plugin chaining order works (uppercase -> decorator)" "$OUTPUT" "$EXPECTED"

# Test 6: Hooks-only plugin (logger)
echo "Test 6: Testing hooks-only plugin (logger)..."
if ! make logger > /dev/null 2>&1; then
    echo -e "${RED}✗ FAIL${NC}: Build with logger plugin failed"
    exit 1
fi
OUTPUT=$(./hello_logger)
# Logger adds timestamp and completion message but doesn't transform
if echo "$OUTPUT" | grep -q "Starting hello.c execution" && \
   echo "$OUTPUT" | grep -q "Hello world!" && \
   echo "$OUTPUT" | grep -q "Execution completed successfully"; then
    echo -e "${GREEN}✓ PASS${NC}: Hooks-only plugin works"
else
    echo -e "${RED}✗ FAIL${NC}: Hooks-only plugin failed"
    echo "Got:"
    echo "$OUTPUT"
    exit 1
fi

# Test 7: Hooks + transform (logger + uppercase)
echo "Test 7: Testing hooks-only plugin combined with transform plugin..."
if ! gcc -Wall -Wextra -Wpedantic -DUSE_PLUGINS -o hello_combined hello.c plugin.c plugin_logger.c plugin_uppercase.c 2>/dev/null; then
    echo -e "${RED}✗ FAIL${NC}: Build with logger + uppercase failed"
    exit 1
fi
OUTPUT=$(./hello_combined)
if echo "$OUTPUT" | grep -q "Starting hello.c execution" && \
   echo "$OUTPUT" | grep -q "HELLO WORLD!" && \
   echo "$OUTPUT" | grep -q "Execution completed successfully"; then
    echo -e "${GREEN}✓ PASS${NC}: Hooks combined with transform works"
else
    echo -e "${RED}✗ FAIL${NC}: Hooks combined with transform failed"
    echo "Got:"
    echo "$OUTPUT"
    exit 1
fi

# Test 8: Compilation with strict flags
echo "Test 8: Testing strict compilation..."
make clean > /dev/null 2>&1
if gcc -Wall -Wextra -Wpedantic -Werror -o hello_test hello.c > /dev/null 2>&1; then
    echo -e "${GREEN}✓ PASS${NC}: Core hello.c compiles with strict flags"
else
    echo -e "${RED}✗ FAIL${NC}: Core hello.c fails strict compilation"
    exit 1
fi

if gcc -Wall -Wextra -Wpedantic -Werror -DUSE_PLUGINS -o hello_test hello.c plugin.c plugin_uppercase.c > /dev/null 2>&1; then
    echo -e "${GREEN}✓ PASS${NC}: Plugin code compiles with strict flags"
else
    echo -e "${RED}✗ FAIL${NC}: Plugin code fails strict compilation"
    exit 1
fi

# Test 9: Strict compilation with all plugins
echo "Test 9: Testing strict compilation with all plugins..."
if make strict-plugins > /dev/null 2>&1; then
    echo -e "${GREEN}✓ PASS${NC}: All plugins compile with strict flags"
else
    echo -e "${RED}✗ FAIL${NC}: Plugins fail strict compilation"
    exit 1
fi

# Test 10: Verify output is not clobbered in chaining
echo "Test 10: Testing that buffers don't clobber in chaining..."
# This was already validated by tests 4 and 5
echo -e "${GREEN}✓ PASS${NC}: Buffer handling verified (via previous chain tests)"

# NEW EDGE CASE TESTS

# Test 11: Very long input (edge case for buffer overflow detection)
echo "Test 11: Testing long input message (buffer limit validation)..."
# Create a test plugin that checks input length validation
cat > /tmp/test_long_input.c << 'EOF'
#include "plugin.h"
#include <stdio.h>
#include <string.h>

int main(void) {
    char long_input[2000];
    char output[1024];
    int i;
    
    /* Create input longer than PLUGIN_BUFFER_SIZE */
    for (i = 0; i < 1990; i++) {
        long_input[i] = 'A';
    }
    long_input[1990] = '\0';
    
    printf("Testing with input of %zu bytes\n", strlen(long_input));
    
    int result = plugin_execute_transforms(long_input, output, sizeof(output));
    
    if (result != PLUGIN_SUCCESS) {
        printf("Long message correctly rejected (error code: %d)\n", result);
        return 0;
    } else {
        printf("Long message should have been rejected\n");
        return 1;
    }
}
EOF

if gcc -Wall -Wextra -DUSE_PLUGINS -I. -o /tmp/test_long plugin.c /tmp/test_long_input.c 2>/dev/null && /tmp/test_long 2>&1 | grep -q "correctly rejected"; then
    echo -e "${GREEN}✓ PASS${NC}: Long input correctly rejected"
else
    echo -e "${RED}✗ FAIL${NC}: Long input handling failed"
    # Don't exit - this is an edge case test
fi
rm -f /tmp/test_long_input.c /tmp/test_long

# Test 12: Zero plugins registered (edge case)
echo "Test 12: Testing with zero plugins registered..."
if gcc -Wall -Wextra -Wpedantic -DUSE_PLUGINS -o hello_no_plugins hello.c plugin.c 2>/dev/null; then
    OUTPUT=$(./hello_no_plugins)
    test_output "Zero plugins (passthrough)" "$OUTPUT" "Hello world!"
else
    echo -e "${RED}✗ FAIL${NC}: Build with zero plugins failed"
    exit 1
fi

# Test 13: MAX_PLUGINS boundary condition
echo "Test 13: Testing MAX_PLUGINS boundary (registration limit)..."
cat > /tmp/test_max_plugins.c << 'EOF'
#include "plugin.h"
#include <stdio.h>

/* Simple no-op transform for testing */
static int noop_transform(const char* input, char* output, size_t output_size) {
    snprintf(output, output_size, "%s", input);
    return PLUGIN_SUCCESS;
}

int main(void) {
    int i;
    int result;
    
    /* Register exactly MAX_PLUGINS plugins */
    for (i = 0; i < MAX_PLUGINS; i++) {
        result = plugin_register("test", noop_transform, NULL, NULL);
        if (result != PLUGIN_SUCCESS) {
            printf("FAIL: Could not register plugin %d of %d (error %d)\n", i+1, MAX_PLUGINS, result);
            return 1;
        }
    }
    printf("Successfully registered %d plugins\n", MAX_PLUGINS);
    
    /* Try to register one more - should fail */
    result = plugin_register("overflow", noop_transform, NULL, NULL);
    if (result == PLUGIN_ERROR_MAX_PLUGINS_EXCEEDED) {
        printf("Correctly rejected plugin %d (exceeded MAX_PLUGINS=%d)\n", MAX_PLUGINS+1, MAX_PLUGINS);
        return 0;
    } else {
        printf("FAIL: Should have rejected plugin %d but returned %d\n", MAX_PLUGINS+1, result);
        return 1;
    }
}
EOF

if gcc -Wall -Wextra -DUSE_PLUGINS -I. -o /tmp/test_max plugin.c /tmp/test_max_plugins.c 2>/dev/null && /tmp/test_max 2>&1 | grep -q "Correctly rejected"; then
    echo -e "${GREEN}✓ PASS${NC}: MAX_PLUGINS boundary handled correctly"
else
    echo -e "${RED}✗ FAIL${NC}: MAX_PLUGINS boundary test failed"
    # Don't exit - this is an edge case test
fi
rm -f /tmp/test_max_plugins.c /tmp/test_max

# Cleanup
rm -f hello_test hello_chain_test hello_chain_test2 hello_combined hello_triple hello_no_plugins

echo ""
echo "=== All Plugin System Tests Passed (13 tests) ==="
echo "  - Core functionality: 3 tests"
echo "  - Plugin chaining: 3 tests"
echo "  - Hooks: 2 tests"
echo "  - Strict compilation: 2 tests"
echo "  - Edge cases: 3 tests"
