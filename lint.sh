#!/bin/bash

# lint.sh - Run compiler warnings and checks on C source files
#
# This script compiles the C source files with strict warning flags
# to catch potential issues. It treats warnings as errors.

set -e

# Configuration
SOURCE_FILES="hello.c"
OUTPUT_BINARY="hello_lint_check"
GCC_FLAGS="-Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror"

echo "Running lint checks with strict compiler warnings..."

# Check if gcc is available
if ! command -v gcc &> /dev/null; then
    echo "Error: gcc is not installed or not in PATH"
    exit 1
fi

# Compile with strict warnings
echo "Compiling with flags: $GCC_FLAGS"
if gcc $GCC_FLAGS -o "$OUTPUT_BINARY" $SOURCE_FILES; then
    echo "✓ No compiler warnings detected"
    
    # Clean up the test binary
    rm -f "$OUTPUT_BINARY"
    
    echo "✓ Lint check passed successfully"
else
    echo "✗ Lint check failed: compiler warnings or errors detected"
    echo ""
    echo "Please fix the warnings above and run the lint check again."
    echo "You can also run the formatter first: ./format.sh"
    
    # Clean up any partial binary
    rm -f "$OUTPUT_BINARY"
    exit 1
fi

# Optional: Also check with clang if available
if command -v clang &> /dev/null; then
    echo ""
    echo "Also checking with clang..."
    if clang $GCC_FLAGS -o "${OUTPUT_BINARY}_clang" $SOURCE_FILES; then
        echo "✓ Clang check passed"
        rm -f "${OUTPUT_BINARY}_clang"
    else
        echo "⚠ Clang detected additional issues"
        rm -f "${OUTPUT_BINARY}_clang"
        exit 1
    fi
fi