#!/bin/bash
# Test buffer flushing behavior
# Ensures output is properly flushed before program termination

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT"

echo "=== Testing Buffer Flushing Behavior ==="
echo ""

# Compile the program
rm -f hello_test
gcc -Wall -Wextra -o hello_test hello.c
echo "Program compiled successfully"
echo ""

EXPECTED_OUTPUT="Hello world!"

# Test 1: Test output with immediate redirection (tests auto-flush on exit)
echo "Test 1: Testing output with immediate file redirection..."
rm -f output.txt
./hello_test > output.txt
OUTPUT=$(cat output.txt)
if [[ "$OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
    echo "✗ ERROR: Output not flushed to file"
    echo "  Expected: '$EXPECTED_OUTPUT'"
    echo "  Got: '$OUTPUT'"
    rm -f hello_test output.txt
    exit 1
fi
echo "✓ Output properly flushed to file on exit"
rm -f output.txt
echo ""

# Test 2: Test output through pipe (tests buffer flushing through pipe)
echo "Test 2: Testing output through pipe..."
OUTPUT=$(./hello_test | cat)
if [[ "$OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
    echo "✗ ERROR: Output not properly flushed through pipe"
    echo "  Expected: '$EXPECTED_OUTPUT'"
    echo "  Got: '$OUTPUT'"
    rm -f hello_test
    exit 1
fi
echo "✓ Output properly flushed through pipe"
echo ""

# Test 3: Test with multiple pipe stages
echo "Test 3: Testing output through multiple pipe stages..."
OUTPUT=$(./hello_test | cat | cat | cat)
if [[ "$OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
    echo "✗ ERROR: Output not properly flushed through multiple pipes"
    echo "  Expected: '$EXPECTED_OUTPUT'"
    echo "  Got: '$OUTPUT'"
    rm -f hello_test
    exit 1
fi
echo "✓ Output properly flushed through multiple pipes"
echo ""

# Test 4: Test with command substitution (requires proper flushing)
echo "Test 4: Testing output with command substitution..."
OUTPUT=$(./hello_test)
if [[ "$OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
    echo "✗ ERROR: Output not captured in command substitution"
    echo "  Expected: '$EXPECTED_OUTPUT'"
    echo "  Got: '$OUTPUT'"
    rm -f hello_test
    exit 1
fi
echo "✓ Output properly captured in command substitution"
echo ""

# Test 5: Test output consistency across multiple runs
echo "Test 5: Testing output consistency across multiple runs..."
for i in {1..10}; do
    OUTPUT=$(./hello_test)
    if [[ "$OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
        echo "✗ ERROR: Inconsistent output on run $i"
        echo "  Expected: '$EXPECTED_OUTPUT'"
        echo "  Got: '$OUTPUT'"
        rm -f hello_test
        exit 1
    fi
done
echo "✓ Output consistent across 10 runs"
echo ""

# Test 6: Test that output is complete (no partial writes)
echo "Test 6: Testing complete output (no partial writes)..."
for i in {1..20}; do
    OUTPUT=$(./hello_test)
    OUTPUT_LENGTH=${#OUTPUT}
    EXPECTED_LENGTH=${#EXPECTED_OUTPUT}
    if [[ $OUTPUT_LENGTH -ne $EXPECTED_LENGTH ]]; then
        echo "✗ ERROR: Partial output detected on run $i"
        echo "  Expected length: $EXPECTED_LENGTH"
        echo "  Got length: $OUTPUT_LENGTH"
        rm -f hello_test
        exit 1
    fi
done
echo "✓ Complete output verified across 20 runs"
echo ""

# Clean up
rm -f hello_test

echo "=== All buffer flushing tests passed ==="
exit 0
