# Testing Documentation

This document describes the unit testing infrastructure for the hello.c program.

## Overview

The testing infrastructure validates that the hello.c program:
- Outputs the correct message ("Hello world!")
- Exits with the expected code (0)

## Files

- `test_hello.c` - Unit test program that validates hello.c functionality
- `run_tests.sh` - Shell script that builds and runs all tests
- `TESTING.md` - This documentation file

## Running Tests

### Quick Start
```bash
# Run all tests with the provided script
./run_tests.sh
```

### Manual Testing
```bash
# Build the hello program
gcc -Wall -Wextra -o hello hello.c

# Build the test program  
gcc -Wall -Wextra -o test_hello test_hello.c

# Run the tests
./test_hello
```

## Test Output

When all tests pass:
```
Running unit tests for hello.c
================================
✓ PASS: Hello program outputs correct message
✓ PASS: Hello program exits with code 0
================================
Tests completed: 2/2 passed
```

When tests fail, you'll see detailed information about what was expected vs. actual:
```
✗ FAIL: Hello program outputs correct message
  Expected: "Hello world!"
  Actual:   "Wrong message!"
```

## Test Implementation

The test program (`test_hello.c`) uses a simple testing framework that:
1. Forks a child process to run the hello program
2. Captures the stdout output via pipes
3. Waits for the child process and captures its exit code
4. Compares both output and exit code against expected values

## Exit Codes

- `0` - All tests passed
- `1` - One or more tests failed

This allows the test results to be easily integrated into CI/CD pipelines or automated build systems.