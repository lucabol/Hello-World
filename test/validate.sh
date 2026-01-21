#!/bin/bash
# Validation script for Hello World program
# Validates compilation, output, and exit code

set -e

QUIET=0
if [ "$1" = "--quiet" ]; then
    QUIET=1
fi

log() {
    if [ $QUIET -eq 0 ]; then
        echo "$@"
    fi
}

# Colors for output (only if not quiet)
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

log "=== Hello World Validation ==="
log ""

# Create secure temporary directory for test files
TEMP_DIR=$(mktemp -d)
trap "rm -rf $TEMP_DIR" EXIT

# Test 1: Compile with strict flags
log "Test 1: Compiling with strict flags..."
TEST_BIN="$(pwd)/hello_test"
if gcc -Wall -Wextra -Wpedantic -Werror -o "$TEST_BIN" hello.c 2>&1 | tee $TEMP_DIR/compile.log; then
    log "${GREEN}✓${NC} Compilation successful"
else
    echo "${RED}✗${NC} Compilation failed"
    cat $TEMP_DIR/compile.log
    exit 1
fi

# Test 2: Run program and capture output
log ""
log "Test 2: Running program and checking output..."
OUTPUT=$("$TEST_BIN")
EXPECTED="Hello world!"

if [ "$OUTPUT" = "$EXPECTED" ]; then
    log "${GREEN}✓${NC} Output matches expected: '$EXPECTED'"
else
    echo "${RED}✗${NC} Output mismatch"
    echo "  Expected: '$EXPECTED'"
    echo "  Got:      '$OUTPUT'"
    exit 1
fi

# Test 3: Check exit code
log ""
log "Test 3: Checking exit code..."
"$TEST_BIN" > /dev/null
EXIT_CODE=$?

if [ $EXIT_CODE -eq 0 ]; then
    log "${GREEN}✓${NC} Exit code is 0"
else
    echo "${RED}✗${NC} Exit code is $EXIT_CODE (expected 0)"
    exit 1
fi

# Test 4: Verify exact output (no trailing newline)
log ""
log "Test 4: Verifying byte-level output..."
"$TEST_BIN" | xxd > $TEMP_DIR/output.hex
echo -n "Hello world!" | xxd > $TEMP_DIR/expected.hex

if diff -q $TEMP_DIR/output.hex $TEMP_DIR/expected.hex > /dev/null 2>&1; then
    log "${GREEN}✓${NC} Byte-level output is correct (no trailing newline)"
else
    echo "${RED}✗${NC} Byte-level output mismatch"
    echo "Expected hex:"
    cat $TEMP_DIR/expected.hex
    echo "Got hex:"
    cat $TEMP_DIR/output.hex
    exit 1
fi

# Cleanup
rm -f "$TEST_BIN"

log ""
log "${GREEN}=== All validation tests passed! ===${NC}"
