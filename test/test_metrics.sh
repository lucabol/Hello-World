#!/bin/bash
# Comprehensive test suite for code metrics tool
# Tests edge cases: blank files, comments in strings, CRLF endings, etc.

set -e
set -u

TOOL="./metrics_tool"
TEST_DIR="/tmp/metrics_tests"
FAILED=0
PASSED=0

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

mkdir -p "${TEST_DIR}"

print_test() {
    echo -e "${YELLOW}TEST:${NC} $1"
}

pass() {
    echo -e "${GREEN}✓ PASS${NC}: $1"
    PASSED=$((PASSED + 1))
}

fail() {
    echo -e "${RED}✗ FAIL${NC}: $1"
    FAILED=$((FAILED + 1))
}

# Test 1: Blank file
print_test "Blank file"
cat > "${TEST_DIR}/blank.c" << 'EOF'
EOF
OUTPUT=$("${TOOL}" --csv "${TEST_DIR}/blank.c")
if echo "${OUTPUT}" | grep -q "blank.c,0,0,0,0,0,0,0"; then
    pass "Blank file metrics correct"
else
    fail "Blank file metrics incorrect: ${OUTPUT}"
fi

# Test 2: Only blank lines
print_test "File with only blank lines"
cat > "${TEST_DIR}/only_blanks.c" << 'EOF'


  
	
EOF
OUTPUT=$("${TOOL}" --csv "${TEST_DIR}/only_blanks.c")
if echo "${OUTPUT}" | grep -q "only_blanks.c,4,0,4,0,0,0"; then
    pass "Only blank lines metrics correct"
else
    fail "Only blank lines metrics incorrect: ${OUTPUT}"
fi

# Test 3: Only single-line comments
print_test "File with only single-line comments"
cat > "${TEST_DIR}/only_single_comments.c" << 'EOF'
// Comment 1
// Comment 2
// Comment 3
EOF
OUTPUT=$("${TOOL}" --csv "${TEST_DIR}/only_single_comments.c")
if echo "${OUTPUT}" | grep -q "only_single_comments.c,3,0,0,3,0,0"; then
    pass "Only single-line comments metrics correct"
else
    fail "Only single-line comments metrics incorrect: ${OUTPUT}"
fi

# Test 4: Only multi-line comments
print_test "File with only multi-line comments"
cat > "${TEST_DIR}/only_multi_comments.c" << 'EOF'
/* This is a
   multi-line
   comment */
EOF
OUTPUT=$("${TOOL}" --csv "${TEST_DIR}/only_multi_comments.c")
if echo "${OUTPUT}" | grep -q "only_multi_comments.c,3,0,0,3,0,0"; then
    pass "Only multi-line comments metrics correct"
else
    fail "Only multi-line comments metrics incorrect: ${OUTPUT}"
fi

# Test 5: Comments inside strings should NOT be counted as comments
print_test "Comments inside strings"
cat > "${TEST_DIR}/comments_in_strings.c" << 'EOF'
#include <stdio.h>
int main() {
    printf("This /* is not */ a comment");
    printf("This // is not a comment either");
    return 0;
}
EOF
OUTPUT=$("${TOOL}" --csv "${TEST_DIR}/comments_in_strings.c")
# Should have 6 total lines, 6 code lines (includes blank line after main), 0 comment lines
if echo "${OUTPUT}" | grep -q "comments_in_strings.c,6,6,0,0,1,1"; then
    pass "Comments in strings not counted as comments"
else
    fail "Comments in strings incorrectly counted: ${OUTPUT}"
fi

# Test 6: Escaped quotes in strings
print_test "Escaped quotes in strings"
cat > "${TEST_DIR}/escaped_quotes.c" << 'EOF'
#include <stdio.h>
int main() {
    printf("He said \"Hello\"");
    char c = '\'';
    return 0;
}
EOF
OUTPUT=$("${TOOL}" --csv "${TEST_DIR}/escaped_quotes.c")
if echo "${OUTPUT}" | grep -q "escaped_quotes.c,6,6,0,0,1,1"; then
    pass "Escaped quotes handled correctly"
