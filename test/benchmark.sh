#!/bin/bash
# Micro-benchmark script for Hello World C program
# Measures startup time and runtime performance
#
# Usage: ./test/benchmark.sh [binary_name] [--quiet] [--iterations=N]
#        binary_name: Name of the binary to benchmark (default: hello)
#        --quiet: Reduce output verbosity for CI environments
#        --iterations=N: Number of iterations to run (default: 100)

set -e  # Exit on any error
set -u  # Exit on unset variables  
set -o pipefail  # Exit on pipeline failures

# Default configuration
BINARY_NAME="${1:-hello}"
QUIET_MODE=false
ITERATIONS=100
STARTUP_TIME_THRESHOLD_MS=100  # 100ms threshold for startup time
RUNTIME_THRESHOLD_MS=50        # 50ms threshold for total runtime

# Parse command line arguments
for arg in "$@"; do
    case "${arg}" in
        --quiet)
            QUIET_MODE=true
            ;;
        --iterations=*)
            ITERATIONS="${arg#*=}"
            if ! [[ "${ITERATIONS}" =~ ^[0-9]+$ ]] || [[ ${ITERATIONS} -lt 1 ]]; then
                echo "Error: Invalid iterations value. Must be a positive integer."
                exit 1
            fi
            ;;
        *)
            if [[ "${arg}" != "${BINARY_NAME}" ]]; then
                BINARY_NAME="${arg}"
            fi
            ;;
    esac
done

# Colors for output (disabled in quiet mode)
if [[ "${QUIET_MODE}" == "false" ]]; then
    RED='\033[0;31m'
    GREEN='\033[0;32m'
    YELLOW='\033[1;33m'
    BLUE='\033[0;34m'
    NC='\033[0m' # No Color
else
    RED=''
    GREEN=''
    YELLOW=''
    BLUE=''
    NC=''
fi

# Function to print colored messages
print_success() {
    if [[ "${QUIET_MODE}" == "false" ]]; then
        printf '%b' "${GREEN}✓ $1${NC}\n"
    fi
}

print_error() {
    printf '%b' "${RED}✗ $1${NC}\n"
}

print_info() {
    if [[ "${QUIET_MODE}" == "false" ]]; then
        printf '%b' "${BLUE}ℹ $1${NC}\n"
    fi
}

print_warning() {
    if [[ "${QUIET_MODE}" == "false" ]]; then
        printf '%b' "${YELLOW}⚠ $1${NC}\n"
    fi
}

print_info "Running micro-benchmark for: ${BINARY_NAME}"

# Step 1: Verify binary exists and is executable
if [[ ! -f "${BINARY_NAME}" ]]; then
    print_error "Binary '${BINARY_NAME}' not found"
    print_error "Make sure to build the binary first: gcc -o ${BINARY_NAME} hello.c"
    exit 1
fi

if [[ ! -x "${BINARY_NAME}" ]]; then
    print_error "Binary '${BINARY_NAME}' is not executable"
    exit 1
fi

# Step 2: Verify the binary works correctly first
print_info "Verifying binary functionality..."
if ! OUTPUT=$(timeout 5s ./"${BINARY_NAME}" 2>&1); then
    print_error "Binary failed to execute or timed out"
    exit 1
fi

EXPECTED_OUTPUT="Hello world!"
if [[ "${OUTPUT}" != "${EXPECTED_OUTPUT}" ]]; then
    print_error "Binary output is incorrect"
    printf "Expected: '%s'\n" "${EXPECTED_OUTPUT}"
    printf "Actual:   '%s'\n" "${OUTPUT}"
    exit 1
fi

print_success "Binary functionality verified"

# Step 3: Warm up the system (run a few times to avoid cold start effects)
print_info "Warming up system..."
for i in {1..5}; do
    ./"${BINARY_NAME}" >/dev/null 2>&1
done

# Step 4: Single execution timing test
print_info "Measuring single execution time..."
if command -v time >/dev/null 2>&1; then
    # Use time command for single execution measurement
    SINGLE_TIME_OUTPUT=$(timeout 10s time -f "%e" ./"${BINARY_NAME}" 2>&1 >/dev/null)
    SINGLE_TIME_SEC=$(echo "${SINGLE_TIME_OUTPUT}" | tail -1)
    SINGLE_TIME_MS=$(echo "${SINGLE_TIME_SEC} * 1000" | bc -l 2>/dev/null || python3 -c "print(${SINGLE_TIME_SEC} * 1000)")
    SINGLE_TIME_MS_INT=$(printf "%.0f" "${SINGLE_TIME_MS}")
