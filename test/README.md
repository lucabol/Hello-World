# Comprehensive Test Suite for hello.c

This directory contains a comprehensive test suite for the hello.c program that validates edge cases and boundary conditions.

## Test Suites

### 1. validate.sh - Basic Validation Tests
The foundational test suite that validates core functionality:
- Strict compilation with `-Wall -Wextra -Wpedantic -Werror`
- Output correctness ("Hello world!" without trailing newline)
- Exit code verification (returns 0)
- Byte-level output validation

**Usage:**
```bash
bash test/validate.sh
bash test/validate.sh --quiet  # Minimal output for CI
```

### 2. test_optimization_levels.sh - Optimization Level Tests
Tests compilation and behavior with different GCC optimization levels:
- `-O0` (no optimization)
- `-O1` (basic optimization)
- `-O2` (moderate optimization)
- `-O3` (aggressive optimization)
- `-Os` (optimize for size)
- `-Ofast` (aggressive speed optimization, may break standards)

Validates that output and behavior remain consistent across all optimization levels.

**Usage:**
```bash
bash test/test_optimization_levels.sh
```

### 3. test_locale_settings.sh - Locale Settings Tests
Tests program behavior with different locale settings:
- `C` (POSIX minimal locale)
- `C.UTF-8` (POSIX with UTF-8)
- `en_US.UTF-8` (US English with UTF-8)
- `POSIX` (POSIX standard locale)

Ensures the program works correctly regardless of locale settings and that ASCII output is unaffected by localization.

**Usage:**
```bash
bash test/test_locale_settings.sh
```

### 4. test_output_streams.sh - Output Stream Redirection Tests
Tests output stream handling:
- Verifies output goes to stdout (not stderr)
- Tests stdout redirection to files
- Tests stdout piping
- Validates both streams can be independently redirected

**Usage:**
```bash
bash test/test_output_streams.sh
```

### 5. test_buffer_flushing.sh - Buffer Flushing Tests
Tests output buffer management:
- File redirection (ensures auto-flush on exit)
- Pipe handling (single and multiple stages)
- Command substitution
- Output consistency across multiple runs
- Complete output verification (no partial writes)

**Usage:**
```bash
bash test/test_buffer_flushing.sh
```

### 6. test_warning_flags.sh - Warning Flags Tests
Tests compilation with various warning flag combinations:
- Multiple warning flag combinations from basic to strict
- Different C standards (C89, C90, C99, C11, C17)
- Pedantic-errors mode

Note: C89/C90 may produce warnings about missing return statements but should still compile and produce correct output.

**Usage:**
```bash
bash test/test_warning_flags.sh
```

### 7. test_minimal_environment.sh - Minimal Environment Tests
Tests program behavior in restricted environments:
- Minimal PATH
- Empty environment
- Missing HOME/USER variables
- Different working directories
- Non-interactive mode
- File descriptor limits

**Usage:**
```bash
bash test/test_minimal_environment.sh
```

## Running All Tests

### Using the Master Test Runner:
```bash
bash test/run_all_tests.sh
```

### Using Make:
```bash
make test          # Run all test suites with verbose output
make test-quiet    # Run tests with minimal output (CI mode)
make unit-test     # Run unit tests (CI compatibility)
```

## Test Results

All test suites should pass. The master test runner provides a summary:
```
==========================================
  Test Summary
==========================================
Total test suites: 7
Passed: 7
Failed: 0

✓ All test suites passed!
```

## CI Integration

The test suite integrates with the existing CI workflow:
- `test/validate.sh --quiet` is called during CI builds
- All other test suites can be run via `make test`
- Test scripts are designed to fail fast with clear error messages

## Test Coverage

The comprehensive test suite validates:
- ✓ Compilation with different optimization levels
- ✓ Behavior with different locale settings
- ✓ Output stream redirection (stdout vs stderr)
- ✓ Buffer flushing behavior
- ✓ Compilation with various warning flags
- ✓ Behavior in minimal environments
- ✓ Edge cases and boundary conditions

## Requirements

- GCC compiler (tested with GCC 13.3.0)
- Bash shell
- Standard Unix utilities (od, tee, cat, etc.)

## Notes

- All tests clean up temporary files automatically
- Tests use `hello_test` as the binary name to avoid conflicts
- Tests are designed to be independent and can run in any order
- The test suite is comprehensive but non-invasive (doesn't modify source code)