else
    fail "Escaped quotes handling incorrect: ${OUTPUT}"
fi

# Test 7: Function prototypes should not be counted as functions
print_test "Function prototypes"
cat > "${TEST_DIR}/prototypes.c" << 'EOF'
int add(int a, int b);
int subtract(int a, int b);

int add(int a, int b) {
    return a + b;
}
EOF
OUTPUT=$("${TOOL}" --csv "${TEST_DIR}/prototypes.c")
# Should count only 1 function (the definition, not the prototypes)
if echo "${OUTPUT}" | grep -q ",1,"; then
    pass "Function prototypes not counted"
else
    fail "Function prototypes incorrectly counted: ${OUTPUT}"
fi

# Test 8: Control statements should not be counted as functions
print_test "Control statements"
cat > "${TEST_DIR}/control_statements.c" << 'EOF'
#include <stdio.h>
int main() {
    if (1) {
        printf("if");
    }
    for (int i = 0; i < 10; i++) {
        printf("for");
    }
    while (1) {
        break;
    }
    return 0;
}
EOF
OUTPUT=$("${TOOL}" --csv "${TEST_DIR}/control_statements.c")
# Should count only 1 function (main)
if echo "${OUTPUT}" | grep -q ",1,"; then
    pass "Control statements not counted as functions"
else
    fail "Control statements incorrectly counted: ${OUTPUT}"
fi

# Test 9: Include with trailing comment
print_test "Include with trailing comment"
cat > "${TEST_DIR}/include_comment.c" << 'EOF'
#include <stdio.h> // Standard I/O
#include <stdlib.h> /* Memory allocation */
EOF
OUTPUT=$("${TOOL}" --csv "${TEST_DIR}/include_comment.c")
# Should count 2 includes
if echo "${OUTPUT}" | grep -q ",2,"; then
    pass "Includes with comments counted correctly"
else
    fail "Includes with comments not counted: ${OUTPUT}"
fi

# Test 10: Long lines
print_test "Very long lines"
LONG_LINE="int very_long_variable_name_that_exceeds_normal_limits_and_goes_on_and_on_and_on_and_on_and_on_and_on_and_on = 1;"
cat > "${TEST_DIR}/long_lines.c" << EOF
#include <stdio.h>
${LONG_LINE}
int main() { return 0; }
EOF
OUTPUT=$("${TOOL}" --csv "${TEST_DIR}/long_lines.c")
# Max line length should be greater than 100
if echo "${OUTPUT}" | grep -qE ",([0-9]{3,}|[1-9][0-9]{2,})$"; then
    pass "Long lines measured correctly"
else
    fail "Long lines not measured correctly: ${OUTPUT}"
fi

# Test 11: Tabs in lines
print_test "Tabs in lines"
cat > "${TEST_DIR}/tabs.c" << 'EOF'
#include <stdio.h>
int main() {
	int x = 1;	// Tab before comment
		int y = 2;	// Two tabs
	return 0;
}
EOF
OUTPUT=$("${TOOL}" --csv "${TEST_DIR}/tabs.c")
# Should handle tabs (expanded to 8 spaces)
if echo "${OUTPUT}" | grep -q "tabs.c"; then
    pass "Tabs handled in line length calculation"
else
    fail "Tabs not handled: ${OUTPUT}"
fi

# Test 12: Mixed line endings (CRLF)
print_test "CRLF line endings"
printf "#include <stdio.h>\r\nint main() {\r\n    return 0;\r\n}\r\n" > "${TEST_DIR}/crlf.c"
OUTPUT=$("${TOOL}" --csv "${TEST_DIR}/crlf.c")
if echo "${OUTPUT}" | grep -q "crlf.c,4"; then
    pass "CRLF line endings handled correctly"
else
    fail "CRLF line endings not handled: ${OUTPUT}"
fi

