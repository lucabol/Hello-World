#!/bin/bash
# Example: Profile with gprof

set -e

echo "=== gprof Profiling Example ==="
echo ""

# Compile with profiling enabled
echo "Compiling with -pg flag..."
gcc -pg -o hello_prof hello.c

# Run the program (generates gmon.out)
echo "Running program to generate profile data..."
./hello_prof

# Generate profile report
echo ""
echo "Generating profile report..."
gprof hello_prof gmon.out > gprof_analysis.txt

echo ""
echo "Profile report saved to: gprof_analysis.txt"
echo ""
echo "=== Profile Summary ==="
head -50 gprof_analysis.txt

# Cleanup
rm -f hello_prof gmon.out

echo ""
echo "Complete! Full report in gprof_analysis.txt"
