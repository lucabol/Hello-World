# Hello World
Test repo for JediMaster

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