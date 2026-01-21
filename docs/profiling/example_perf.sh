#!/bin/bash
# Example: Performance analysis with perf

set -e

echo "=== perf Performance Analysis Example ==="
echo ""

# Check if perf is available
if ! command -v perf &> /dev/null; then
    echo "Warning: perf is not installed"
    echo "Install with: sudo apt-get install linux-tools-common linux-tools-generic"
    exit 1
fi

# Compile with debug symbols
echo "Compiling with debug symbols..."
gcc -g -fno-omit-frame-pointer -o hello_debug hello.c

# Basic performance statistics
echo ""
echo "=== Basic Performance Statistics ==="
perf stat ./hello_debug 2>&1 | tee perf_stats.txt

# Record performance data
echo ""
echo "=== Recording Performance Data ==="
perf record -g ./hello_debug

# Generate report
echo ""
echo "=== Performance Report ==="
perf report --stdio > perf_report.txt 2>&1 || true
head -100 perf_report.txt

echo ""
echo "Reports saved to:"
echo "  - perf_stats.txt (performance statistics)"
echo "  - perf_report.txt (detailed report)"
echo "  - perf.data (raw data - can be viewed with 'perf report')"

# Cleanup
rm -f hello_debug

echo ""
echo "Complete!"
