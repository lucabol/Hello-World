#!/bin/bash
# Test script to verify code formatting compliance with clang-format

set -e

echo "=== Testing Code Formatting Compliance ==="

# Check if clang-format is installed
if ! command -v clang-format >/dev/null 2>&1; then
    echo "ERROR: clang-format is not installed"
    echo "Please install clang-format to run formatting checks"
    exit 1
fi

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Track results
FAILED=0

# Find all C source files
C_FILES=$(find . -name "*.c" -not -path "./test/*" -not -path "./.git/*")

if [ -z "$C_FILES" ]; then
    echo -e "${RED}ERROR: No C source files found${NC}"
    exit 1
fi

echo "Checking formatting for C files..."

# Check each file
for file in $C_FILES; do
    echo -n "  Checking $file... "
    
    # Run clang-format and compare with original
    if clang-format "$file" | diff -u "$file" - > /dev/null 2>&1; then
        echo -e "${GREEN}OK${NC}"
    else
        echo -e "${RED}FAILED${NC}"
        echo -e "${RED}    File is not properly formatted${NC}"
        echo "    Run 'make format' to fix formatting issues"
        FAILED=1
    fi
done

# Report results
echo ""
if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}=== All files are properly formatted ===${NC}"
    exit 0
else
    echo -e "${RED}=== Formatting check failed ===${NC}"
    echo "Run 'make format' to automatically format all files"
    exit 1
fi
