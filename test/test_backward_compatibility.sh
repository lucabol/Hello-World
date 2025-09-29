#!/bin/bash
# Test script to verify exact backward compatibility
# Ensures that ./hello outputs exactly "Hello world!" with no trailing newline and exit code 0

set -e

EXPECTED_OUTPUT="Hello world!"
EXPECTED_EXIT_CODE=0

# Temporarily disable plugins by renaming directory
if [ -d "plugins" ]; then
    mv plugins plugins_temp_disabled
    PLUGINS_RESTORED=false
else
    PLUGINS_RESTORED=true
fi

# Function to restore plugins on exit
cleanup() {
    if [ "$PLUGINS_RESTORED" = "false" ]; then
        mv plugins_temp_disabled plugins
    fi
}
trap cleanup EXIT

# Run hello and capture output and exit code
set +e
OUTPUT=$(./hello 2>&1)
EXIT_CODE=$?
set -e

# Verify exit code
if [ "$EXIT_CODE" != "$EXPECTED_EXIT_CODE" ]; then
    echo "FAIL: Expected exit code $EXPECTED_EXIT_CODE, got $EXIT_CODE"
    exit 1
fi

# Verify exact output (no trailing newline)
if [ "$OUTPUT" != "$EXPECTED_OUTPUT" ]; then
    echo "FAIL: Output mismatch"
    echo "Expected: '$EXPECTED_OUTPUT'"
    echo "Actual:   '$OUTPUT'"
    echo "Expected length: ${#EXPECTED_OUTPUT}"
    echo "Actual length:   ${#OUTPUT}"
    exit 1
fi

echo "PASS: Backward compatibility verified"
echo "      Output: '$OUTPUT'"
echo "      Exit code: $EXIT_CODE"