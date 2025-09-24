#!/bin/bash
# Security Functions Demonstration Script
# Extracts and demonstrates the security functions from visual-editor.html

echo "Visual Editor Security Functions Demonstration"
echo "=============================================="
echo

echo "1. Input Sanitization Functions:"
echo "================================"

# Extract the escapeForC function
echo "escapeForC Function:"
grep -A 10 "function escapeForC" visual-editor.html | head -12

echo
echo "validateCIdentifier Function:"
grep -A 5 "function validateCIdentifier" visual-editor.html | head -7

echo  
echo "validateCExpression Function:"
grep -A 5 "function validateCExpression" visual-editor.html | head -7

echo
echo "2. Testing the Security Functions:"
echo "=================================="

# Run the Node.js security tests
if command -v node >/dev/null 2>&1; then
    echo "Running security function tests..."
    ./test/test-visual-editor.sh | grep -A 20 "Testing Code Generation Functions"
else
    echo "Node.js not available, showing test file instead:"
    grep -A 20 "escapeForC.*Hello" test/test-visual-editor.sh
fi

echo
echo "3. Generated Code Example:"
echo "========================="
echo "Input: Hello \"world\" %s with newline"
echo "Output: Hello \\\"world\\\" %%s with newline\\n"
echo
echo "This demonstrates proper escaping of:"
echo "- Quotes: \" → \\\""  
echo "- Format specifiers: %s → %%s"
echo "- Newlines: automatically added \\n"

echo
echo "All security functions are implemented in visual-editor.html"
echo "and thoroughly tested in test/test-visual-editor.sh"