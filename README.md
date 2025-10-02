# Hello World
Test repo for JediMaster

## Overview

This repository contains a minimal "Hello World" C program with automated testing infrastructure, including:
- Unit tests using a lightweight testing framework
- Integration testing via validation scripts
- Strict compilation with multiple warning flags

## Building the Program

### Basic Build

```bash
gcc -o hello hello.c
./hello
```

**Expected Output:** `Hello world!` (without trailing newline)

### Build with Strict Warnings

```bash
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -o hello hello.c
```

## Testing

### Unit Tests

The project includes automated unit tests for the greeting functionality.

**Run unit tests:**
```bash
bash test/run_unit_tests.sh
```

The unit tests verify:
- The greeting function returns a non-null pointer
- The greeting contains the expected text "Hello world!"
- The greeting has the correct length (12 characters)
- The greeting does not contain a trailing newline

**Test Framework:**
The project uses a lightweight, self-contained testing framework (`test/simple_test.h`) with no external dependencies. This framework provides:
- Colored test output
- Multiple assertion types (string equality, null checks, integer equality)
- Test summary with pass/fail counts

### Integration Tests

Run the validation script to test compilation and program output:

```bash
bash test/validate.sh
```

## Repository Structure

```
.
├── hello.c               # Main C source file
├── hello.h               # Header file with function declarations
├── test/
│   ├── simple_test.h     # Lightweight testing framework
│   ├── test_hello.c      # Unit tests for hello.c
│   ├── run_unit_tests.sh # Script to build and run unit tests
│   └── validate.sh       # Integration validation script
├── README.md             # This file
└── .gitignore            # Git ignore rules
```

## Development Workflow

1. **Make changes** to `hello.c` or related files
2. **Run unit tests**: `bash test/run_unit_tests.sh`
3. **Run integration tests**: `bash test/validate.sh`
4. **Commit** your changes

## Adding New Tests

To add new unit tests:

1. Open `test/test_hello.c`
2. Add a new test function following the existing pattern:
   ```c
   void test_my_new_test(void) {
       // Your test code here
       TEST_ASSERT_EQUAL_STRING("expected", actual);
   }
   ```
3. Add the test to the main function:
   ```c
   RUN_TEST(test_my_new_test);
   ```
4. Run `bash test/run_unit_tests.sh` to verify

## Testing Framework Details

The `simple_test.h` framework provides the following assertions:

- `TEST_ASSERT_EQUAL_STRING(expected, actual)` - Compares two strings
- `TEST_ASSERT_NOT_NULL(ptr)` - Verifies a pointer is not NULL
- `TEST_ASSERT_EQUAL_INT(expected, actual)` - Compares two integers

All assertions automatically track test counts and provide colored output for easy debugging.