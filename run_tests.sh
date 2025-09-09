#!/bin/bash

# Simple test runner for hello.c program
set -e

echo "Building hello program..."
gcc -Wall -Wextra -o hello hello.c

echo "Building test program..."
gcc -Wall -Wextra -o test_hello test_hello.c

echo ""
echo "Running unit tests..."
./test_hello

echo ""
echo "All tests completed successfully!"