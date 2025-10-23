# Contributing to Hello-World

Thank you for your interest in contributing to this project!

## Building the Project

The project uses a Makefile for building. The main targets are:

- `make all` - Builds the hello program and metrics_tool (default)
- `make hello` - Builds just the hello program
- `make metrics_tool` - Builds the code metrics analysis tool
- `make unit-test` - Builds and runs unit tests
- `make clang` - Builds with Clang compiler
- `make clean` - Removes all build artifacts

## Compiler Flags and CI Quality Standards

### -Werror Usage in CI

The project **intentionally uses `-Werror` in CI** to maintain high code quality by treating all compiler warnings as errors. This helps catch potential issues early before they become problems.

**Important for Contributors:**
- CI builds use strict compilation flags including `-Werror`
- Your code must compile cleanly without any warnings
- The flags used in CI are:
  - GCC: `-Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99`
  - For POSIX code (metrics tool): Add `-D_POSIX_C_SOURCE=200809L`
  - Clang: Same flags but without `-Werror` for cross-compiler portability

**Before submitting a PR:**
1. Build with `make all` to ensure it compiles cleanly
2. Run `make unit-test` to ensure all tests pass
3. Test with both GCC and Clang if possible:
   - `make all` (GCC)
   - `make clang` (Clang)
   - `make metrics_tool_clang` (Clang for metrics tool)

### Why -Werror?

Using `-Werror` in CI helps:
- Catch potential bugs early (uninitialized variables, type mismatches, etc.)
- Maintain consistent code quality across contributions
- Prevent accumulation of warnings over time
- Ensure portability across different platforms and compiler versions

### Local Development

For local development, you can disable `-Werror` if needed by modifying CFLAGS in the Makefile, but remember that CI will still enforce it. It's better to fix warnings than suppress them.

## Running Tests

The project has multiple test suites:

1. **Unit Tests**: `make unit-test`
   - Tests core functionality of hello.c
   - Works with both GCC and Clang: `CC=clang make unit-test`

2. **Validation Tests**: `bash test/validate.sh`
   - Validates the hello program output

3. **Code Metrics Tests**: `bash test/test_metrics.sh`
   - Tests the metrics_tool with various edge cases

4. **Memory Safety**: Build with AddressSanitizer
   - `make metrics_tool_asan`
   - Run with `ASAN_OPTIONS=detect_leaks=1 ./metrics_tool_asan`

## Submitting Changes

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Ensure all builds and tests pass
5. Submit a pull request

## Questions?

If you have questions about the build system, compiler flags, or contribution process, feel free to open an issue for discussion.
