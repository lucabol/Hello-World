#!/bin/bash
# Script to set performance baseline
# Run this after validating that current performance is acceptable

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RESULTS_FILE="${SCRIPT_DIR}/benchmark_results.json"
BASELINE_FILE="${SCRIPT_DIR}/baseline.json"

# ANSI color codes
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

if [ ! -f "$RESULTS_FILE" ]; then
    echo -e "${YELLOW}No benchmark results found. Run './perf/benchmark.sh' first.${NC}"
    exit 1
fi

# Copy current results to baseline
cp "$RESULTS_FILE" "$BASELINE_FILE"

echo -e "${GREEN}Baseline set successfully!${NC}"
echo "Baseline saved to: $BASELINE_FILE"
echo ""
echo "Future benchmark runs will be compared against this baseline."
