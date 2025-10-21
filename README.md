# Hello World

A simple C "Hello World" program demonstrating standard build practices and testing.

## Quick Start

```bash
# Build the program
make

# Run it
./hello

# Run all validation tests
make test
```

## Build Targets

### Standard Builds
- `make` or `make all` - Build optimized binary with warnings enabled
- `make strict` - Build with strict compiler flags (warnings as errors, C99 standard)
- `make debug` - Build with debug symbols for debugging
- `make clang` - Build using the Clang compiler (produces `hello_clang`)

### Testing
- `make test` - Run validation tests with verbose output
- `make test-quiet` - Run validation tests in quiet mode (CI-friendly)
- `make unit-test` - Build and run unit tests

### Maintenance
- `make clean` - Remove all build artifacts

## Development Workflow

1. Make changes to `hello.c`
2. Build with strict flags: `make strict`
3. Run validation: `make test`
4. Clean up: `make clean`

## Expected Output

The program outputs exactly:
```
Hello world!
```
(with a trailing newline)

Exit code: 0

## Requirements

- GCC or Clang compiler
- GNU Make
- Bash (for test scripts)

## Project Structure

```
.
├── Makefile           # Build configuration
├── README.md          # This file
├── hello.c            # Main source file
└── test/              # Test scripts
    ├── validate.sh    # Validation test script
    └── run_unit_tests.sh  # Unit test runner
```