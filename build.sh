#!/bin/bash

# Build script for Hello-World project with code metrics

echo "Building Hello-World project..."

# Build the original hello program
echo "Compiling hello.c..."
gcc -Wall -Wextra -o hello hello.c
if [ $? -eq 0 ]; then
    echo "✓ hello.c compiled successfully"
else
    echo "✗ Failed to compile hello.c"
    exit 1
fi

# Build the metrics analyzer
echo "Compiling metrics.c..."
gcc -Wall -Wextra -o metrics metrics.c
if [ $? -eq 0 ]; then
    echo "✓ metrics.c compiled successfully"
else
    echo "✗ Failed to compile metrics.c"
    exit 1
fi

echo ""
echo "Build completed successfully!"
echo ""
echo "Usage:"
echo "  ./hello           - Run the Hello World program"
echo "  ./metrics         - Analyze hello.c code metrics"
echo "  ./metrics <file>  - Analyze any C file's code metrics"
echo ""