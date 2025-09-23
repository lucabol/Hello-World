#!/bin/bash
<<<<<<< HEAD

# Validation script for Hello World program
# Validates program output and exit code against expected values

set -e

BINARY="${1:-./hello}"
EXPECTED_OUTPUT="Hello world!

Exit code: 0"
EXPECTED_EXIT_CODE=0

echo "=== Hello World Program Validation ==="
echo "Binary: $BINARY"
echo "Expected output: '$EXPECTED_OUTPUT'"
echo "Expected exit code: $EXPECTED_EXIT_CODE"
echo ""

# Check if binary exists
if [[ ! -f "$BINARY" ]]; then
    echo "ERROR: Binary '$BINARY' not found"
    exit 1
fi

# Check if binary is executable
if [[ ! -x "$BINARY" ]]; then
    echo "ERROR: Binary '$BINARY' is not executable"
    exit 1
fi

echo "=== Running validation ==="

# Capture output and exit code
set +e  # Temporarily disable exit on error to capture exit code
ACTUAL_OUTPUT=$("$BINARY" 2>&1)
ACTUAL_EXIT_CODE=$?
set -e  # Re-enable exit on error

echo "Actual output: '$ACTUAL_OUTPUT'"
echo "Actual exit code: $ACTUAL_EXIT_CODE"
echo ""

# Validate exit code
if [[ $ACTUAL_EXIT_CODE -ne $EXPECTED_EXIT_CODE ]]; then
    echo "FAIL: Exit code mismatch"
    echo "  Expected: $EXPECTED_EXIT_CODE"
    echo "  Actual: $ACTUAL_EXIT_CODE"
    exit 1
fi

# Validate output
if [[ "$ACTUAL_OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
    echo "FAIL: Output mismatch"
    echo "  Expected: '$EXPECTED_OUTPUT'"
    echo "  Actual: '$ACTUAL_OUTPUT'"
    echo ""
    echo "Character-by-character comparison:"
    echo "Expected (hex): $(echo -n "$EXPECTED_OUTPUT" | od -An -tx1 | tr -d ' ')"
    echo "Actual (hex):   $(echo -n "$ACTUAL_OUTPUT" | od -An -tx1 | tr -d ' ')"
    exit 1
fi

echo "SUCCESS: All validations passed!"
echo "  ✓ Output matches expected: '$EXPECTED_OUTPUT'"
echo "  ✓ Exit code matches expected: $EXPECTED_EXIT_CODE"
=======
# Top-level validation script wrapper
# Calls the actual validation script in the test directory
#
# This script provides compatibility with both the simple validation
# and the comprehensive test suite in the test/ directory.

exec "$(dirname "$0")/test/validate.sh" "$@"
>>>>>>> 3467337fe53fa4b352ac3706bfa2889cec515d5a
