#!/bin/bash
# Test script to validate benchmark measurement accuracy

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
BENCHMARK_SCRIPT="$REPO_ROOT/benchmarks/run_benchmarks.sh"
RESULTS_FILE="$REPO_ROOT/benchmarks/benchmark_results.json"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Test 1: Benchmark script runs successfully
test_benchmark_runs() {
    log_info "Test 1: Benchmark script runs successfully"
    
    cd "$REPO_ROOT"
    
    if bash "$BENCHMARK_SCRIPT" --skip-comparison > /dev/null 2>&1; then
        log_success "Benchmark script executed successfully"
        return 0
    else
        log_error "Benchmark script failed to execute"
        return 1
    fi
}

# Test 2: Results file is created
test_results_file_created() {
    log_info "Test 2: Results file is created"
    
    if [ -f "$RESULTS_FILE" ]; then
        log_success "Results file created: $RESULTS_FILE"
        return 0
    else
        log_error "Results file not found: $RESULTS_FILE"
        return 1
    fi
}

# Test 3: Results file is valid JSON
test_results_valid_json() {
    log_info "Test 3: Results file is valid JSON"
    
    if python3 -m json.tool "$RESULTS_FILE" > /dev/null 2>&1; then
        log_success "Results file is valid JSON"
        return 0
    else
        log_error "Results file is not valid JSON"
        return 1
    fi
}

# Test 4: Results contain expected fields
test_results_have_required_fields() {
    log_info "Test 4: Results contain expected fields"
    
    local required_fields=("timestamp" "git_commit" "benchmarks")
    local all_present=true
    
    for field in "${required_fields[@]}"; do
        if ! grep -q "\"$field\"" "$RESULTS_FILE"; then
            log_error "Missing required field: $field"
            all_present=false
        fi
    done
    
    if [ "$all_present" = true ]; then
        log_success "All required fields present"
        return 0
    else
        return 1
    fi
}

# Test 5: Benchmarks contain GCC results
test_results_have_gcc_benchmarks() {
    log_info "Test 5: Benchmarks contain GCC results"
    
    if grep -q "\"gcc_O" "$RESULTS_FILE"; then
        log_success "GCC benchmarks present"
        return 0
    else
        log_error "GCC benchmarks missing"
        return 1
    fi
}

# Test 6: Benchmarks contain Clang results
test_results_have_clang_benchmarks() {
    log_info "Test 6: Benchmarks contain Clang results"
    
    if grep -q "\"clang_O" "$RESULTS_FILE"; then
        log_success "Clang benchmarks present"
        return 0
    else
        log_error "Clang benchmarks missing"
        return 1
    fi
}

# Test 7: Compilation times are reasonable
test_compilation_times_reasonable() {
    log_info "Test 7: Compilation times are reasonable"
    
    # Extract compile times and verify they're within reasonable bounds
    # GCC should be < 200ms, Clang O0 might be higher but < 1000ms
    local gcc_times=$(grep -A 3 "gcc_O" "$RESULTS_FILE" | grep "compile_time_ms" | grep -oP '\d+')
    local max_gcc_time=0
    
    while read -r time; do
        if [ "$time" -gt "$max_gcc_time" ]; then
            max_gcc_time=$time
        fi
        
        if [ "$time" -gt 1000 ]; then
            log_error "GCC compilation time too high: ${time}ms"
            return 1
        fi
    done <<< "$gcc_times"
    
    log_success "Compilation times are within reasonable bounds (max GCC: ${max_gcc_time}ms)"
    return 0
}

