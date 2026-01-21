#!/bin/bash
<<<<<<< HEAD
# Validation script for hello world program

set -e

QUIET=false
if [ "$1" = "--quiet" ]; then
=======
# Validation script for hello.c
# Ensures the program outputs exactly "Hello world!" with no trailing newline

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

QUIET=false
if [[ "$1" == "--quiet" ]]; then
>>>>>>> main
    QUIET=true
fi

log() {
    if [ "$QUIET" = false ]; then
        echo "$@"
    fi
}

<<<<<<< HEAD
log "=== Validation Tests ==="

# Test 1: Default English output
log "Test 1: Default English output..."
OUTPUT=$(./hello)
EXPECTED="Hello world!"
if [ "$OUTPUT" != "$EXPECTED" ]; then
    echo "ERROR: Expected '$EXPECTED', got '$OUTPUT'"
    exit 1
fi
log "✓ Default English output correct"

# Test 2: Spanish translation
log "Test 2: Spanish translation..."
OUTPUT=$(LANG=es_ES.UTF-8 ./hello)
EXPECTED="¡Hola mundo!"
if [ "$OUTPUT" != "$EXPECTED" ]; then
    echo "ERROR: Expected '$EXPECTED', got '$OUTPUT'"
    exit 1
fi
log "✓ Spanish translation correct"

# Test 3: French translation
log "Test 3: French translation..."
OUTPUT=$(LANG=fr_FR.UTF-8 ./hello)
EXPECTED="Bonjour le monde!"
if [ "$OUTPUT" != "$EXPECTED" ]; then
    echo "ERROR: Expected '$EXPECTED', got '$OUTPUT'"
    exit 1
fi
log "✓ French translation correct"

# Test 4: German translation
log "Test 4: German translation..."
OUTPUT=$(LANG=de_DE.UTF-8 ./hello)
EXPECTED="Hallo Welt!"
if [ "$OUTPUT" != "$EXPECTED" ]; then
    echo "ERROR: Expected '$EXPECTED', got '$OUTPUT'"
    exit 1
fi
log "✓ German translation correct"

# Test 5: Japanese translation
log "Test 5: Japanese translation..."
OUTPUT=$(LANG=ja_JP.UTF-8 ./hello)
EXPECTED="こんにちは世界!"
if [ "$OUTPUT" != "$EXPECTED" ]; then
    echo "ERROR: Expected '$EXPECTED', got '$OUTPUT'"
    exit 1
fi
log "✓ Japanese translation correct"

# Test 6: Command-line language argument
log "Test 6: Command-line --lang argument..."
OUTPUT=$(./hello --lang es_ES.UTF-8)
EXPECTED="¡Hola mundo!"
if [ "$OUTPUT" != "$EXPECTED" ]; then
    echo "ERROR: Expected '$EXPECTED', got '$OUTPUT'"
    exit 1
fi
log "✓ Command-line language argument works"

# Test 7: UTF-8 encoding validation
log "Test 7: UTF-8 encoding validation..."
# Check Spanish special characters
# ¡ (inverted exclamation) should be encoded as 0xC2 0xA1 in UTF-8
SPANISH_HEX=$(LANG=es_ES.UTF-8 ./hello | od -A n -t x1 | tr -d ' \n')
if [[ ! "$SPANISH_HEX" =~ c2a1 ]]; then
    echo "ERROR: Spanish UTF-8 encoding invalid"
    exit 1
fi
log "✓ Spanish UTF-8 encoding valid"

# Check Japanese UTF-8 encoding
# こ (hiragana KO) should be encoded as 0xE3 0x81 0x93 in UTF-8
JAPANESE_HEX=$(LANG=ja_JP.UTF-8 ./hello | od -A n -t x1 | tr -d ' \n')
if [[ ! "$JAPANESE_HEX" =~ e38193 ]]; then
    echo "ERROR: Japanese UTF-8 encoding invalid"
    exit 1
fi
log "✓ Japanese UTF-8 encoding valid"

# Test 8: Exit code check
log "Test 8: Exit code validation..."
./hello > /dev/null
EXIT_CODE=$?
if [ $EXIT_CODE -ne 0 ]; then
    echo "ERROR: Expected exit code 0, got $EXIT_CODE"
    exit 1
fi
log "✓ Exit code correct"

log ""
log "=== All validation tests passed! ==="
=======
log "Starting validation tests for hello.c..."

# Build the program with consistent flags matching Makefile
log "Building hello.c..."
gcc -Wall -Wextra -Wpedantic -Wformat=2 -std=c99 -o hello hello.c

# Test 1: Check basic output
log "Test 1: Checking basic output..."
OUTPUT=$(./hello)
EXPECTED="Hello world!"
if [ "$OUTPUT" = "$EXPECTED" ]; then
    log -e "${GREEN}✓${NC} Output is correct"
else
    echo -e "${RED}✗${NC} Output mismatch!"
    echo "Expected: '$EXPECTED'"
    echo "Got: '$OUTPUT'"
    exit 1
fi

# Test 2: Check exit code
log "Test 2: Checking exit code..."
./hello > /dev/null
EXIT_CODE=$?
if [ $EXIT_CODE -eq 0 ]; then
    log -e "${GREEN}✓${NC} Exit code is correct (0)"
else
    echo -e "${RED}✗${NC} Exit code mismatch!"
    echo "Expected: 0"
    echo "Got: $EXIT_CODE"
    exit 1
fi

# Test 3: Check no trailing newline
log "Test 3: Checking for no trailing newline..."
OUTPUT_HEX=$(./hello | xxd -p)
EXPECTED_HEX="48656c6c6f20776f726c6421"
if [ "$OUTPUT_HEX" = "$EXPECTED_HEX" ]; then
    log -e "${GREEN}✓${NC} No trailing newline (correct)"
else
    echo -e "${RED}✗${NC} Output has unexpected bytes!"
    echo "Expected hex: $EXPECTED_HEX"
    echo "Got hex: $OUTPUT_HEX"
    exit 1
fi

log -e "${GREEN}All validation tests passed!${NC}"
>>>>>>> main
