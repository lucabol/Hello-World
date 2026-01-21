#!/bin/bash
# Test compilation with various warning flag combinations
# Ensures code compiles cleanly with strict warning settings

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT"

echo "=== Testing Various Warning Flag Combinations ==="
echo ""

EXPECTED_OUTPUT="Hello world!"

# Test different warning flag combinations
declare -a WARNING_SETS=(
    "-Wall -Wextra"
    "-Wall -Wextra -Wpedantic"
    "-Wall -Wextra -Wpedantic -Werror"
    "-Wall -Wextra -Wpedantic -Wformat=2"
    "-Wall -Wextra -Wpedantic -Wformat=2 -Werror"
    "-Wall -Wextra -Wpedantic -Wshadow"
    "-Wall -Wextra -Wpedantic -Wconversion"
    "-Wall -Wextra -Wpedantic -Wsign-conversion"
    "-Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion"
    "-Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror"
)

for i in "${!WARNING_SETS[@]}"; do
    FLAGS="${WARNING_SETS[$i]}"
    echo "Test $((i+1)): Testing with flags: $FLAGS"
    
    # Clean up previous binary
    rm -f hello_test
    
    # Compile with warning flags
    if ! gcc $FLAGS -o hello_test hello.c 2>&1; then
        echo "  ✗ ERROR: Compilation failed with flags: $FLAGS"
        exit 1
    fi
    echo "  ✓ Compilation successful"
    
    # Test output
    OUTPUT=$(./hello_test)
    if [[ "$OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
        echo "  ✗ ERROR: Output mismatch"
        echo "    Expected: '$EXPECTED_OUTPUT'"
        echo "    Got: '$OUTPUT'"
        rm -f hello_test
        exit 1
    fi
    echo "  ✓ Output correct"
    
    # Clean up
    rm -f hello_test
    echo ""
done

echo "=== Testing with different C standards ==="
echo ""

# Test with different C standards
# Note: C89/C90 may produce warnings about missing return in main(),
# but should still compile and produce correct output
C_STANDARDS=("c89" "c90" "c99" "c11" "c17")

for STD in "${C_STANDARDS[@]}"; do
    echo "Testing with -std=$STD"
    
    rm -f hello_test
    
    # Compile with standard flag (warnings allowed for older standards)
    if ! gcc -std=$STD -Wall -Wextra -Wpedantic -o hello_test hello.c 2>&1; then
        echo "  ✗ ERROR: Compilation failed with -std=$STD"
        exit 1
    fi
    
    # Check if compilation produced a binary
    if [[ ! -f hello_test ]]; then
        echo "  ✗ ERROR: Binary not created with -std=$STD"
        exit 1
    fi
    echo "  ✓ Compilation successful with -std=$STD"
    
    # Test output
    # Note: C89/C90 may return non-zero exit codes without explicit return statement
    # We focus on output correctness for edge case testing
    OUTPUT=$(./hello_test || true)
    if [[ "$OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
        echo "  ✗ ERROR: Output mismatch with -std=$STD"
        rm -f hello_test
        exit 1
    fi
    echo "  ✓ Output correct with -std=$STD"
    
    rm -f hello_test
    echo ""
done

echo "=== Testing with pedantic-errors ==="
echo ""

# Test with pedantic-errors (strictest mode)
echo "Testing with -pedantic-errors..."
rm -f hello_test
if ! gcc -Wall -Wextra -pedantic-errors -o hello_test hello.c 2>&1; then
    echo "✗ ERROR: Compilation failed with -pedantic-errors"
    exit 1
fi
echo "✓ Compilation successful with -pedantic-errors"

OUTPUT=$(./hello_test)
if [[ "$OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
    echo "✗ ERROR: Output mismatch"
    rm -f hello_test
    exit 1
fi
echo "✓ Output correct with -pedantic-errors"
rm -f hello_test
echo ""

echo "=== All warning flag tests passed ==="
exit 0