else
    print_warning "time command not available, using date-based measurement"
    START_TIME=$(date +%s%N)
    ./"${BINARY_NAME}" >/dev/null 2>&1
    END_TIME=$(date +%s%N)
    SINGLE_TIME_NS=$((END_TIME - START_TIME))
    SINGLE_TIME_MS=$((SINGLE_TIME_NS / 1000000))
    SINGLE_TIME_MS_INT=${SINGLE_TIME_MS}
fi

print_info "Single execution time: ${SINGLE_TIME_MS_INT}ms"

# Step 5: Batch execution timing test
print_info "Running ${ITERATIONS} iterations for statistical analysis..."

# Create a temporary file for timing results
TEMP_RESULTS=$(mktemp)
trap "rm -f ${TEMP_RESULTS}" EXIT

# Measure execution times
START_BATCH=$(date +%s%N)
for ((i=1; i<=ITERATIONS; i++)); do
    if command -v time >/dev/null 2>&1; then
        ITER_TIME=$(timeout 5s time -f "%e" ./"${BINARY_NAME}" 2>&1 >/dev/null | tail -1)
        echo "${ITER_TIME}" >> "${TEMP_RESULTS}"
    else
        # Fallback to date-based measurement
        ITER_START=$(date +%s%N)
        ./"${BINARY_NAME}" >/dev/null 2>&1
        ITER_END=$(date +%s%N)
        ITER_TIME_NS=$((ITER_END - ITER_START))
        ITER_TIME_SEC=$(echo "scale=6; ${ITER_TIME_NS} / 1000000000" | bc -l 2>/dev/null || python3 -c "print(${ITER_TIME_NS} / 1000000000)")
        echo "${ITER_TIME_SEC}" >> "${TEMP_RESULTS}"
    fi
    
    # Progress indicator (only in non-quiet mode)
    if [[ "${QUIET_MODE}" == "false" && $((i % 25)) -eq 0 ]]; then
        printf "  Progress: %d/%d iterations\n" "${i}" "${ITERATIONS}"
    fi
done
END_BATCH=$(date +%s%N)

