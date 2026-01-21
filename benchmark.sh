#!/bin/bash
# Performance Benchmarking Suite for C Compilation Times
# Measures compilation times across different optimization levels and compilers
# Outputs results in JSON format for tracking over time

set -e

# Configuration
SOURCE_FILE="${1:-hello.c}"
NUM_ITERATIONS="${2:-5}"
OUTPUT_FILE="${3:-benchmark_results.json}"

# Check if source file exists
if [[ ! -f "$SOURCE_FILE" ]]; then
    echo "ERROR: Source file '$SOURCE_FILE' not found" >&2
    exit 1
fi

# Optimization levels to test
OPT_LEVELS=("O0" "O1" "O2" "O3" "Os")

# Compilers to test
COMPILERS=("gcc" "clang")

# Colors for output (if terminal supports it)
if [[ -t 1 ]]; then
    GREEN='\033[0;32m'
    BLUE='\033[0;34m'
    YELLOW='\033[1;33m'
    NC='\033[0m' # No Color
else
    GREEN=''
    BLUE=''
    YELLOW=''
    NC=''
fi

# Function to measure compilation time
# Args: compiler, optimization_level, source_file, output_binary
measure_compilation_time() {
    local compiler="$1"
    local opt_level="$2"
    local source="$3"
    local output="$4"
    
    # Use /usr/bin/time for accurate measurement (not bash builtin)
    # Complex redirection: suppress compiler stdout (1>/dev/null), 
    # capture stderr to stdout (2>&1), then extract last line with time
    local time_output
    time_output=$( { /usr/bin/time -f "%e" "$compiler" "-$opt_level" -o "$output" "$source" 2>&1 1>/dev/null; } 2>&1 | tail -n 1)
    
    echo "$time_output"
}

# Function to get compiler version
get_compiler_version() {
    local compiler="$1"
    "$compiler" --version | head -n 1
}

# Main benchmarking function
run_benchmarks() {
    local results="[]"
    local timestamp
    timestamp=$(date -u +"%Y-%m-%dT%H:%M:%SZ")
    
    echo -e "${BLUE}=== Performance Benchmarking Suite ===${NC}"
    echo -e "${YELLOW}Source: $SOURCE_FILE${NC}"
    echo -e "${YELLOW}Iterations: $NUM_ITERATIONS${NC}"
    echo -e "${YELLOW}Timestamp: $timestamp${NC}"
    echo ""
    
    for compiler in "${COMPILERS[@]}"; do
        # Check if compiler is available
        if ! command -v "$compiler" &> /dev/null; then
            echo -e "${YELLOW}Warning: $compiler not found, skipping...${NC}"
            continue
        fi
        
        local compiler_version
        compiler_version=$(get_compiler_version "$compiler")
        echo -e "${GREEN}Testing with $compiler${NC}"
        echo "  Version: $compiler_version"
        echo ""
        
        for opt_level in "${OPT_LEVELS[@]}"; do
            echo -e "  Optimization: ${BLUE}-$opt_level${NC}"
            
            local times=()
            local total_time=0
            
            # Run multiple iterations
            for ((i=1; i<=NUM_ITERATIONS; i++)); do
                local output_binary
                output_binary=$(mktemp /tmp/bench_XXXXXX)
                local time_taken
                time_taken=$(measure_compilation_time "$compiler" "$opt_level" "$SOURCE_FILE" "$output_binary")
                times+=("$time_taken")
                total_time=$(echo "$total_time + $time_taken" | bc)
                rm -f "$output_binary"
                
                printf "    Run %d: %.4fs\n" "$i" "$time_taken"
            done
            
            # Calculate average
            local avg_time
            avg_time=$(echo "scale=6; $total_time / $NUM_ITERATIONS" | bc)
            
            # Calculate min and max
            local min_time="${times[0]}"
            local max_time="${times[0]}"
            for time in "${times[@]}"; do
                if (( $(echo "$time < $min_time" | bc -l) )); then
                    min_time="$time"
                fi
                if (( $(echo "$time > $max_time" | bc -l) )); then
                    max_time="$time"
                fi
            done
            
            # Calculate standard deviation
            local sum_sq_diff=0
            for time in "${times[@]}"; do
                local diff=$(echo "$time - $avg_time" | bc)
                local sq_diff=$(echo "$diff * $diff" | bc)
                sum_sq_diff=$(echo "$sum_sq_diff + $sq_diff" | bc)
            done
            local variance=$(echo "scale=6; $sum_sq_diff / $NUM_ITERATIONS" | bc)
            local std_dev=$(echo "scale=6; sqrt($variance)" | bc)
            
            printf "    ${GREEN}Average: %.4fs (min: %.4fs, max: %.4fs, stddev: %.4fs)${NC}\n\n" "$avg_time" "$min_time" "$max_time" "$std_dev"
            
            # Build JSON entry
            local times_json=$(printf '%s\n' "${times[@]}" | jq -R . | jq -s .)
            local entry=$(jq -n \
                --arg compiler "$compiler" \
                --arg compiler_version "$compiler_version" \
                --arg opt_level "$opt_level" \
                --arg avg_time "$avg_time" \
                --arg min_time "$min_time" \
                --arg max_time "$max_time" \
                --arg std_dev "$std_dev" \
                --argjson times "$times_json" \
                --arg timestamp "$timestamp" \
                --arg source "$SOURCE_FILE" \
                --arg iterations "$NUM_ITERATIONS" \
                '{
                    compiler: $compiler,
                    compiler_version: $compiler_version,
                    optimization_level: $opt_level,
                    source_file: $source,
                    iterations: ($iterations | tonumber),
                    average_time: ($avg_time | tonumber),
                    min_time: ($min_time | tonumber),
                    max_time: ($max_time | tonumber),
                    std_dev: ($std_dev | tonumber),
                    individual_times: ($times | map(tonumber)),
                    timestamp: $timestamp
                }')
            
            results=$(echo "$results" | jq ". += [$entry]")
        done
        
        echo ""
    done
    
    # Write results to file
    echo "$results" | jq . > "$OUTPUT_FILE"
    echo -e "${GREEN}=== Benchmark Complete ===${NC}"
    echo -e "Results saved to: ${BLUE}$OUTPUT_FILE${NC}"
}

# Check for required commands
for cmd in bc jq; do
    if ! command -v "$cmd" &> /dev/null; then
        echo "ERROR: Required command '$cmd' not found. Please install it." >&2
        exit 1
    fi
done

# Run the benchmarks
run_benchmarks
