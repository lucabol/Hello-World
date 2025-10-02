# Test Directory

This directory contains automated tests for the Hello World C program.

## Testing Approaches

The project uses two complementary testing approaches:

### 1. C Unit Tests (test_hello.c)

Automated unit tests using the minunit testing framework. These tests:
- Compile the program with various flags
- Verify output is exactly "Hello world!" without trailing newline
- Verify exit code is 0
- Test strict compilation with all warnings enabled

**Run with:**
```bash
make test-unit
```

Or compile and run manually:
```bash
gcc -o test/test_hello test/test_hello.c
./test/test_hello
```

### 2. Shell Validation Script (validate.sh)

Shell-based validation that:
- Builds with strict GCC flags
- Verifies exact output format (byte-level)
- Checks exit codes
- Provides detailed error messages with hex dumps

**Run with:**
```bash
make test-validate           # Normal mode with colored output
make test-quiet              # Quiet mode for CI
bash test/validate.sh        # Direct execution
```

## Running All Tests

To run both unit tests and validation:
```bash
make test
```

This will:
1. Run C unit tests (test_hello.c)
2. Run shell validation (validate.sh)
3. Report overall status

## Testing Framework

### minunit.h

A minimal, header-only unit testing framework for C. Features:
- `mu_assert(message, test)` - Assert a condition
- `mu_run_test(test)` - Run a test function
- Simple pass/fail reporting
- No external dependencies

### Test Structure

Each test function:
- Returns `char *` (NULL on success, error message on failure)
- Uses `mu_assert()` to verify conditions
- Is called via `mu_run_test()` in the main test suite

Example:
```c
static char *test_example(void) {
    mu_assert("Should be true", 1 == 1);
    return 0;
}
```

## CI Integration

Tests are automatically run in CI using:
```bash
make test
```

This ensures all changes are validated before merging.

## Adding New Tests

To add a new test:

1. Add a test function to `test_hello.c`:
```c
static char *test_my_feature(void) {
    mu_assert("Description", condition);
    return 0;
}
```

2. Add to the test suite in `all_tests()`:
```c
static char *all_tests(void) {
    mu_run_test(test_my_feature);
    return 0;
}
```

3. Run tests to verify:
```bash
make test-unit
```

## Expected Output

The program must output exactly:
- Text: "Hello world!" (12 characters)
- No trailing newline
- Exit code: 0

Any deviation from this will cause tests to fail.
