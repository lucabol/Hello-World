# Contributing to Hello World

Thank you for your interest in contributing to this Hello World C program! This document outlines the coding standards and development workflow for this project.

## Coding Standards

### Compiler Warnings

All code must compile cleanly with strict compiler warnings enabled. When developing locally, always use the strict compilation flags:

```bash
# Use the make target for strict compilation (recommended)
make strict

# Or use gcc directly with strict flags
gcc -std=c99 -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -o hello hello.c
```

### Required Warning Flags

- `-Wall` - Enable all commonly used warning flags
- `-Wextra` - Enable extra warning flags not covered by -Wall
- `-Wpedantic` - Strict ISO C compliance warnings
- `-Wformat=2` - Strict format string checking
- `-Wconversion` - Warn about type conversions that may alter values
- `-Wsign-conversion` - Warn about sign conversions
- `-Werror` - Treat warnings as errors (enforced in CI)

### Code Style

- Use standard C99 syntax (`-std=c99`)
- Include clear, descriptive comments for functions and complex logic
- Use meaningful variable and function names
- Follow existing code structure and style

## Development Workflow

### Building and Testing

1. **Standard Development Build:**
   ```bash
   make build    # Basic build with warnings
   ```

2. **Strict Build (Required before submitting):**
   ```bash
   make strict   # Build with strict flags and -Werror
   ```

3. **Run Tests:**
   ```bash
   make test     # Run full validation suite
   ```

4. **Clean Build Artifacts:**
   ```bash
   make clean    # Remove compiled binaries
   ```

### Testing Your Changes

Before submitting any changes, ensure they pass all validation:

```bash
# Run the complete validation suite
make test

# This will:
# 1. Compile with strict flags (including -Werror)
# 2. Verify the program runs correctly
# 3. Check output format matches specification
# 4. Validate exit codes
```

### Alternative Compilers

The project supports both GCC and Clang:

```bash
make clang    # Build with Clang compiler
```

Both compilers should produce identical output and pass all tests.

## Continuous Integration

The CI pipeline enforces code quality through multiple jobs:

1. **Strict Build Job** - Uses `make strict` to ensure code compiles with all strict flags
2. **Lint Check Job** - Performs syntax-only compilation with strict warnings as errors
3. **Standard Build Jobs** - Tests with both GCC and Clang compilers

All jobs must pass before changes can be merged.

## Output Specification

The program must output exactly `Hello world!` with:
- No trailing newline
- Exit code 0
- No additional output to stdout or stderr

This specification is strictly enforced by the validation tests.

## Makefile Targets

| Target | Description |
|--------|-------------|
| `hello` (default) | Basic build with C99 standard |
| `build` | Build with standard warnings (-Wall -Wextra) |
| `strict` | Strict build with pedantic warnings and -Werror |
| `debug` | Debug build with symbols |
| `clang` | Build with Clang compiler |
| `test` | Build strict and run validation tests |
| `test-quiet` | Run tests with minimal output (CI-style) |
| `clean` | Remove compiled binaries |
| `help` | Show available targets |

## Questions or Issues

If you have questions about these standards or encounter issues with the build process, please open an issue in the repository.