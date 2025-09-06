# Unit Tests for Hello World

This directory contains unit tests for the hello.c program.

## Files

- `hello.h` - Header file with function declarations
- `hello_lib.c` - Library containing the testable functions  
- `hello.c` - Main program that uses the library
- `test_hello.c` - Unit tests for the library functions
- `Makefile` - Build system for compiling and running tests

## Building and Running

### Build the main program:
```bash
make
# or manually:
gcc -Wall -Wextra -o hello hello.c hello_lib.c
```

### Run the main program:
```bash
./hello
```

### Run unit tests:
```bash
make test
# or manually:
gcc -Wall -Wextra -o hello_test test_hello.c hello_lib.c
./hello_test
```

### Clean compiled files:
```bash
make clean
```

## Test Coverage

The unit tests cover the `get_greeting()` function with the following test cases:

1. **test_get_greeting_returns_hello_world** - Verifies the function returns the correct greeting string
2. **test_get_greeting_not_null** - Ensures the function doesn't return NULL
3. **test_get_greeting_not_empty** - Verifies the returned string is not empty

All tests use a simple assertion-based framework built into the test file.