# Hello World

A simple C program that prints "Hello world!" - demonstrating clean C programming practices with comprehensive build automation and testing.

## Quick Start

```bash
# Build the program
make

# Run the program
./hello

# Run all tests
make test

# Clean up
make clean
```

## Building

The project uses a Makefile for standardized builds across different environments.

### Basic Build Targets

```bash
# Standard build (default)
make
# or
make build

# Build with strict compilation flags (matches CI)
make strict

# Build with Clang instead of GCC
make clang

# Debug build with symbols
make debug

# Optimized build
make optimized
```

### Build Options

Control build verbosity:
```bash
# Verbose mode - show full compiler commands
make VERBOSE=1

# Default - quiet mode
make
```

Override compiler:
```bash
# Use a different compiler
make CC=gcc-11
```

## Testing

### Unit Tests

Run the unit test suite:
```bash
make unit-test
```

### Validation Tests

Run integration/validation tests:
```bash
# Standard validation with detailed output
make test

# Quiet mode (for CI environments)
make test-quiet
```

## Cleaning

Remove all generated binaries and build artifacts:
```bash
make clean
```

## Available Make Targets

| Target | Description |
|--------|-------------|
| `all` / `build` | Build the standard hello binary (default) |
| `strict` | Build with strict compilation flags (-Werror) for CI |
| `clang` | Build using Clang compiler |
| `unit-test` | Compile and run unit tests |
| `test` | Run validation tests |
| `test-quiet` | Run validation tests with minimal output (CI mode) |
| `clean` | Remove all generated binaries and artifacts |
| `debug` | Build with debug symbols |
| `optimized` | Build with optimizations |
| `help` | Show help message with all targets |

## Development Workflow

Typical development workflow:

```bash
# 1. Make changes to hello.c
vim hello.c

# 2. Build with strict flags to catch issues early
make strict

# 3. Run unit tests
make unit-test

# 4. Run validation tests
make test

# 5. Clean up when done
make clean
```

## CI Integration

The Makefile targets align with CI workflows:

- `make unit-test` - Runs the same unit tests as CI
- `make strict` - Reproduces CI strict-build behavior
- `make test-quiet` - Provides CI-friendly output

## Compiler Flags

The Makefile uses different flag sets for different build types:

- **Standard build**: `-std=c99 -Wall -Wextra -Wpedantic`
- **Strict build**: Adds `-Wformat=2 -Wconversion -Wsign-conversion -Werror`
- **Debug build**: Adds `-g` for debugging symbols
- **Optimized build**: Adds `-O2` for performance optimization

## Requirements

- GCC or Clang compiler
- Make utility
- Bash (for test scripts)

## Project Structure

```
.
├── Makefile           # Build automation
├── README.md          # This file
├── hello.c            # Main C source file
├── hello.h            # Header file with function declarations
└── test/
    ├── validate.sh    # Integration test script
    ├── run_unit_tests.sh  # Unit test runner (legacy)
    ├── test_hello.c   # Unit tests
    └── simple_test.h  # Testing framework
```