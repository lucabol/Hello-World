#!/bin/bash

# Test script for the visual block editor
# This script verifies the editor.html file exists and is properly structured

set -e

echo "Testing Visual Block Editor..."
echo ""

# Test 1: Check if editor.html exists
echo "✓ Test 1: Checking if editor.html exists..."
if [ -f "editor.html" ]; then
    echo "  ✓ editor.html found"
else
    echo "  ✗ editor.html not found"
    exit 1
fi

# Test 2: Check if editor.html is valid HTML
echo "✓ Test 2: Checking if editor.html is valid HTML..."
if file editor.html | grep -q "HTML"; then
    echo "  ✓ editor.html is valid HTML"
else
    echo "  ✗ editor.html is not valid HTML"
    exit 1
fi

# Test 3: Check if editor.html contains required elements
echo "✓ Test 3: Checking for required editor components..."
required_elements=(
    "C Code Block Editor"
    "toolbox"
    "blockArea"
    "generatedCode"
    "loadExample"
    "exportToFile"
)

for element in "${required_elements[@]}"; do
    if grep -q "$element" editor.html; then
        echo "  ✓ Found: $element"
    else
        echo "  ✗ Missing: $element"
        exit 1
    fi
done

# Test 4: Check if EDITOR_GUIDE.md exists
echo "✓ Test 4: Checking if documentation exists..."
if [ -f "EDITOR_GUIDE.md" ]; then
    echo "  ✓ EDITOR_GUIDE.md found"
else
    echo "  ✗ EDITOR_GUIDE.md not found"
    exit 1
fi

# Test 5: Check if README mentions the editor
echo "✓ Test 5: Checking if README.md mentions the editor..."
if grep -q "Visual Block Editor" README.md; then
    echo "  ✓ README.md mentions Visual Block Editor"
else
    echo "  ✗ README.md does not mention Visual Block Editor"
    exit 1
fi

echo ""
echo "========================================="
echo "All tests passed! ✓"
echo "========================================="
echo ""
echo "To use the editor:"
echo "  1. Open editor.html in a web browser"
echo "  2. Or run: python3 -m http.server 8000"
echo "  3. Then navigate to: http://localhost:8000/editor.html"
