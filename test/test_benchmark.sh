#!/bin/bash
# Test script for benchmark.sh
# Validates that the benchmark produces consistent and accurate timing measurements

set -e

echo "=== Testing Benchmark Script ==="
echo ""

# Test 1: Check that benchmark script exists and is executable
echo "Test 1: Checking benchmark script..."
if [[ ! -f "benchmark.sh" ]]; then
    echo "ERROR: benchmark.sh not found"
    exit 1
fi
if [[ ! -x "benchmark.sh" ]]; then
    echo "ERROR: benchmark.sh is not executable"
    exit 1
fi
echo "✓ benchmark.sh exists and is executable"
echo ""

# Test 2: Check required dependencies
echo "Test 2: Checking required dependencies..."
for cmd in bc jq gcc; do
    if ! command -v "$cmd" &> /dev/null; then
        echo "ERROR: Required command '$cmd' not found"
        exit 1
    fi
done
echo "✓ All required dependencies available"
echo ""

# Test 3: Run benchmark with minimal iterations
echo "Test 3: Running benchmark with 2 iterations..."
TEMP_OUTPUT=$(mktemp)
./benchmark.sh hello.c 2 "$TEMP_OUTPUT" > /dev/null 2>&1
if [[ ! -f "$TEMP_OUTPUT" ]]; then
    echo "ERROR: Benchmark output file not created"
    exit 1
fi
echo "✓ Benchmark completed successfully"
echo ""

# Test 4: Validate JSON structure
echo "Test 4: Validating JSON structure..."
if ! jq empty "$TEMP_OUTPUT" 2>/dev/null; then
    echo "ERROR: Output is not valid JSON"
    rm -f "$TEMP_OUTPUT"
    exit 1
fi
echo "✓ Output is valid JSON"
echo ""

# Test 5: Check expected fields in JSON
echo "Test 5: Checking required fields in JSON..."
REQUIRED_FIELDS=("compiler" "compiler_version" "optimization_level" "source_file" "iterations" "average_time" "min_time" "max_time" "std_dev" "individual_times" "timestamp")
FIRST_ENTRY=$(jq '.[0]' "$TEMP_OUTPUT")
for field in "${REQUIRED_FIELDS[@]}"; do
    if ! echo "$FIRST_ENTRY" | jq -e ".$field" > /dev/null 2>&1; then
        echo "ERROR: Required field '$field' not found in JSON"
        rm -f "$TEMP_OUTPUT"
        exit 1
    fi
done
echo "✓ All required fields present"
echo ""

# Test 6: Verify optimization levels
echo "Test 6: Verifying optimization levels..."
EXPECTED_OPT_LEVELS=("O0" "O1" "O2" "O3" "Os")
for opt_level in "${EXPECTED_OPT_LEVELS[@]}"; do
    COUNT=$(jq "[.[] | select(.optimization_level == \"$opt_level\")] | length" "$TEMP_OUTPUT")
    if [[ $COUNT -eq 0 ]]; then
        echo "ERROR: No results found for optimization level $opt_level"
        rm -f "$TEMP_OUTPUT"
        exit 1
    fi
done
echo "✓ All optimization levels tested"
echo ""

# Test 7: Verify compilers tested
echo "Test 7: Verifying compilers..."
GCC_COUNT=$(jq '[.[] | select(.compiler == "gcc")] | length' "$TEMP_OUTPUT")
if [[ $GCC_COUNT -eq 0 ]]; then
    echo "ERROR: No GCC results found"
    rm -f "$TEMP_OUTPUT"
    exit 1
fi
echo "✓ GCC compiler tested"

if command -v clang &> /dev/null; then
    CLANG_COUNT=$(jq '[.[] | select(.compiler == "clang")] | length' "$TEMP_OUTPUT")
    if [[ $CLANG_COUNT -eq 0 ]]; then
        echo "ERROR: Clang is available but no results found"
        rm -f "$TEMP_OUTPUT"
        exit 1
    fi
    echo "✓ Clang compiler tested"
fi
echo ""

# Test 8: Verify timing consistency
echo "Test 8: Verifying timing data consistency..."
# Check that average_time is between min_time and max_time
INCONSISTENT=$(jq '[.[] | select(.average_time < .min_time or .average_time > .max_time)] | length' "$TEMP_OUTPUT")
if [[ $INCONSISTENT -gt 0 ]]; then
    echo "ERROR: Found $INCONSISTENT entries where average_time is not between min_time and max_time"
    rm -f "$TEMP_OUTPUT"
    exit 1
fi
echo "✓ Timing data is consistent (average between min and max)"
echo ""

# Test 9: Verify individual times count matches iterations
echo "Test 9: Verifying individual times count..."
EXPECTED_ITERATIONS=2
MISMATCH=$(jq "[.[] | select((.individual_times | length) != $EXPECTED_ITERATIONS)] | length" "$TEMP_OUTPUT")
if [[ $MISMATCH -gt 0 ]]; then
    echo "ERROR: Found $MISMATCH entries where individual_times count doesn't match iterations"
    rm -f "$TEMP_OUTPUT"
    exit 1
fi
echo "✓ Individual times count matches iterations"
echo ""

# Test 10: Verify times are positive and reasonable
echo "Test 10: Verifying times are positive and reasonable..."
INVALID_TIMES=$(jq '[.[] | select(.average_time <= 0 or .average_time > 60)] | length' "$TEMP_OUTPUT")
if [[ $INVALID_TIMES -gt 0 ]]; then
    echo "ERROR: Found $INVALID_TIMES entries with invalid times (<=0 or >60 seconds)"
    rm -f "$TEMP_OUTPUT"
    exit 1
fi
echo "✓ All times are positive and reasonable"
echo ""

# Test 11: Verify timestamp format
echo "Test 11: Verifying timestamp format..."
TIMESTAMP=$(jq -r '.[0].timestamp' "$TEMP_OUTPUT")
if ! echo "$TIMESTAMP" | grep -qE '^[0-9]{4}-[0-9]{2}-[0-9]{2}T[0-9]{2}:[0-9]{2}:[0-9]{2}Z$'; then
    echo "ERROR: Timestamp format is invalid: $TIMESTAMP"
    rm -f "$TEMP_OUTPUT"
    exit 1
fi
echo "✓ Timestamp format is valid (ISO 8601)"
echo ""

# Test 12: Test benchmark with different source file
echo "Test 12: Testing with non-existent source file..."
TEMP_FAIL=$(mktemp)
if ./benchmark.sh nonexistent.c 2 "$TEMP_FAIL" > /dev/null 2>&1; then
    echo "ERROR: Benchmark should fail with non-existent source file"
    rm -f "$TEMP_OUTPUT" "$TEMP_FAIL"
    exit 1
fi
rm -f "$TEMP_FAIL"
echo "✓ Benchmark properly handles missing source file"
echo ""

# Clean up
rm -f "$TEMP_OUTPUT"

echo "=== All benchmark tests passed! ==="