# Step 6: Calculate statistics
if command -v python3 >/dev/null 2>&1; then
    # Use Python for statistical analysis
    STATS_OUTPUT=$(python3 -c "
import sys
import math

times = []
with open('${TEMP_RESULTS}', 'r') as f:
    for line in f:
        times.append(float(line.strip()))

times_ms = [t * 1000 for t in times]
n = len(times_ms)
mean_ms = sum(times_ms) / n
variance = sum((x - mean_ms) ** 2 for x in times_ms) / n
std_dev_ms = math.sqrt(variance)
min_ms = min(times_ms)
max_ms = max(times_ms)

# Calculate median
sorted_times = sorted(times_ms)
if n % 2 == 0:
    median_ms = (sorted_times[n//2 - 1] + sorted_times[n//2]) / 2
else:
    median_ms = sorted_times[n//2]

print(f'{mean_ms:.2f},{std_dev_ms:.2f},{min_ms:.2f},{max_ms:.2f},{median_ms:.2f}')
")
    
    IFS=',' read -r MEAN_MS STD_DEV_MS MIN_MS MAX_MS MEDIAN_MS <<< "${STATS_OUTPUT}"
else
    # Fallback to basic statistics using shell
    print_warning "Python3 not available, using basic shell calculations"
    TOTAL_SEC=0
    COUNT=0
    while IFS= read -r line; do
        TIME_MS=$(echo "${line} * 1000" | bc -l 2>/dev/null || python3 -c "print(${line} * 1000)" 2>/dev/null || echo "1")
        TOTAL_SEC=$(echo "${TOTAL_SEC} + ${line}" | bc -l 2>/dev/null || echo "${TOTAL_SEC}")
        COUNT=$((COUNT + 1))
    done < "${TEMP_RESULTS}"
    
    MEAN_MS=$(echo "${TOTAL_SEC} * 1000 / ${COUNT}" | bc -l 2>/dev/null || echo "1")
    STD_DEV_MS="N/A"
    MIN_MS="N/A"
    MAX_MS="N/A"
    MEDIAN_MS="N/A"
fi

# Step 7: Calculate total batch time
TOTAL_BATCH_NS=$((END_BATCH - START_BATCH))
TOTAL_BATCH_MS=$((TOTAL_BATCH_NS / 1000000))
AVG_BATCH_MS=$((TOTAL_BATCH_MS / ITERATIONS))

# Step 8: Performance analysis and thresholds
MEAN_MS_INT=$(printf "%.0f" "${MEAN_MS}" 2>/dev/null || echo "1")
PERFORMANCE_PASSED=true

if [[ ${MEAN_MS_INT} -gt ${RUNTIME_THRESHOLD_MS} ]]; then
    PERFORMANCE_PASSED=false
    print_error "Average runtime exceeds threshold!"
    printf "  Average runtime: %s ms\n" "${MEAN_MS}"
    printf "  Threshold: %d ms\n" "${RUNTIME_THRESHOLD_MS}"
fi

if [[ ${SINGLE_TIME_MS_INT} -gt ${STARTUP_TIME_THRESHOLD_MS} ]]; then
    PERFORMANCE_PASSED=false
    print_error "Single execution time exceeds startup threshold!"
    printf "  Single execution: %d ms\n" "${SINGLE_TIME_MS_INT}"
    printf "  Threshold: %d ms\n" "${STARTUP_TIME_THRESHOLD_MS}"
fi

# Step 9: Save benchmark results
BENCHMARK_RESULTS_FILE="test/benchmark_results.txt"
if [[ "${QUIET_MODE}" == "false" ]]; then
    print_info "Saving benchmark results to ${BENCHMARK_RESULTS_FILE}"
fi

{
    echo "# Benchmark results - automatically updated"
    echo "# Format: timestamp,binary_name,iterations,mean_ms,std_dev_ms,min_ms,max_ms,median_ms,single_exec_ms"
    echo "# Last updated: $(date -u '+%Y-%m-%d %H:%M:%S UTC')"
    echo "$(date -u '+%Y-%m-%d %H:%M:%S'),${BINARY_NAME},${ITERATIONS},${MEAN_MS},${STD_DEV_MS},${MIN_MS},${MAX_MS},${MEDIAN_MS},${SINGLE_TIME_MS_INT}"
} > "${BENCHMARK_RESULTS_FILE}"

# Step 10: Report results
if [[ "${PERFORMANCE_PASSED}" == "true" ]]; then
    print_success "Performance benchmark passed"
else
    print_error "Performance benchmark failed"
fi

if [[ "${QUIET_MODE}" == "false" ]]; then
    printf "\n"
    print_info "Benchmark Summary:"
    printf "  - Binary: %s\n" "${BINARY_NAME}"
    printf "  - Iterations: %d\n" "${ITERATIONS}"
    printf "  - Single execution: %d ms\n" "${SINGLE_TIME_MS_INT}"
    printf "  - Average runtime: %s ms\n" "${MEAN_MS}"
    if [[ "${STD_DEV_MS}" != "N/A" ]]; then
        printf "  - Standard deviation: %s ms\n" "${STD_DEV_MS}"
        printf "  - Min/Max: %s/%s ms\n" "${MIN_MS}" "${MAX_MS}"
        printf "  - Median: %s ms\n" "${MEDIAN_MS}"
    fi
    printf "  - Total batch time: %d ms (%d ms average)\n" "${TOTAL_BATCH_MS}" "${AVG_BATCH_MS}"
    printf "  - Status: %s\n" "$(if [[ "${PERFORMANCE_PASSED}" == "true" ]]; then echo "PASSED"; else echo "FAILED"; fi)"
else
    printf "Benchmark: %s (%d iterations, avg %.2f ms)\n" "$(if [[ "${PERFORMANCE_PASSED}" == "true" ]]; then echo "PASSED"; else echo "FAILED"; fi)" "${ITERATIONS}" "${MEAN_MS}"
fi

if [[ "${PERFORMANCE_PASSED}" == "false" ]]; then
    exit 1
fi