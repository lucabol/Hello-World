#!/bin/bash
# Test output stream redirection (stdout vs stderr)
# Ensures output goes to stdout, not stderr

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT"

echo "=== Testing Output Stream Redirection ==="
echo ""

# Compile the program
rm -f hello_test
gcc -Wall -Wextra -o hello_test hello.c
echo "Program compiled successfully"
echo ""

EXPECTED_OUTPUT="Hello world!"

# Test 1: Verify output goes to stdout
echo "Test 1: Verifying output goes to stdout..."
STDOUT_OUTPUT=$(./hello_test 2>/dev/null)
if [[ "$STDOUT_OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
    echo "✗ ERROR: stdout output mismatch"
    echo "  Expected: '$EXPECTED_OUTPUT'"
    echo "  Got: '$STDOUT_OUTPUT'"
    rm -f hello_test
    exit 1
fi
echo "✓ Output correctly sent to stdout"
echo ""

# Test 2: Verify nothing goes to stderr
echo "Test 2: Verifying nothing goes to stderr..."
STDERR_OUTPUT=$(./hello_test 2>&1 1>/dev/null)
if [[ -n "$STDERR_OUTPUT" ]]; then
    echo "✗ ERROR: Unexpected output on stderr"
    echo "  Got: '$STDERR_OUTPUT'"
    rm -f hello_test
    exit 1
fi
echo "✓ No output sent to stderr"
echo ""

# Test 3: Verify stdout can be redirected to file
echo "Test 3: Testing stdout redirection to file..."
rm -f test_output.txt
./hello_test > test_output.txt
FILE_CONTENT=$(cat test_output.txt)
if [[ "$FILE_CONTENT" != "$EXPECTED_OUTPUT" ]]; then
    echo "✗ ERROR: File content mismatch after redirection"
    echo "  Expected: '$EXPECTED_OUTPUT'"
    echo "  Got: '$FILE_CONTENT'"
    rm -f hello_test test_output.txt
    exit 1
fi
echo "✓ stdout successfully redirected to file"
rm -f test_output.txt
echo ""

# Test 4: Verify stdout can be piped
echo "Test 4: Testing stdout piping..."
PIPED_OUTPUT=$(./hello_test | cat)
if [[ "$PIPED_OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
    echo "✗ ERROR: Piped output mismatch"
    echo "  Expected: '$EXPECTED_OUTPUT'"
    echo "  Got: '$PIPED_OUTPUT'"
    rm -f hello_test
    exit 1
fi
echo "✓ stdout successfully piped"
echo ""

# Test 5: Verify output with both streams redirected
echo "Test 5: Testing with both stdout and stderr redirected..."
rm -f stdout.txt stderr.txt
./hello_test > stdout.txt 2> stderr.txt
STDOUT_CONTENT=$(cat stdout.txt)
STDERR_CONTENT=$(cat stderr.txt)
if [[ "$STDOUT_CONTENT" != "$EXPECTED_OUTPUT" ]]; then
    echo "✗ ERROR: stdout redirection failed"
    rm -f hello_test stdout.txt stderr.txt
    exit 1
fi
if [[ -n "$STDERR_CONTENT" ]]; then
    echo "✗ ERROR: stderr should be empty"
    rm -f hello_test stdout.txt stderr.txt
    exit 1
fi
echo "✓ Both streams correctly redirected"
rm -f stdout.txt stderr.txt
echo ""

# Clean up
rm -f hello_test

echo "=== All output stream tests passed ==="
exit 0