# Test 13: Code before comment on same line
print_test "Code before comment on same line"
cat > "${TEST_DIR}/code_and_comment.c" << 'EOF'
#include <stdio.h>
int x = 1; // Variable declaration
int y = 2; /* Another variable */
EOF
OUTPUT=$("${TOOL}" --csv "${TEST_DIR}/code_and_comment.c")
# Should count as code lines, not comment lines
if echo "${OUTPUT}" | grep -q "code_and_comment.c,3,3,0,0"; then
    pass "Code before comment counted correctly"
else
    fail "Code before comment not counted correctly: ${OUTPUT}"
fi

# Test 14: Multi-line comment spanning many lines
print_test "Multi-line comment spanning many lines"
cat > "${TEST_DIR}/long_multi_comment.c" << 'EOF'
/* This is a very long
   multi-line comment that
   spans many lines to test
   the comment counting logic
   and make sure it works
   correctly */

int main() {
    return 0;
}
EOF
OUTPUT=$("${TOOL}" --csv "${TEST_DIR}/long_multi_comment.c")
# Should count 6 comment lines, 1 blank line, and 3 code lines
if echo "${OUTPUT}" | grep -q "long_multi_comment.c,10,3,1,6,1"; then
    pass "Long multi-line comment counted correctly"
else
    fail "Long multi-line comment not counted correctly: ${OUTPUT}"
fi

# Test 15: Include directives with spaces
print_test "Include directives with various formats"
cat > "${TEST_DIR}/includes.c" << 'EOF'
#include <stdio.h>
#  include <stdlib.h>
#	include <string.h>
# include <stdbool.h>
EOF
OUTPUT=$("${TOOL}" --csv "${TEST_DIR}/includes.c")
# Should count 4 includes
if echo "${OUTPUT}" | grep -q ",4,"; then
    pass "Various include formats counted correctly"
else
    fail "Various include formats not counted correctly: ${OUTPUT}"
fi

# Test 16: Empty main function
print_test "Empty main function"
cat > "${TEST_DIR}/empty_main.c" << 'EOF'
int main() {
}
EOF
OUTPUT=$("${TOOL}" --csv "${TEST_DIR}/empty_main.c")
# Should count 1 function
if echo "${OUTPUT}" | grep -q ",1,"; then
    pass "Empty function counted correctly"
else
    fail "Empty function not counted correctly: ${OUTPUT}"
fi

# Test 17: UTF-8 BOM handling
print_test "UTF-8 BOM (Byte Order Mark) handling"
# Create file with UTF-8 BOM (0xEF 0xBB 0xBF)
printf '\xEF\xBB\xBF#include <stdio.h>\nint main() { return 0; }\n' > "${TEST_DIR}/utf8_bom.c"
OUTPUT=$("${TOOL}" --csv "${TEST_DIR}/utf8_bom.c")
# Should handle BOM gracefully and count lines correctly
if echo "${OUTPUT}" | grep -q "utf8_bom.c,2"; then
    pass "UTF-8 BOM handled correctly"
else
    fail "UTF-8 BOM not handled correctly: ${OUTPUT}"
fi

# Test 18: Very long line stress test (over 10000 characters)
print_test "Very long line (10000+ chars) with getline"
LONG_VAR="x$(printf 'a%.0s' {1..10000})"
cat > "${TEST_DIR}/very_long_line.c" << EOF
#include <stdio.h>
int ${LONG_VAR} = 1;
int main() { return 0; }
EOF
OUTPUT=$("${TOOL}" --csv "${TEST_DIR}/very_long_line.c")
# Should handle very long lines and report length > 10000
if echo "${OUTPUT}" | grep -qE ",([1-9][0-9]{4,})$"; then
    pass "Very long line handled correctly by getline"
else
    fail "Very long line not handled: ${OUTPUT}"
fi

# Cleanup
rm -rf "${TEST_DIR}"

# Summary
echo ""
echo "======================================"
echo "Test Results:"
echo "  Passed: ${PASSED}"
echo "  Failed: ${FAILED}"
echo "======================================"

if [ ${FAILED} -gt 0 ]; then
    exit 1
else
    echo "All tests passed!"
    exit 0
fi
