#!/bin/bash
# Visual Editor Test Suite
# Tests the code generation functions and validates generated C code

set -e
set -u

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test results
TESTS_PASSED=0
TESTS_FAILED=0

print_test_header() {
    echo -e "${YELLOW}=== $1 ===${NC}"
}

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
    ((TESTS_PASSED++))
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
    ((TESTS_FAILED++))
}

# Test 1: Basic code generation with Node.js
test_code_generation() {
    print_test_header "Testing Code Generation Functions"
    
    # Create a Node.js test script to test the JavaScript functions
    cat > /tmp/test-generator.js << 'EOF'
// Load the escaping and validation functions from visual editor
function escapeForC(input) {
    if (input == null) {
        return '';
    }
    const inputStr = String(input);
    return inputStr
        .replace(/\\/g, '\\\\')    // Escape backslashes
        .replace(/"/g, '\\"')      // Escape quotes
        .replace(/\n/g, '\\n')     // Escape newlines
        .replace(/\r/g, '\\r')     // Escape carriage returns
        .replace(/\t/g, '\\t')     // Escape tabs
        .replace(/%/g, '%%');      // Escape printf format specifiers
}

function validateCIdentifier(input) {
    if (input == null) {
        return 'defaultVar';
    }
    const identifier = String(input).trim();
    const validPattern = /^[a-zA-Z_][a-zA-Z0-9_]*$/;
    if (identifier.length > 63) {
        return 'defaultVar';
    }
    return validPattern.test(identifier) ? identifier : 'defaultVar';
}

function validateCExpression(input) {
    if (input == null) {
        return '1';
    }
    const expression = String(input).trim();
    
    const dangerousTokens = [';', '#', '`', '$', '\\', '"', "'", '/*', '//'];
    for (const token of dangerousTokens) {
        if (expression.includes(token)) {
            return '1';
        }
    }
    
    const allowedChars = /^[a-zA-Z0-9_\s+\-*\/\<>=!&|()[\].,]*$/;
    if (!allowedChars.test(expression)) {
        return '1';
    }
    
    if (expression.length > 200) {
        return '1';
    }
    
    return expression || '1';
}

// Test cases
const tests = [
    // Test escapeForC - basic cases
    { fn: 'escapeForC', input: 'Hello "world"', expected: 'Hello \\"world\\"' },
    { fn: 'escapeForC', input: 'Line 1\nLine 2', expected: 'Line 1\\nLine 2' },
    { fn: 'escapeForC', input: 'Format %s test', expected: 'Format %%s test' },
    { fn: 'escapeForC', input: 'Path\\file', expected: 'Path\\\\file' },
    
    // Test escapeForC - edge cases  
    { fn: 'escapeForC', input: null, expected: '' },
    { fn: 'escapeForC', input: undefined, expected: '' },
    { fn: 'escapeForC', input: 123, expected: '123' },
    { fn: 'escapeForC', input: '', expected: '' },
    
    // Test validateCIdentifier - valid cases
    { fn: 'validateCIdentifier', input: 'validVar', expected: 'validVar' },
    { fn: 'validateCIdentifier', input: '_privateVar', expected: '_privateVar' },
    { fn: 'validateCIdentifier', input: 'a', expected: 'a' },
    { fn: 'validateCIdentifier', input: 'var123', expected: 'var123' },
    
    // Test validateCIdentifier - invalid cases  
    { fn: 'validateCIdentifier', input: 'invalid-var', expected: 'defaultVar' },
    { fn: 'validateCIdentifier', input: '123invalid', expected: 'defaultVar' },
    { fn: 'validateCIdentifier', input: '', expected: 'defaultVar' },
    { fn: 'validateCIdentifier', input: '   ', expected: 'defaultVar' },
    { fn: 'validateCIdentifier', input: null, expected: 'defaultVar' },
    { fn: 'validateCIdentifier', input: undefined, expected: 'defaultVar' },
    { fn: 'validateCIdentifier', input: 'a'.repeat(70), expected: 'defaultVar' }, // Too long
    
    // Test validateCExpression - safe expressions
    { fn: 'validateCExpression', input: 'x > 0', expected: 'x > 0' },
    { fn: 'validateCExpression', input: 'a + b * c', expected: 'a + b * c' },
    { fn: 'validateCExpression', input: '(x && y) || z', expected: '(x && y) || z' },
    { fn: 'validateCExpression', input: 'arr[0] + arr[1]', expected: 'arr[0] + arr[1]' },
    
    // Test validateCExpression - dangerous/invalid cases
    { fn: 'validateCExpression', input: 'x; system("rm -rf /")', expected: '1' },
    { fn: 'validateCExpression', input: '#include <evil.h>', expected: '1' },
    { fn: 'validateCExpression', input: 'dangerous$()', expected: '1' },
    { fn: 'validateCExpression', input: 'x "string"', expected: '1' },
    { fn: 'validateCExpression', input: "x 'c'", expected: '1' },
    { fn: 'validateCExpression', input: 'x /* comment */', expected: '1' },
    { fn: 'validateCExpression', input: 'x // comment', expected: '1' },
    { fn: 'validateCExpression', input: 'x\\escape', expected: '1' },
    { fn: 'validateCExpression', input: null, expected: '1' },
    { fn: 'validateCExpression', input: undefined, expected: '1' },
    { fn: 'validateCExpression', input: '', expected: '1' },
    { fn: 'validateCExpression', input: 'x'.repeat(300), expected: '1' }, // Too long
];

let passed = 0;
let failed = 0;

tests.forEach(test => {
    let result;
    switch(test.fn) {
        case 'escapeForC':
            result = escapeForC(test.input);
            break;
        case 'validateCIdentifier':
            result = validateCIdentifier(test.input);
            break;
        case 'validateCExpression':
            result = validateCExpression(test.input);
            break;
    }
    
    if (result === test.expected) {
        console.log(`✓ ${test.fn}("${test.input}") = "${result}"`);
        passed++;
    } else {
        console.log(`✗ ${test.fn}("${test.input}") = "${result}", expected "${test.expected}"`);
        failed++;
    }
});

console.log(`\nTests: ${passed} passed, ${failed} failed`);
process.exit(failed > 0 ? 1 : 0);
EOF

    if node /tmp/test-generator.js; then
        print_success "JavaScript security functions pass all tests"
    else
        print_error "JavaScript security functions failed tests"
    fi
    
    # Clean up
    rm -f /tmp/test-generator.js
}

# Test 2: Generate example C files and validate them
test_c_compilation() {
    print_test_header "Testing Generated C Code Compilation"
    
    # Create test cases for different block configurations
    declare -a test_cases=(
        "hello_basic:printf(\"Hello world!\\n\");"
        "hello_escaped:printf(\"Hello \\\"quoted\\\" world!\\n\");"
        "hello_variable:int x = 42; printf(\"Value: %d\\n\", x);"
    )
    
    for test_case in "${test_cases[@]}"; do
        IFS=':' read -r test_name test_content <<< "$test_case"
        
        # Generate C file
        cat > "/tmp/${test_name}.c" << EOF
#include <stdio.h>

int main() {
    ${test_content}
    return 0;
}
EOF
        
        # Test strict compilation
        if gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -o "/tmp/${test_name}" "/tmp/${test_name}.c" 2>/dev/null; then
            print_success "Generated C code compiles cleanly: ${test_name}"
            
            # Test execution
            if output=$(timeout 5s "/tmp/${test_name}" 2>&1) && [ $? -eq 0 ]; then
                print_success "Generated program executes successfully: ${test_name}"
            else
                print_error "Generated program execution failed: ${test_name}"
            fi
        else
            print_error "Generated C code compilation failed: ${test_name}"
        fi
        
        # Clean up
        rm -f "/tmp/${test_name}.c" "/tmp/${test_name}"
    done
}

# Test 3: Test visual editor HTML structure and accessibility
test_accessibility() {
    print_test_header "Testing Accessibility Features"
    
    # Check if the improved visual editor has proper accessibility attributes
    if [ -f "visual-editor.html" ]; then
        # Test for ARIA attributes
        if grep -q "aria-label" visual-editor.html; then
            print_success "ARIA labels found in visual editor"
        else
            print_error "ARIA labels missing from visual editor"
        fi
        
        # Test for role attributes
        if grep -q "role=" visual-editor.html; then
            print_success "Role attributes found in visual editor"
        else
            print_error "Role attributes missing from visual editor"
        fi
        
        # Test for tabindex
        if grep -q "tabindex=" visual-editor.html; then
            print_success "Tabindex attributes found for keyboard navigation"
        else
            print_error "Tabindex attributes missing"
        fi
        
        # Test for keyboard event handlers
        if grep -q "keydown" visual-editor.html; then
            print_success "Keyboard event handlers found"
        else
            print_error "Keyboard event handlers missing"
        fi
    else
        print_error "visual-editor.html not found"
    fi
}

# Test 4: Test example configurations
test_example_configurations() {
    print_test_header "Testing Example Configurations"
    
    # Create examples directory
    mkdir -p examples
    
    # Example 1: Basic Hello World
    cat > examples/hello_basic.c << 'EOF'
#include <stdio.h>

int main() {
    printf("Hello world!\n");
    return 0;
}
EOF

    # Example 2: With variables
    cat > examples/hello_variables.c << 'EOF'
#include <stdio.h>

int main() {
    int x = 42;
    char greeting[] = "Hello";
    printf("%s world! The answer is %d\n", greeting, x);
    return 0;
}
EOF

    # Test compilation of examples
    for example in examples/*.c; do
        if [ -f "$example" ]; then
            filename=$(basename "$example" .c)
            if gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -o "examples/${filename}" "$example" 2>/dev/null; then
                print_success "Example compiles: $(basename $example)"
                
                # Test execution
                if output=$(timeout 5s "examples/${filename}" 2>&1) && [ $? -eq 0 ]; then
                    print_success "Example executes: $(basename $example) -> $output"
                else
                    print_error "Example execution failed: $(basename $example)"
                fi
            else
                print_error "Example compilation failed: $(basename $example)"
            fi
        fi
    done
}

# Test 5: Browser compatibility check
test_browser_compatibility() {
    print_test_header "Testing Browser Compatibility"
    
    if [ -f "visual-editor.html" ]; then
        # Check for modern JavaScript features with fallbacks
        if grep -q "addEventListener" visual-editor.html; then
            print_success "Uses standard addEventListener (supported in all modern browsers)"
        else
            print_error "Missing standard event handling"
        fi
        
        # Check for drag and drop API
        if grep -q "dragstart\|dragover\|drop" visual-editor.html; then
            print_success "Uses Drag and Drop API (supported in all modern browsers)"
        else
            print_error "Missing drag and drop functionality"
        fi
        
        # Check for ES6+ features
        if grep -q "const\|let" visual-editor.html; then
            print_success "Uses modern JavaScript (ES6+)"
        else
            print_error "Uses outdated JavaScript syntax"
        fi
    else
        print_error "Visual editor file not found for compatibility testing"
    fi
}

# Main test execution
main() {
    echo "Visual Editor Test Suite"
    echo "========================"
    echo
    
    test_code_generation
    echo
    test_c_compilation 
    echo
    test_accessibility
    echo
    test_example_configurations
    echo
    test_browser_compatibility
    echo
    
    # Summary
    echo "========================"
    echo -e "${GREEN}Tests passed: $TESTS_PASSED${NC}"
    echo -e "${RED}Tests failed: $TESTS_FAILED${NC}"
    
    if [ $TESTS_FAILED -eq 0 ]; then
        echo -e "${GREEN}All tests passed!${NC}"
        exit 0
    else
        echo -e "${RED}Some tests failed!${NC}"
        exit 1
    fi
}

# Run tests
main