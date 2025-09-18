#!/bin/bash

# Basic unit test script for hello.c
# Tests compilation, execution, and output validation

set -e  # Exit on any error

echo "Running unit tests for hello.c..."

# Clean up any existing binary
rm -f hello

# Compile the program
echo "Compiling hello.c..."
gcc -Wall -Wextra -o hello hello.c

if [ ! -f hello ]; then
    echo "Test failed: Compilation did not produce executable"
    exit 1
fi

echo "Compilation successful."

# Run the program and capture the output
echo "Running the program..."
output=$(./hello)

# Check if the output is as expected
if [ "$output" == "Hello world!" ]; then
    echo "Test passed: Output matches expected 'Hello world!'"
else
    echo "Test failed: Expected 'Hello world!' but got '$output'"
    exit 1
fi

# Check exit code
./hello > /dev/null
exit_code=$?
if [ $exit_code -eq 0 ]; then
    echo "Test passed: Program exits with code 0"
else
    echo "Test failed: Program exited with code $exit_code, expected 0"
    exit 1
fi

echo "All tests passed successfully!"