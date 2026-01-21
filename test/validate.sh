#!/bin/bash
# Validation script for hello world program

set -e

QUIET=false
if [ "$1" = "--quiet" ]; then
    QUIET=true
fi

log() {
    if [ "$QUIET" = false ]; then
        echo "$@"
    fi
}

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
# Check Spanish special characters (¡)
SPANISH_HEX=$(LANG=es_ES.UTF-8 ./hello | od -A n -t x1 | tr -d ' \n')
if [[ ! "$SPANISH_HEX" =~ c2a1 ]]; then
    echo "ERROR: Spanish UTF-8 encoding invalid"
    exit 1
fi
log "✓ Spanish UTF-8 encoding valid"

# Check Japanese UTF-8 encoding (こ)
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
