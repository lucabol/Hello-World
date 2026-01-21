#!/bin/bash
# Example: Memory analysis with Valgrind

set -e

echo "=== Valgrind Memory Analysis Example ==="
echo ""

# Compile with debug symbols
echo "Compiling with debug symbols..."
gcc -g -o hello_debug hello.c

# Run memcheck
echo ""
echo "Running Valgrind memcheck..."
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./hello_debug 2>&1 | tee valgrind_memcheck.txt

echo ""
echo "Memory check complete! Report saved to: valgrind_memcheck.txt"

# Cleanup
rm -f hello_debug

echo ""
echo "=== Summary ==="
echo "Check valgrind_memcheck.txt for detailed memory analysis"
