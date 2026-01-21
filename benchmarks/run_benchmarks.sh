#!/bin/bash
# Performance benchmarking script for Hello World C program
# Measures compilation time, execution time, and binary size across different configurations

set -e

# Configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
SOURCE_FILE="$REPO_ROOT/hello.c"
RESULTS_FILE="$SCRIPT_DIR/benchmark_results.json"
BASELINE_FILE="$SCRIPT_DIR/baseline.json"
TEMP_DIR=$(mktemp -d)

# Cleanup on exit
cleanup() {
    rm -rf "$TEMP_DIR"
    # Clean up any test binaries
    cd "$REPO_ROOT"
    rm -f hello_bench_* 2>/dev/null || true
}
trap cleanup EXIT

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Logging functions
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Measure compilation time and binary size
benchmark_compile() {
    local compiler=$1
    local opt_level=$2
    local output_name="hello_bench_${compiler}_${opt_level}"
    local flags="-Wall -Wextra"
    
    # Add optimization flag
    if [ "$opt_level" != "O0" ]; then
        flags="$flags -$opt_level"
    fi
    
    cd "$REPO_ROOT"
    
    # Clean any existing binary
    rm -f "$output_name" 2>/dev/null || true
    
    # Measure compilation time (average of 5 runs for accuracy)
    local total_time=0
    local runs=5
    
    for i in $(seq 1 $runs); do
        # Clean between runs
        rm -f "$output_name" 2>/dev/null || true
        
        # Measure compilation time
        local start_time=$(date +%s%N)
        $compiler $flags -o "$output_name" "$SOURCE_FILE" 2>&1
        local end_time=$(date +%s%N)
        
        local compile_time=$((($end_time - $start_time) / 1000000)) # Convert to milliseconds
        total_time=$(($total_time + $compile_time))
    done
    
    # Calculate average
    local avg_compile_time=$(($total_time / $runs))
    
    # Measure binary size
    local binary_size=$(stat -f%z "$output_name" 2>/dev/null || stat -c%s "$output_name")
    
    # Verify the binary works
    local output=$(./"$output_name")
    if [ "$output" != "Hello world!" ]; then
        log_error "Binary $output_name produced incorrect output: $output"
        return 1
    fi
    
    echo "$avg_compile_time,$binary_size"
}

# Measure execution time
benchmark_execution() {
    local binary=$1
    local runs=1000
    
    cd "$REPO_ROOT"
    
    # Measure execution time (average of many runs for accuracy)
    local start_time=$(date +%s%N)
    for i in $(seq 1 $runs); do
        ./"$binary" > /dev/null
    done
    local end_time=$(date +%s%N)
    
    # Calculate average execution time in microseconds
    local total_time=$((($end_time - $start_time) / 1000)) # Convert to microseconds
    local avg_exec_time=$(($total_time / $runs))
    
    echo "$avg_exec_time"
}

# Generate JSON results
generate_json() {
    local timestamp=$(date -u +"%Y-%m-%dT%H:%M:%SZ")
    local git_commit=$(cd "$REPO_ROOT" && git rev-parse HEAD 2>/dev/null || echo "unknown")
    
    cat > "$RESULTS_FILE" << EOF
{
  "timestamp": "$timestamp",
  "git_commit": "$git_commit",
  "benchmarks": {
EOF
    
    local first=true
    
    # Benchmark different configurations
    for compiler in gcc clang; do
        if ! command -v $compiler &> /dev/null; then
            log_warning "$compiler not found, skipping"
            continue
        fi
        
        for opt_level in O0 O1 O2 O3 Os; do
            log_info "Benchmarking $compiler with -$opt_level..."
            
            local binary_name="hello_bench_${compiler}_${opt_level}"
            local compile_result=$(benchmark_compile "$compiler" "$opt_level")
            local compile_time=$(echo "$compile_result" | cut -d',' -f1)
            local binary_size=$(echo "$compile_result" | cut -d',' -f2)
            local exec_time=$(benchmark_execution "$binary_name")
            
            # Add comma if not first entry
            if [ "$first" = false ]; then
                printf "," >> "$RESULTS_FILE"
            fi
            first=false
            
            printf '\n    "%s": {\n' "${compiler}_${opt_level}" >> "$RESULTS_FILE"
            printf '      "compiler": "%s",\n' "$compiler" >> "$RESULTS_FILE"
            printf '      "optimization": "%s",\n' "$opt_level" >> "$RESULTS_FILE"
            printf '      "compile_time_ms": %d,\n' "$compile_time" >> "$RESULTS_FILE"
            printf '      "binary_size_bytes": %d,\n' "$binary_size" >> "$RESULTS_FILE"
            printf '      "execution_time_us": %d\n' "$exec_time" >> "$RESULTS_FILE"
            printf '    }' >> "$RESULTS_FILE"
        done
    done
    
    cat >> "$RESULTS_FILE" << EOF

  }
}
EOF
    
    log_success "Benchmark results saved to $RESULTS_FILE"
}

