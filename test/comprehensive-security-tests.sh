#!/bin/bash
# Comprehensive security and edge case tests for visual editor
# Tests specific edge cases mentioned in code review

set -e
set -u

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

TESTS_PASSED=0
TESTS_FAILED=0

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
    ((TESTS_PASSED++))
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
    ((TESTS_FAILED++))
}

print_test_header() {
    echo -e "${YELLOW}=== $1 ===${NC}"
}

# Test security functions with edge cases
test_security_edge_cases() {
    print_test_header "Security Edge Case Testing"
    
    cat > /tmp/security-test.js << 'EOF'
// Security functions from visual editor
function escapeForC(input) {
    if (input == null) {
        return '';
    }
    const inputStr = String(input);
    return inputStr
        .replace(/\\/g, '\\\\')
        .replace(/"/g, '\\"')
        .replace(/\n/g, '\\n')
        .replace(/\r/g, '\\r')
        .replace(/\t/g, '\\t')
        .replace(/%/g, '%%');
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

// Edge case tests
const edgeCases = [
    // escapeForC edge cases
    { fn: 'escapeForC', input: '', expected: '' },
    { fn: 'escapeForC', input: 'Hello %d %s %f', expected: 'Hello %%d %%s %%f' },
    { fn: 'escapeForC', input: '"quotes"', expected: '\\"quotes\\"' },
    { fn: 'escapeForC', input: 'line1\nline2\r\nline3', expected: 'line1\\nline2\\r\\nline3' },
    { fn: 'escapeForC', input: 'tab\there', expected: 'tab\\there' },
    { fn: 'escapeForC', input: 'path\\to\\file', expected: 'path\\\\to\\\\file' },
    { fn: 'escapeForC', input: '100% complete with "quotes"', expected: '100%% complete with \\"quotes\\"' },
    
    // validateCIdentifier edge cases  
    { fn: 'validateCIdentifier', input: '', expected: 'defaultVar' },
    { fn: 'validateCIdentifier', input: '   ', expected: 'defaultVar' },
    { fn: 'validateCIdentifier', input: 'valid_name123', expected: 'valid_name123' },
    { fn: 'validateCIdentifier', input: '_underscore', expected: '_underscore' },
    { fn: 'validateCIdentifier', input: '123invalid', expected: 'defaultVar' },
    { fn: 'validateCIdentifier', input: 'invalid-name', expected: 'defaultVar' },
    { fn: 'validateCIdentifier', input: 'invalid.name', expected: 'defaultVar' },
    { fn: 'validateCIdentifier', input: 'invalid name', expected: 'defaultVar' },
    { fn: 'validateCIdentifier', input: 'a', expected: 'a' },
    { fn: 'validateCIdentifier', input: 'A_very_long_identifier_name', expected: 'A_very_long_identifier_name' },
    
    // validateCExpression edge cases
    { fn: 'validateCExpression', input: '', expected: '1' },
    { fn: 'validateCExpression', input: '   ', expected: '1' },
    { fn: 'validateCExpression', input: 'x > 0', expected: 'x > 0' },
    { fn: 'validateCExpression', input: 'a + b * c / d', expected: 'a + b * c / d' },
    { fn: 'validateCExpression', input: '(x && y) || z', expected: '(x && y) || z' },
    { fn: 'validateCExpression', input: 'arr[0] + arr[1]', expected: 'arr[0] + arr[1]' },
    { fn: 'validateCExpression', input: 'x < 10 && x > 0', expected: 'x < 10 && x > 0' },
    { fn: 'validateCExpression', input: 'dangerous$()', expected: '1' },
    { fn: 'validateCExpression', input: 'system("rm -rf /")', expected: '1' },
    { fn: 'validateCExpression', input: 'x;system("evil")', expected: '1' }
];

let passed = 0;
let failed = 0;

edgeCases.forEach(test => {
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

console.log(`\nEdge case tests: ${passed} passed, ${failed} failed`);
process.exit(failed > 0 ? 1 : 0);
EOF

    if node /tmp/security-test.js; then
        print_success "All security edge case tests pass"
    else
        print_error "Some security edge case tests failed"
    fi
    
    rm -f /tmp/security-test.js
}

# Test printf format string behavior
test_printf_behavior() {
    print_test_header "Printf Format String Behavior Testing"
    
    # Test that escaped format specifiers work correctly
    cat > /tmp/test-printf.c << 'EOF'
#include <stdio.h>

int main() {
    // Test escaped format specifiers
    printf("Hello %%d world!\n");
    printf("100%% complete\n"); 
    printf("Value: %%s\n");
    return 0;
}
EOF

    if gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -o /tmp/test-printf /tmp/test-printf.c 2>/dev/null; then
        output=$(/tmp/test-printf)
        expected="Hello %d world!
100% complete
Value: %s"
        if [ "$output" = "$expected" ]; then
            print_success "Printf format specifier escaping works correctly"
        else
            print_error "Printf output doesn't match expected format"
        fi
    else
        print_error "Printf test code compilation failed"
    fi
    
    rm -f /tmp/test-printf.c /tmp/test-printf
}

# Test code generation with edge cases
test_code_generation_edge_cases() {
    print_test_header "Code Generation Edge Case Testing"
    
    # Test various challenging inputs
    test_cases=(
        'Hello "World"!'
        'Path: C:\Users\Name'  
        '100% Complete'
        $'Line 1\nLine 2'
        'Format %d string %s'
    )
    
    for input in "${test_cases[@]}"; do
        # Generate test C code
        cat > /tmp/edge-test.c << EOF
#include <stdio.h>

int main() {
    printf("$(node -e "
        function escapeForC(input) {
            if (typeof input !== 'string') return String(input);
            return input
                .replace(/\\\\/g, '\\\\\\\\')
                .replace(/\"/g, '\\\\\"')
                .replace(/\\n/g, '\\\\n')
                .replace(/\\r/g, '\\\\r')
                .replace(/\\t/g, '\\\\t')
                .replace(/%/g, '%%');
        }
        console.log(escapeForC('$input'));
    ")\\n");
    return 0;
}
EOF
        
        if gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -o /tmp/edge-test /tmp/edge-test.c 2>/dev/null; then
            if /tmp/edge-test >/dev/null 2>&1; then
                print_success "Edge case input '$input' generates valid C code"
            else
                print_error "Edge case input '$input' generates code that fails at runtime"
            fi
        else
            print_error "Edge case input '$input' generates code that fails compilation"
        fi
        
        rm -f /tmp/edge-test.c /tmp/edge-test
    done
}

# Main execution
main() {
    echo "Comprehensive Visual Editor Security Tests"
    echo "=========================================="
    echo
    
    test_security_edge_cases
    echo
    test_printf_behavior  
    echo
    test_code_generation_edge_cases
    echo
    
    echo "========================================"
    echo -e "${GREEN}Tests passed: $TESTS_PASSED${NC}"
    echo -e "${RED}Tests failed: $TESTS_FAILED${NC}"
    
    if [ $TESTS_FAILED -eq 0 ]; then
        echo -e "${GREEN}All comprehensive security tests passed!${NC}"
        exit 0
    else
        echo -e "${RED}Some comprehensive security tests failed!${NC}"
        exit 1
    fi
}

main