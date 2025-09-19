#!/bin/bash

# format.sh - Format C source files using clang-format
#
# This script formats all C source files in the current directory
# using the project's .clang-format configuration.

set -e

# Check if clang-format is available
if ! command -v clang-format &> /dev/null; then
    echo "Error: clang-format is not installed or not in PATH"
    echo "Please install clang-format to use this script"
    exit 1
fi

# Check if .clang-format exists
if [[ ! -f .clang-format ]]; then
    echo "Error: .clang-format configuration file not found"
    echo "Please run this script from the project root directory"
    exit 1
fi

echo "Formatting C source files..."

# Format all .c and .h files
find . -name "*.c" -o -name "*.h" | while read -r file; do
    echo "Formatting: $file"
    clang-format -i "$file"
done

echo "Formatting complete!"

# Verify formatting
echo "Verifying formatting..."
if find . -name "*.c" -o -name "*.h" | xargs -r clang-format --dry-run --Werror 2>/dev/null; then
    echo "✓ All files are properly formatted"
else
    echo "⚠ Some formatting issues detected. Please check the output above."
    exit 1
fi