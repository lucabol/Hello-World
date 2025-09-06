# Testing Documentation

## Automated Test Suite

This repository includes a comprehensive automated test suite using the CUnit testing framework to validate the functionality of the Hello World program.

### Test Framework
- **Framework**: CUnit (C Unit Testing Framework)
- **Test File**: `test_hello.c`
- **Tests Implemented**: 4 comprehensive test cases

### Test Cases

#### 1. Function-Level Tests
- **`test_print_greeting_return_value`**: Validates that `print_greeting()` returns 0 (success)
- **`test_print_greeting_output`**: Verifies that `print_greeting()` produces the correct "Hello world!" output

#### 2. Program-Level Tests
- **`test_hello_program_exit_code`**: Ensures the compiled program exits with code 0
- **`test_hello_program_output`**: Validates the complete program output matches expected result

### Building and Running Tests

#### Prerequisites
```bash
sudo apt install libcunit1-dev
```

#### Quick Test Run
```bash
make test
```

#### Manual Testing Steps
```bash
# Build main program
make all

# Build test suite
make build-all

# Run tests
./test_hello

# Clean up
make clean
```

#### Available Make Targets
- `make test` - Build and run all tests (recommended)
- `make all` - Build main hello program
- `make build-all` - Build both main program and tests
- `make clean` - Remove compiled binaries
- `make run` - Run the hello program
- `make debug` - Build with debugging symbols
- `make strict` - Build with strict compiler warnings
- `make help` - Show all available targets

### Test Results Example
```
CUnit - A unit testing framework for C - Version 2.1-3

Suite: Hello World Test Suite
  Test: test print_greeting return value ...passed
  Test: test print_greeting output ...passed
  Test: test hello program exit code ...passed
  Test: test hello program output ...passed

Run Summary:    Type  Total    Ran Passed Failed Inactive
              suites      1      1    n/a      0        0
               tests      4      4      4      0        0
             asserts     11     11     11      0      n/a

Elapsed time =    0.000 seconds
```

### Code Structure

#### Source Files
- `hello.c` - Main program entry point
- `hello_lib.c` - Library containing testable functions
- `hello.h` - Header file with function declarations
- `test_hello.c` - CUnit test suite
- `Makefile` - Build configuration

#### Testing Strategy
1. **Unit Testing**: Tests individual functions in isolation
2. **Integration Testing**: Tests the complete program behavior
3. **Output Validation**: Captures and validates program output
4. **Exit Code Testing**: Ensures proper program termination

### Continuous Integration Ready
The test suite is designed to integrate with CI/CD pipelines:
- Tests complete in under 1 second
- Returns proper exit codes (0 for success, non-zero for failure)
- Provides verbose output for debugging
- No external dependencies beyond CUnit

### Validation Scenarios Covered
- ✅ Function return values
- ✅ Standard output content
- ✅ Program exit codes
- ✅ Complete program behavior
- ✅ Cross-compilation compatibility
- ✅ Strict compiler warning compliance