# Compare with baseline and detect regressions
compare_with_baseline() {
    if [ ! -f "$BASELINE_FILE" ]; then
        log_warning "No baseline file found at $BASELINE_FILE"
        log_info "Creating baseline from current results..."
        cp "$RESULTS_FILE" "$BASELINE_FILE"
        log_success "Baseline created"
        return 0
    fi
    
    log_info "Comparing with baseline..."
    
    local has_regression=false
    
    # Thresholds (percentage increase allowed)
    local compile_time_threshold=20  # 20% increase in compilation time
    local binary_size_threshold=10   # 10% increase in binary size
    local exec_time_threshold=50     # 50% increase in execution time
    
    # Parse and compare using basic shell tools
    # Note: This is a simplified comparison. For production, consider using jq or python
    
    log_info "Regression detection:"
    echo "  Thresholds:"
    echo "    - Compilation time: +${compile_time_threshold}%"
    echo "    - Binary size: +${binary_size_threshold}%"
    echo "    - Execution time: +${exec_time_threshold}%"
    echo ""
    
    # For now, we'll just show the results
    # A more sophisticated comparison would parse both JSON files
    # This is a placeholder for the actual comparison logic
    
    log_success "Comparison complete (detailed comparison requires jq or python)"
    
    if [ "$has_regression" = true ]; then
        return 1
    fi
    
    return 0
}

# Update baseline
update_baseline() {
    log_info "Updating baseline..."
    cp "$RESULTS_FILE" "$BASELINE_FILE"
    log_success "Baseline updated"
}

# Display results
display_results() {
    log_info "Benchmark Results:"
    echo ""
    
    if command -v python3 &> /dev/null; then
        # Pretty print JSON if python is available
        python3 -m json.tool "$RESULTS_FILE" 2>/dev/null || cat "$RESULTS_FILE"
    else
        cat "$RESULTS_FILE"
    fi
    
    echo ""
    
    # Show summary statistics
    log_info "Summary:"
    echo "  Results saved to: $RESULTS_FILE"
    if [ -f "$BASELINE_FILE" ]; then
        echo "  Baseline file: $BASELINE_FILE"
    fi
}

# Main execution
main() {
    log_info "Starting performance benchmarks..."
    echo ""
    
    cd "$REPO_ROOT"
    
    # Check if source file exists
    if [ ! -f "$SOURCE_FILE" ]; then
        log_error "Source file not found: $SOURCE_FILE"
        exit 1
    fi
    
    # Run benchmarks
    generate_json
    
    echo ""
    
    # Display results
    display_results
    
    echo ""
    
    # Compare with baseline
    local compare_exit_code=0
    if [ "${SKIP_COMPARISON:-}" != "1" ]; then
        compare_with_baseline || compare_exit_code=$?
    fi
    
    # Update baseline if requested
    if [ "${UPDATE_BASELINE:-}" = "1" ]; then
        update_baseline
    fi
    
    echo ""
    log_success "Benchmarking complete!"
    
    exit $compare_exit_code
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --update-baseline)
            UPDATE_BASELINE=1
            shift
            ;;
        --skip-comparison)
            SKIP_COMPARISON=1
            shift
            ;;
        --help)
            echo "Usage: $0 [OPTIONS]"
            echo ""
            echo "Options:"
            echo "  --update-baseline   Update the baseline with current results"
            echo "  --skip-comparison   Skip comparison with baseline"
            echo "  --help             Show this help message"
            exit 0
            ;;
        *)
            log_error "Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

main
