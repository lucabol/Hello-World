#!/bin/bash
# Top-level validation script wrapper
# Validates a specific binary or calls the comprehensive test suite
#
# Usage: ./validate.sh [binary_path]
#        If binary_path is provided, validates that specific binary
#        Otherwise calls the test suite validation script

if [ "$#" -eq 1 ]; then
    # Validate specific binary
    BINARY="$1"
    EXPECTED_OUTPUT="Hello world!"
    
    echo "Validating $BINARY..."
    
    # Check if binary exists
    if [[ ! -f "$BINARY" ]]; then
        echo "Error: Binary $BINARY does not exist"
        exit 1
    fi
    
    # Run the program and capture output
    OUTPUT=$("$BINARY" 2>&1)
    EXIT_CODE=$?
    
    # Check exit code
    if [[ $EXIT_CODE -ne 0 ]]; then
        echo "Error: Program exited with code $EXIT_CODE, expected 0"
        exit 1
    fi
    
    # Check output
    if [[ "$OUTPUT" != "$EXPECTED_OUTPUT" ]]; then
        echo "Error: Output mismatch!"
        echo "Expected: '$EXPECTED_OUTPUT'"
        echo "Actual:   '$OUTPUT'"
        exit 1
    fi
    
    echo "✓ Validation passed for $BINARY"
    exit 0
else
    # Call the comprehensive test suite if it exists
    if [[ -f "test/validate.sh" ]]; then
        exec "$(dirname "$0")/test/validate.sh" "$@"
    else
        echo "Error: No specific binary provided and test/validate.sh not found"
        echo "Usage: $0 [binary_path]"
        exit 1
    fi
fi