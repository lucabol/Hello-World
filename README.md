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

**Compiler Requirements:**
- GCC >= 4.9 or Clang >= 3.5
- The strict compilation flags (`-Werror` with conversion warnings) are tested with GCC 13.3.0 and Clang 14+
- Older compilers may generate additional warnings that need to be addressed

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

**Build Process:**
The test runner script (`test/run_unit_tests.sh`) compiles the code as follows:
1. Compiles `hello.c` with `-DUNIT_TEST` to exclude the `main()` function
2. Links it with `test/test_hello.c` which provides its own `main()` for tests
3. Uses strict compiler flags: `-Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99`

**Note:** Test binaries (`test_hello_runner`, `*.o`) are automatically excluded by `.gitignore`

### Integration Tests

Run the validation script to test compilation and program output:

```bash
bash test/validate.sh
```

**Note:** The `validate.sh` script is separate from the unit tests and performs end-to-end validation by:
- Compiling the full program with strict flags
- Running the binary and checking its output
- Verifying the exit code

The unit tests (`run_unit_tests.sh`) test individual functions, while `validate.sh` validates the complete program behavior.

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

### Building Tests Manually

If you need to build tests manually:

```bash
# Compile hello.c without main() using -DUNIT_TEST
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -I. -c -o hello_lib.o hello.c -DUNIT_TEST

# Compile and link the test runner
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -I. -o test_hello_runner test/test_hello.c hello_lib.o

# Run tests
./test_hello_runner

# Clean up
rm hello_lib.o test_hello_runner
```

The `-DUNIT_TEST` flag is crucial—it tells the preprocessor to exclude `main()` from `hello.c` so the test runner can provide its own.

## Testing Framework Details

The `simple_test.h` framework provides the following assertions:

- `TEST_ASSERT_EQUAL_STRING(expected, actual)` - Compares two strings
- `TEST_ASSERT_NOT_NULL(ptr)` - Verifies a pointer is not NULL
- `TEST_ASSERT_EQUAL_INT(expected, actual)` - Compares two integers

All assertions automatically track test counts and provide colored output for easy debugging.

**Portability Notes:**
- The framework uses ANSI color codes for terminal output. Colors may not display correctly on very old terminals but will not affect test results.
- All code compiles cleanly with the strict flags on GCC >= 4.9 and Clang >= 3.5
- The framework has no external dependencies beyond the C standard library (`stdio.h`, `string.h`)

**Future Enhancements:**
- Additional test coverage (e.g., testing that `get_greeting()` returns static storage)
- CI integration with GitHub Actions to run tests automatically on push/PR
- Support for additional compilers and platforms