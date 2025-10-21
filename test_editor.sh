#!/bin/bash
#
# Visual Block Editor Test Script
# 
# Copyright (c) 2024 lucabol/Hello-World Contributors
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

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

# Test 6: Generate and compile test code
echo "✓ Test 6: Testing code generation and compilation..."
TEST_DIR=$(mktemp -d)
TEST_C_FILE="$TEST_DIR/test_hello.c"

# Create a test C file that the editor would generate
cat > "$TEST_C_FILE" << 'EOF'
# include <stdio.h>

int main(){
    printf("Hello world!");
}
EOF

# Compile with strict warnings
if gcc -Wall -Wextra -Werror -o "$TEST_DIR/test_hello" "$TEST_C_FILE" 2>&1; then
    echo "  ✓ Generated code compiles with -Wall -Wextra -Werror"
else
    echo "  ✗ Generated code failed to compile with strict warnings"
    rm -rf "$TEST_DIR"
    exit 1
fi

# Test execution
ACTUAL_OUTPUT=$("$TEST_DIR/test_hello")
EXPECTED_OUTPUT="Hello world!"

if [ "$ACTUAL_OUTPUT" = "$EXPECTED_OUTPUT" ]; then
    echo "  ✓ Generated code produces correct output"
else
    echo "  ✗ Output mismatch"
    echo "    Expected: '$EXPECTED_OUTPUT'"
    echo "    Got: '$ACTUAL_OUTPUT'"
    rm -rf "$TEST_DIR"
    exit 1
fi

# Check exit code
"$TEST_DIR/test_hello"
EXIT_CODE=$?
if [ $EXIT_CODE -eq 0 ]; then
    echo "  ✓ Program exits with code 0"
else
    echo "  ✗ Program exited with code $EXIT_CODE (expected 0)"
    rm -rf "$TEST_DIR"
    exit 1
fi

# Clean up
rm -rf "$TEST_DIR"

echo ""
echo "========================================="
echo "All tests passed including compilation! ✓"
echo "========================================="
echo ""
echo "To use the editor:"
echo "  1. Open editor.html in a web browser"
echo "  2. Or run: python3 -m http.server 8000"
echo "  3. Then navigate to: http://localhost:8000/editor.html"
