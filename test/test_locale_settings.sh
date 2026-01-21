#!/bin/bash
# Test program behavior with different locale settings
# Ensures the program works correctly regardless of locale

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT"

echo "=== Testing Different Locale Settings ==="
echo ""

# Compile the program once
rm -f hello_test
gcc -Wall -Wextra -o hello_test hello.c
echo "Program compiled successfully"
echo ""

EXPECTED_OUTPUT="Hello world!"

# Test with various locale settings
LOCALES=("C" "C.UTF-8" "en_US.UTF-8" "POSIX")

for LOCALE in "${LOCALES[@]}"; do
    echo "Testing with locale: $LOCALE"
    
    # Test output with locale set for this test only
    OUTPUT=$(LANG=$LOCALE LC_ALL=$LOCALE LC_CTYPE=$LOCALE ./hello_test)
    if [[ "$OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
        echo "  ✗ ERROR: Output mismatch with locale $LOCALE"
        echo "    Expected: '$EXPECTED_OUTPUT'"
        echo "    Got: '$OUTPUT'"
        rm -f hello_test
        exit 1
    fi
    echo "  ✓ Output correct with locale $LOCALE"
    
    # Test exit code with locale set
    LANG=$LOCALE LC_ALL=$LOCALE LC_CTYPE=$LOCALE ./hello_test > /dev/null
    EXIT_CODE=$?
    if [[ $EXIT_CODE -ne 0 ]]; then
        echo "  ✗ ERROR: Exit code with locale $LOCALE is $EXIT_CODE, expected 0"
        rm -f hello_test
        exit 1
    fi
    echo "  ✓ Exit code correct with locale $LOCALE"
    
    # Verify output encoding (should be ASCII, unaffected by locale)
    OUTPUT_HEX=$(LANG=$LOCALE LC_ALL=$LOCALE LC_CTYPE=$LOCALE ./hello_test | od -A n -t x1 | tr -d ' \n')
    EXPECTED_HEX="48656c6c6f20776f726c64210a"  # "Hello world!\n" in hex
    if [[ "$OUTPUT_HEX" != "$EXPECTED_HEX" ]]; then
        echo "  ✗ ERROR: Byte output affected by locale $LOCALE"
        rm -f hello_test
        exit 1
    fi
    echo "  ✓ Byte encoding unaffected by locale $LOCALE"
    echo ""
done

# Clean up
rm -f hello_test

echo "=== All locale tests passed ==="
exit 0
