#!/bin/bash
# Lightweight test script to check for common pitfalls
# This script performs quick checks without full compilation
# Usage: ./test/quick-check.sh

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

print_success() {
    printf '%b' "${GREEN}✓ $1${NC}\n"
}

print_error() {
    printf '%b' "${RED}✗ $1${NC}\n"
}

print_info() {
    printf '%b' "${YELLOW}ℹ $1${NC}\n"
}

print_info "Running quick checks for common pitfalls..."

# Check 1: Presence of #include <stdio.h> (allowing for spaces)
if ! grep -q "#\s*include\s*<stdio.h>" hello.c; then
    print_error "Missing #include <stdio.h> in hello.c"
    exit 1
fi
print_success "Found required #include <stdio.h>"

# Check 2: Presence of return value (main function should have return type int)
if ! grep -q "int main" hello.c; then
    print_error "main() function should return int"
    exit 1
fi
print_success "main() function has correct return type"

# Check 3: Check if file ends with trailing newline (it should NOT for this program)
# The hello.c file itself should follow standard conventions (end with newline)
# but the program output should NOT have trailing newline
if [[ -n "$(tail -c1 hello.c)" ]]; then
    print_info "hello.c does not end with newline (acceptable)"
else
    print_success "hello.c follows standard format (ends with newline)"
fi

# Check 4: Basic syntax check by attempting to compile
print_info "Performing basic syntax check..."
if gcc -fsyntax-only hello.c 2>/dev/null; then
    print_success "Basic syntax check passed"
else
    print_error "Syntax errors found in hello.c"
    exit 1
fi

# Check 5: Check that printf format is correct
if grep -q 'printf("Hello world!")' hello.c; then
    print_success "Found expected printf statement"
else
    print_error "Expected 'printf(\"Hello world!\")' not found"
    exit 1
fi

print_success "All quick checks passed!"
print_info "Run 'make test' or 'bash test/validate.sh' for full validation"