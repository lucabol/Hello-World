# Hello World

A simple C "Hello World" program that demonstrates basic C programming concepts.

## Quick Start

### Using the Makefile (Recommended)

This repository includes a standardized Makefile for consistent builds and testing:

```bash
# Build optimized version (default)
make

# Run the program
make run

# Run all tests (strict compilation + execution)
make test

# Build debug version
make debug

# Build with Clang
make clang

# Clean build artifacts
make clean

# Show all available targets
make help
```

### Validation Script

Run the validation script to ensure the program builds and works correctly:

```bash
# Run validation (builds with strict flags and tests output)
./validate.sh

# Alternative path
./test/validate.sh
```

### Manual Build Commands

You can also build manually if needed:

```bash
# Basic build
gcc -o hello hello.c

# Development build with warnings
gcc -Wall -Wextra -o hello hello.c

# Run the program
./hello
```

## Expected Output

When you run the program, it will produce the following exact output:

```
Hello world!
```

**Important:** The program intentionally outputs "Hello world!" **without a trailing newline** and exits with code 0. This behavior is validated by the test scripts to ensure exact byte-level output compliance.

## Validation and Testing

### Local Testing

To reproduce what CI runs locally, use these commands:

```bash
# Run the same validation that CI uses
make test

# Or run validation script directly
./test/validate.sh

# For quiet output (like CI)
make test-quiet
```

### Manual Validation Steps

**ALWAYS run these validation steps after making any changes:**

1. **Compile the program:** `make strict` (or `gcc -Wall -Wextra -Wpedantic -Werror -o hello hello.c`)
2. **Run the program:** `./hello`
3. **Verify output is exactly:** `Hello world!` (no trailing newline)
4. **Check exit code:** `echo $?` should return `0`

### CI Process

The continuous integration runs these targets:

- **`make test`** - Builds with strict warnings and validates output
- Uses **`test/validate.sh`** script which:
  - Builds using `make strict` (with `-Werror` for quality assurance)
  - Validates exact output format: "Hello world!" with no trailing newline
  - Verifies exit code is 0
  - Performs byte-level output verification

### Build Targets

- **`make`** - Default optimized build
- **`make strict`** - Build with strict warnings and `-Werror` (used by CI)
- **`make debug`** - Debug build with `-g` flag
- **`make clang`** - Build with Clang compiler
- **`make test`** - Run validation (what CI uses)
- **`make clean`** - Remove all build artifacts

## Development Guidelines

- Always use `make test` or `./test/validate.sh` before committing
- The program must output exactly "Hello world!" with no trailing newline
- Exit code must always be 0
- All builds should compile without warnings under strict flags