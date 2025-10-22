#!/bin/bash
# Comprehensive test runner for all tests including framework tests
# Runs unit tests for hello.c and simple_test framework itself

set -e  # Exit on any error
set -u  # Exit on unset variables
set -o pipefail  # Exit on pipeline failures

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Print colored messages
print_info() {
    printf '%b' "${YELLOW}ℹ $1${NC}\n"
}

print_success() {
    printf '%b' "${GREEN}✓ $1${NC}\n"
}

print_error() {
    printf '%b' "${RED}✗ $1${NC}\n"
}

# Change to repository root
cd "$(dirname "$0")/.."

print_info "Running comprehensive test suite..."
echo ""

# Test 1: Run hello.c unit tests
print_info "Test Suite 1: hello.c unit tests"
if ./test/run_unit_tests.sh; then
    print_success "hello.c unit tests passed"
else
    print_error "hello.c unit tests failed"
    exit 1
fi
echo ""

# Test 2: Run simple_test framework self-tests
print_info "Test Suite 2: simple_test framework self-tests"
STRICT_FLAGS="-Wall -Wextra -Wpedantic -Werror -std=c99"

if gcc ${STRICT_FLAGS} -I. -o test_simple_test_runner test/test_simple_test.c test/simple_test.c 2>&1; then
    print_success "Framework tests compiled successfully"
else
    print_error "Failed to compile framework tests"
    exit 1
fi

if ./test_simple_test_runner; then
    print_success "Framework self-tests passed"
else
    print_error "Framework self-tests failed"
    exit 1
fi
echo ""

# Test 3: Test NO_COLOR environment variable
print_info "Test Suite 3: SIMPLE_TEST_NO_COLOR environment variable"

if gcc ${STRICT_FLAGS} -I. -o test_no_color_runner test/test_no_color.c test/simple_test.c 2>&1; then
    print_success "NO_COLOR test compiled successfully"
else
    print_error "Failed to compile NO_COLOR test"
    exit 1
fi

if SIMPLE_TEST_NO_COLOR=1 ./test_no_color_runner; then
    print_success "NO_COLOR=1 correctly disables colors"
else
    print_error "NO_COLOR=1 test failed"
    exit 1
fi
echo ""

# Test 4: Verify no duplicate symbols when including header in multiple files
print_info "Test Suite 4: Multiple includes (no duplicate symbols)"

# Create temporary test files
mkdir -p /tmp/test_includes
cat > /tmp/test_includes/test1.c << 'EOF'
#include "test/simple_test.h"
void test1(void) { TEST_ASSERT_EQUAL_INT(1, 1); }
EOF

cat > /tmp/test_includes/test2.c << 'EOF'
#include "test/simple_test.h"
void test2(void) { TEST_ASSERT_EQUAL_INT(2, 2); }
EOF

cat > /tmp/test_includes/main.c << 'EOF'
#include "test/simple_test.h"
void test1(void);
void test2(void);
int main(void) {
    simple_test_init();
    RUN_TEST(test1);
    RUN_TEST(test2);
    TEST_SUMMARY();
}
EOF

if gcc ${STRICT_FLAGS} -I. -c test/simple_test.c -o /tmp/test_includes/simple_test.o 2>&1 && \
   gcc ${STRICT_FLAGS} -I. -c /tmp/test_includes/test1.c -o /tmp/test_includes/test1.o 2>&1 && \
   gcc ${STRICT_FLAGS} -I. -c /tmp/test_includes/test2.c -o /tmp/test_includes/test2.o 2>&1 && \
   gcc ${STRICT_FLAGS} -I. -c /tmp/test_includes/main.c -o /tmp/test_includes/main.o 2>&1 && \
   gcc -o /tmp/test_includes/multi_test /tmp/test_includes/*.o 2>&1; then
    print_success "Multiple includes compile without duplicate symbols"
else
    print_error "Multiple includes test failed - duplicate symbols detected"
    rm -rf /tmp/test_includes
    exit 1
fi

if /tmp/test_includes/multi_test > /dev/null 2>&1; then
    print_success "Multiple includes test runs successfully"
else
    print_error "Multiple includes test execution failed"
    rm -rf /tmp/test_includes
    exit 1
fi

# Cleanup
rm -rf /tmp/test_includes
echo ""

# All tests passed
print_success "========================================="
print_success "All test suites passed successfully!"
print_success "========================================="
exit 0
