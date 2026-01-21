#!/bin/bash
# Script to compare current performance with baseline
# Returns non-zero exit code if performance regression is detected

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RESULTS_FILE="${SCRIPT_DIR}/benchmark_results.json"
BASELINE_FILE="${SCRIPT_DIR}/baseline.json"

# Performance regression thresholds (in percentage)
COMPILE_TIME_THRESHOLD=20  # Allow 20% increase in compilation time
EXECUTION_TIME_THRESHOLD=50  # Allow 50% increase in execution time
BINARY_SIZE_THRESHOLD=10  # Allow 10% increase in binary size

# ANSI color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Check if required files exist
if [ ! -f "$RESULTS_FILE" ]; then
    echo -e "${RED}Error: No benchmark results found. Run './perf/benchmark.sh' first.${NC}"
    exit 1
fi

if [ ! -f "$BASELINE_FILE" ]; then
    echo -e "${YELLOW}No baseline found. Run './perf/set_baseline.sh' to establish baseline.${NC}"
    exit 0
fi

# Function to extract value from JSON
extract_value() {
    local file=$1
    local key=$2
    local metric=$3
    
    # Use grep and sed to extract the value
    grep -A 5 "\"$key\":" "$file" | grep "\"$metric\"" | sed 's/.*: \([0-9.]*\).*/\1/'
}

# Function to calculate percentage change
calc_percentage_change() {
    local baseline=$1
    local current=$2
    
    if [ "$baseline" = "0" ] || [ -z "$baseline" ]; then
        echo "0"
        return
    fi
    
    echo "scale=2; (($current - $baseline) / $baseline) * 100" | bc
}

# Main comparison logic
echo -e "${BLUE}=== Performance Comparison ===${NC}"
echo ""

regression_found=0
comparison_count=0

# Get all benchmark keys
benchmark_keys=$(grep -o '"[^"]*_[^"]*": {' "$RESULTS_FILE" | sed 's/": {//' | sed 's/"//g')

for key in $benchmark_keys; do
    comparison_count=$((comparison_count + 1))
    
    # Extract metrics from baseline and current results
    baseline_compile=$(extract_value "$BASELINE_FILE" "$key" "compile_time_seconds")
    current_compile=$(extract_value "$RESULTS_FILE" "$key" "compile_time_seconds")
    
    baseline_exec=$(extract_value "$BASELINE_FILE" "$key" "execution_time_microseconds")
    current_exec=$(extract_value "$RESULTS_FILE" "$key" "execution_time_microseconds")
    
    baseline_size=$(extract_value "$BASELINE_FILE" "$key" "binary_size_bytes")
    current_size=$(extract_value "$RESULTS_FILE" "$key" "binary_size_bytes")
    
    # Skip if baseline values are missing
    if [ -z "$baseline_compile" ] || [ -z "$baseline_exec" ] || [ -z "$baseline_size" ]; then
        echo -e "${YELLOW}⚠ $key: No baseline data${NC}"
        continue
    fi
    
    # Calculate percentage changes
    compile_change=$(calc_percentage_change "$baseline_compile" "$current_compile")
    exec_change=$(calc_percentage_change "$baseline_exec" "$current_exec")
    size_change=$(calc_percentage_change "$baseline_size" "$current_size")
    
    # Check for regressions
    local_regression=0
    
    echo -e "${BLUE}$key:${NC}"
    
    # Check compilation time
    if (( $(echo "$compile_change > $COMPILE_TIME_THRESHOLD" | bc -l) )); then
        echo -e "  ${RED}✗ Compile time: ${current_compile}s (${compile_change}% increase, threshold: ${COMPILE_TIME_THRESHOLD}%)${NC}"
        local_regression=1
    else
        echo -e "  ${GREEN}✓ Compile time: ${current_compile}s (${compile_change}% change)${NC}"
    fi
    
    # Check execution time
    if (( $(echo "$exec_change > $EXECUTION_TIME_THRESHOLD" | bc -l) )); then
        echo -e "  ${RED}✗ Execution time: ${current_exec}μs (${exec_change}% increase, threshold: ${EXECUTION_TIME_THRESHOLD}%)${NC}"
        local_regression=1
    else
        echo -e "  ${GREEN}✓ Execution time: ${current_exec}μs (${exec_change}% change)${NC}"
    fi
    
    # Check binary size
    if (( $(echo "$size_change > $BINARY_SIZE_THRESHOLD" | bc -l) )); then
        echo -e "  ${RED}✗ Binary size: ${current_size} bytes (${size_change}% increase, threshold: ${BINARY_SIZE_THRESHOLD}%)${NC}"
        local_regression=1
    else
        echo -e "  ${GREEN}✓ Binary size: ${current_size} bytes (${size_change}% change)${NC}"
    fi
    
    if [ $local_regression -eq 1 ]; then
        regression_found=1
    fi
    
    echo ""
done

# Summary
echo -e "${BLUE}=== Summary ===${NC}"
echo "Comparisons performed: $comparison_count"

if [ $regression_found -eq 1 ]; then
    echo -e "${RED}Performance regression detected!${NC}"
    echo ""
    echo "Thresholds:"
    echo "  - Compilation time: ${COMPILE_TIME_THRESHOLD}%"
    echo "  - Execution time: ${EXECUTION_TIME_THRESHOLD}%"
    echo "  - Binary size: ${BINARY_SIZE_THRESHOLD}%"
    exit 1
else
    echo -e "${GREEN}All performance metrics within acceptable thresholds.${NC}"
    exit 0
fi