# Test 8: Binary sizes are reasonable
test_binary_sizes_reasonable() {
    log_info "Test 8: Binary sizes are reasonable"
    
    # Extract binary sizes and verify they're reasonable (should be around 15-20KB)
    local sizes=$(grep "binary_size_bytes" "$RESULTS_FILE" | grep -oP '\d+')
    local all_reasonable=true
    
    while read -r size; do
        if [ "$size" -lt 10000 ] || [ "$size" -gt 50000 ]; then
            log_error "Binary size out of expected range: ${size} bytes"
            all_reasonable=false
        fi
    done <<< "$sizes"
    
    if [ "$all_reasonable" = true ]; then
        log_success "Binary sizes are within reasonable range"
        return 0
    else
        return 1
    fi
}

# Test 9: Execution times are measured
test_execution_times_measured() {
    log_info "Test 9: Execution times are measured"
    
    # Verify that execution times are present and > 0
    local exec_times=$(grep "execution_time_us" "$RESULTS_FILE" | grep -oP '\d+')
    local all_nonzero=true
    
    while read -r time; do
        if [ "$time" -eq 0 ]; then
            log_error "Execution time is zero (measurement failed)"
            all_nonzero=false
        fi
    done <<< "$exec_times"
    
    if [ "$all_nonzero" = true ]; then
        log_success "All execution times measured successfully"
        return 0
    else
        return 1
    fi
}

# Test 10: Comparison script detects regressions
test_comparison_detects_regression() {
    log_info "Test 10: Comparison script detects artificial regressions"
    
    # Create a modified baseline with artificially worse performance
    local baseline_file="$REPO_ROOT/benchmarks/baseline.json"
    local temp_baseline="$REPO_ROOT/benchmarks/baseline_test.json"
    
    # Create baseline if it doesn't exist
    if [ ! -f "$baseline_file" ]; then
        cp "$RESULTS_FILE" "$baseline_file"
    fi
    
    # Create a version with worse performance
    python3 -c "
import json
import sys

with open('$RESULTS_FILE', 'r') as f:
    data = json.load(f)

# Make compile times 30% worse (exceeds 20% threshold)
for config in data['benchmarks'].values():
    config['compile_time_ms'] = int(config['compile_time_ms'] * 0.7)  # Make baseline better

with open('$temp_baseline', 'w') as f:
    json.dump(data, f, indent=2)
"
    
    # Temporarily replace baseline
    mv "$baseline_file" "${baseline_file}.bak" 2>/dev/null || true
    mv "$temp_baseline" "$baseline_file"
    
    # Run comparison (should detect regression)
    local detected_regression=false
    if ! python3 "$REPO_ROOT/benchmarks/compare_benchmarks.py" > /dev/null 2>&1; then
        detected_regression=true
    fi
    
    # Restore original baseline
    mv "$baseline_file" "$temp_baseline"
    mv "${baseline_file}.bak" "$baseline_file" 2>/dev/null || true
    rm -f "$temp_baseline"
    
    if [ "$detected_regression" = true ]; then
        log_success "Comparison correctly detected regression"
        return 0
    else
        log_error "Comparison failed to detect regression"
        return 1
    fi
}

# Main test execution
main() {
    echo "=================================="
    echo "Benchmark Accuracy Tests"
    echo "=================================="
    echo ""
    
    local failed_tests=0
    local total_tests=10
    
    # Run all tests
    test_benchmark_runs || ((failed_tests++))
    test_results_file_created || ((failed_tests++))
    test_results_valid_json || ((failed_tests++))
    test_results_have_required_fields || ((failed_tests++))
    test_results_have_gcc_benchmarks || ((failed_tests++))
    test_results_have_clang_benchmarks || ((failed_tests++))
    test_compilation_times_reasonable || ((failed_tests++))
    test_binary_sizes_reasonable || ((failed_tests++))
    test_execution_times_measured || ((failed_tests++))
    test_comparison_detects_regression || ((failed_tests++))
    
    echo ""
    echo "=================================="
    if [ $failed_tests -eq 0 ]; then
        log_success "All $total_tests tests passed!"
        echo "=================================="
        exit 0
    else
        log_error "$failed_tests out of $total_tests tests failed"
        echo "=================================="
        exit 1
    fi
}

main
