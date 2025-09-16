# Testing Documentation for Hello World

## Overview

This document describes the unit testing approach for the hello.c program.

## Test Infrastructure

### Files
- `test_hello.c` - Unit test program that validates hello.c functionality
- `Makefile` - Build configuration with test targets

### Test Framework
The tests use a simple custom test framework that:
- Executes the compiled hello binary as a subprocess
- Captures stdout output using pipes
- Monitors exit codes
- Provides assertion functions for validation

## Running Tests

### Quick Test
```bash
make test
```

### Step-by-Step Testing
```bash
# Build the main program
make hello

# Build the test program  
make test_hello

# Run tests
./test_hello
```

### Manual Testing
```bash
# Build and run manually
gcc -o hello hello.c
./hello
echo "Exit code: $?"
```

## Test Cases

### 1. Output Verification
- **Test**: `Hello program outputs correct message`
- **Validates**: Program prints exactly "Hello world!" to stdout
- **Method**: Captures stdout via pipe and compares with expected string

### 2. Exit Code Verification  
- **Test**: `Hello program exits with code 0`
- **Validates**: Program exits successfully with status code 0
- **Method**: Monitors child process exit status

## Expected Results

### Successful Test Run
```
Running tests for hello.c...

PASS: Hello program outputs correct message
PASS: Hello program exits with code 0

Test Results:
Tests run: 2
Tests passed: 2
Tests failed: 0

All tests PASSED!
```

### Error Handling
If the hello executable is missing:
```
ERROR: ./hello executable not found or not executable.
Please compile with: gcc -o hello hello.c
```

## Build Targets

### Primary Targets
- `make all` - Build hello program
- `make test` - Build and run all tests
- `make clean` - Remove compiled files

### Development Targets
- `make debug` - Build with debug symbols
- `make strict` - Build with strict warning flags
- `make optimized` - Build with optimization flags

## Test Design Principles

1. **Minimal Dependencies**: Tests use only standard C library functions
2. **Subprocess Testing**: Tests the actual compiled binary, not internal functions
3. **Comprehensive Coverage**: Tests both output and exit behavior
4. **Error Handling**: Graceful handling of missing dependencies
5. **Clear Reporting**: Human-readable test results with pass/fail status

## Integration with CI/CD

The test suite can be integrated into continuous integration pipelines:

```bash
# Build and test in CI
make clean
make test
```

Exit codes:
- `0` - All tests passed
- `1` - Tests failed or setup error