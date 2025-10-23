#!/bin/bash
# Test script for spreadsheet-like interface functionality
# Validates that metrics_tool produces correct tabular/CSV output
# Tests: spreadsheet rendering, CSV export, column headers, data formatting

set -euo pipefail
export LC_ALL=C

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

PASSED=0
FAILED=0

# Allow overriding the tool binary via environment variable (default: ./metrics_tool)
TOOL="${METRICS_TOOL_BINARY:-./metrics_tool}"

test_pass() {
    echo -e "${GREEN}✓${NC} $1"
    PASSED=$((PASSED + 1))
}

test_fail() {
    echo -e "${RED}✗${NC} $1"
    FAILED=$((FAILED + 1))
}

# Ensure metrics_tool exists
if [ ! -f "$TOOL" ]; then
    echo "Error: metrics_tool not found. Run 'make metrics_tool' first."
    exit 1
fi

echo "Testing Spreadsheet-like Interface for Code Metrics"
echo "===================================================="
echo ""

# Test 1: CSV output format validation
echo "Test 1: CSV output format with correct headers"
CSV_OUTPUT=$($TOOL --csv hello.c)
if echo "$CSV_OUTPUT" | head -1 | grep -q "Filename,Total Lines,Code Lines,Blank Lines,Comment Lines,Functions,Includes,Max Line Length"; then
    test_pass "CSV headers are correct"
else
    test_fail "CSV headers are incorrect"
fi

# Test 2: CSV data row validation
echo "Test 2: CSV data row contains expected values"
DATA_ROW=$(echo "$CSV_OUTPUT" | tail -1)
if echo "$DATA_ROW" | grep -q "hello.c,"; then
    test_pass "CSV data row starts with filename"
else
    test_fail "CSV data row missing filename"
fi

# Test 3: CSV field count validation
echo "Test 3: CSV has correct number of fields (8)"
FIELD_COUNT=$(echo "$DATA_ROW" | awk -F',' '{print NF}')
if [ "$FIELD_COUNT" -eq 8 ]; then
    test_pass "CSV has 8 fields"
else
    test_fail "CSV has $FIELD_COUNT fields (expected 8)"
fi

# Test 4: Non-interactive mode produces deterministic output
echo "Test 4: Non-interactive mode output is deterministic"
OUTPUT1=$($TOOL -n hello.c)
OUTPUT2=$($TOOL -n hello.c)
if [ "$OUTPUT1" = "$OUTPUT2" ]; then
    test_pass "Non-interactive output is deterministic"
else
    test_fail "Non-interactive output varies between runs"
fi

# Test 5: Spreadsheet table rendering (non-interactive)
echo "Test 5: Spreadsheet table contains box-drawing characters"
TABLE_OUTPUT=$($TOOL -n hello.c)
if echo "$TABLE_OUTPUT" | grep -q "═\|║\|╔\|╗\|╚\|╝\|╠\|╣\|╬"; then
    test_pass "Table uses spreadsheet-like box-drawing characters"
else
    test_fail "Table missing box-drawing characters"
fi

# Test 6: Table contains "CODE METRICS SPREADSHEET" header
echo "Test 6: Table header identifies it as spreadsheet"
if echo "$TABLE_OUTPUT" | grep -q "CODE METRICS SPREADSHEET"; then
    test_pass "Table has 'CODE METRICS SPREADSHEET' header"
else
    test_fail "Table missing spreadsheet header"
fi

# Test 7: Table contains column headers
echo "Test 7: Table contains readable column headers"
if echo "$TABLE_OUTPUT" | grep -q "Filename" && echo "$TABLE_OUTPUT" | grep -q "Total" && echo "$TABLE_OUTPUT" | grep -q "Lines"; then
    test_pass "Table contains column headers"
else
    test_fail "Table missing column headers"
fi

# Test 8: CSV numeric values are valid
echo "Test 8: CSV numeric fields contain valid integers"
# Extract numeric fields (skip filename)
NUMERIC_FIELDS=$(echo "$DATA_ROW" | cut -d',' -f2-)
INVALID_NUMS=0
for field in $(echo "$NUMERIC_FIELDS" | tr ',' ' '); do
    if ! [[ "$field" =~ ^[0-9]+$ ]]; then
        INVALID_NUMS=1
        break
    fi
done
if [ $INVALID_NUMS -eq 0 ]; then
    test_pass "All numeric CSV fields are valid integers"
else
    test_fail "CSV contains invalid numeric values"
fi

# Test 9: Help output mentions spreadsheet
echo "Test 9: Help output describes spreadsheet functionality"
HELP_OUTPUT=$($TOOL --help)
if echo "$HELP_OUTPUT" | grep -qi "spreadsheet\|table\|tabular"; then
    test_pass "Help mentions spreadsheet/table functionality"
else
    test_fail "Help doesn't describe spreadsheet functionality"
fi

# Test 10: Version output is present
echo "Test 10: Version flag produces output"
VERSION_OUTPUT=$($TOOL --version)
if [ -n "$VERSION_OUTPUT" ]; then
    test_pass "Version output is present"
else
    test_fail "Version output is empty"
fi

# Test 11: Exit code validation
echo "Test 11: Successful analysis returns exit code 0"
if $TOOL --csv hello.c > /dev/null 2>&1; then
    test_pass "Exit code 0 on successful analysis"
else
    test_fail "Non-zero exit code on successful analysis"
fi

# Test 12: Table alignment check
echo "Test 12: Table columns are aligned with box-drawing"
# Count if there are consistent box characters in multiple lines
BOX_LINES=$(echo "$TABLE_OUTPUT" | grep -c "║" || true)
if [ "$BOX_LINES" -ge 3 ]; then
    test_pass "Table has multiple aligned rows with box characters"
else
    test_fail "Table alignment appears broken"
fi

echo ""
echo "===================================================="
echo "Spreadsheet Interface Tests Complete"
echo "Passed: $PASSED, Failed: $FAILED"
echo "===================================================="

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed!${NC}"
    exit 1
fi
