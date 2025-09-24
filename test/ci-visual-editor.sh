#!/bin/bash
# CI Integration script for Visual Editor
# Tests generated code from visual editor examples

set -e

echo "Testing Visual Editor Generated Code in CI"
echo "==========================================="

# Test that examples compile with strict flags
for example in examples/*.c; do
    if [ -f "$example" ]; then
        filename=$(basename "$example" .c)
        echo "Testing: $filename"
        
        # Compile with strict flags (matching CI requirements)
        gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 \
            -o "examples/${filename}" "$example"
        
        # Test execution
        output=$(./examples/${filename})
        echo "  Output: $output"
        
        # Clean up
        rm -f "examples/${filename}"
        
        echo "  ✓ $filename passed"
    fi
done

echo "All visual editor examples compile and run successfully!"