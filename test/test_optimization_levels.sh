#!/bin/bash
# Test compilation and behavior with different optimization levels
# Tests: -O0, -O1, -O2, -O3, -Os, -Ofast

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT"

echo "=== Testing Different Optimization Levels ==="
echo ""

EXPECTED_OUTPUT="Hello world!"
OPTIMIZATION_LEVELS=("-O0" "-O1" "-O2" "-O3" "-Os" "-Ofast")

for OPT_LEVEL in "${OPTIMIZATION_LEVELS[@]}"; do
    echo "Testing optimization level: $OPT_LEVEL"
    
    # Clean up previous binary
    rm -f hello_test
    
    # Compile with optimization level
    gcc $OPT_LEVEL -Wall -Wextra -o hello_test hello.c
    echo "  ✓ Compilation successful with $OPT_LEVEL"
    
    # Test output
    OUTPUT=$(./hello_test)
    if [[ "$OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
        echo "  ✗ ERROR: Output mismatch with $OPT_LEVEL"
        echo "    Expected: '$EXPECTED_OUTPUT'"
        echo "    Got: '$OUTPUT'"
        rm -f hello_test
        exit 1
    fi
    echo "  ✓ Output correct with $OPT_LEVEL"
    
    # Test exit code
    ./hello_test > /dev/null
    EXIT_CODE=$?
    if [[ $EXIT_CODE -ne 0 ]]; then
        echo "  ✗ ERROR: Exit code with $OPT_LEVEL is $EXIT_CODE, expected 0"
        rm -f hello_test
        exit 1
    fi
    echo "  ✓ Exit code correct with $OPT_LEVEL"
    
    # Test byte-level output (no trailing newline)
    OUTPUT_HEX=$(./hello_test | od -A n -t x1 | tr -d ' \n')
    EXPECTED_HEX="48656c6c6f20776f726c6421"
    if [[ "$OUTPUT_HEX" != "$EXPECTED_HEX" ]]; then
        echo "  ✗ ERROR: Byte output mismatch with $OPT_LEVEL"
        rm -f hello_test
        exit 1
    fi
    echo "  ✓ Byte-level output correct with $OPT_LEVEL"
    
    # Clean up
    rm -f hello_test
    echo ""
done

echo "=== All optimization level tests passed ==="
exit 0
