#!/bin/bash
# Performance benchmarking script for Hello-World C program
# Measures compilation times, execution times, and binary sizes

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
RESULTS_FILE="${SCRIPT_DIR}/benchmark_results.json"
BASELINE_FILE="${SCRIPT_DIR}/baseline.json"

# ANSI color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to measure compilation time
measure_compile_time() {
    local compiler=$1
    local flags=$2
    local output_binary=$3
    local source="hello.c"
    
    # Clean up previous binary
    rm -f "$output_binary"
    
    # Measure compilation time (average of 5 runs for accuracy)
    local total_time=0
    local runs=5
    
    for ((i=1; i<=runs; i++)); do
        local start_time=$(date +%s.%N)
        $compiler $flags -o "$output_binary" "$source" 2>/dev/null
        local end_time=$(date +%s.%N)
        local compile_time=$(echo "$end_time - $start_time" | bc)
        total_time=$(echo "$total_time + $compile_time" | bc)
        rm -f "$output_binary"
    done
    
    # Calculate average
    local avg_time=$(echo "scale=6; $total_time / $runs" | bc)
    echo "$avg_time"
}

# Function to measure execution time
measure_execution_time() {
    local binary=$1
    
    # Measure execution time (average of 1000 runs for better precision)
    local runs=1000
    
    # Use time command for better precision
    local start_time=$(date +%s.%N)
    for ((i=1; i<=runs; i++)); do
        ./"$binary" > /dev/null 2>&1
    done
    local end_time=$(date +%s.%N)
    
    # Calculate average in microseconds with higher precision
    local total_time=$(echo "$end_time - $start_time" | bc -l)
    local avg_time_us=$(echo "scale=6; ($total_time / $runs) * 1000000" | bc -l)
    # Format to 2 decimal places for output
    printf "%.2f" "$avg_time_us"
}

# Function to measure binary size
measure_binary_size() {
    local binary=$1
    
    if [ ! -f "$binary" ]; then
        echo "0"
        return 1
    fi
    
    # Try Linux stat first, then macOS stat
    local size=$(stat -c%s "$binary" 2>/dev/null || stat -f%z "$binary" 2>/dev/null || echo "0")
    echo "$size"
}

# Function to run benchmarks for a compiler
run_compiler_benchmarks() {
    local compiler=$1
    local compiler_name=$2
    
    echo -e "${BLUE}Running benchmarks for ${compiler_name}...${NC}"
    
    # Test different optimization levels
    local opt_levels=("" "-O0" "-O1" "-O2" "-O3" "-Os")
    local opt_names=("default" "O0" "O1" "O2" "O3" "Os")
    
    for i in "${!opt_levels[@]}"; do
        local opt="${opt_levels[$i]}"
        local opt_name="${opt_names[$i]}"
        local binary="hello_bench_${compiler_name}_${opt_name}"
        
        echo -e "  Testing ${opt_name}..."
        
        # Measure compilation time
        local compile_time=$(measure_compile_time "$compiler" "$opt" "$binary")
        
        # Compile for execution and size measurements
        $compiler $opt -o "$binary" hello.c 2>/dev/null
        
        # Measure execution time
        local exec_time=$(measure_execution_time "$binary")
        
        # Measure binary size
        local binary_size=$(measure_binary_size "$binary")
        
        # Store results
        echo "    Compile: ${compile_time}s, Execute: ${exec_time}μs, Size: ${binary_size} bytes"
        
        # Clean up
        rm -f "$binary"
        
        # Add to results JSON
        cat >> "$RESULTS_FILE" <<EOF
  "${compiler_name}_${opt_name}": {
    "compiler": "${compiler_name}",
    "optimization": "${opt_name}",
    "compile_time_seconds": ${compile_time},
    "execution_time_microseconds": ${exec_time},
    "binary_size_bytes": ${binary_size}
  },
EOF
    done
}

# Main execution
main() {
    cd "$PROJECT_ROOT"
    
    echo -e "${GREEN}=== Performance Benchmark Suite ===${NC}"
    echo "Project: Hello-World C Program"
    echo "Date: $(date -u +"%Y-%m-%d %H:%M:%S UTC")"
    echo ""
    
    # Initialize results file
    echo "{" > "$RESULTS_FILE"
    echo '  "timestamp": "'$(date -u +"%Y-%m-%dT%H:%M:%SZ")'",' >> "$RESULTS_FILE"
    echo '  "benchmarks": {' >> "$RESULTS_FILE"
    
    # Run benchmarks for GCC
    if command -v gcc &> /dev/null; then
        run_compiler_benchmarks "gcc" "gcc"
    else
        echo -e "${YELLOW}Warning: GCC not found, skipping GCC benchmarks${NC}"
    fi
    
    # Run benchmarks for Clang
    if command -v clang &> /dev/null; then
        run_compiler_benchmarks "clang" "clang"
    else
        echo -e "${YELLOW}Warning: Clang not found, skipping Clang benchmarks${NC}"
    fi
    
    # Close JSON (remove trailing comma from last entry)
    # Use temp file for portability across different sed implementations
    sed '$ s/,$//' "$RESULTS_FILE" > "${RESULTS_FILE}.tmp"
    mv "${RESULTS_FILE}.tmp" "$RESULTS_FILE"
    echo "  }" >> "$RESULTS_FILE"
    echo "}" >> "$RESULTS_FILE"
    
    echo ""
    echo -e "${GREEN}Benchmark complete!${NC}"
    echo "Results saved to: $RESULTS_FILE"
    
    # Compare with baseline if it exists
    if [ -f "$BASELINE_FILE" ]; then
        echo ""
        echo -e "${BLUE}Comparing with baseline...${NC}"
        "${SCRIPT_DIR}/compare_performance.sh"
    else
        echo ""
        echo -e "${YELLOW}No baseline found. Run './perf/set_baseline.sh' to establish baseline.${NC}"
    fi
}

main "$@"
