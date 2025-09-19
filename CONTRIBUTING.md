# Contributing to Hello World

Thank you for your interest in contributing to this simple Hello World C project!

## Code Style Guidelines

This project enforces consistent code formatting and compiler warnings to maintain code quality.

### Formatting Standards

We use `clang-format` with a configuration based on the LLVM style with some customizations:

- **Indentation**: 4 spaces (no tabs)
- **Line length**: 80 characters maximum
- **Braces**: Linux style (opening brace on new line for functions)
- **Includes**: `#include` without spaces before the `<` or `"`
- **Spacing**: Consistent spacing around operators and after keywords

### Formatting Your Code

To format your code locally, use the provided script:

```bash
./format.sh
```

Or run clang-format directly:

```bash
clang-format -i *.c
```

### Compiler Warnings

All code must compile without warnings using strict compiler flags. Use the provided linting script:

```bash
./lint.sh
```

Or run the compiler directly:

```bash
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -o hello hello.c
```

### Development Workflow

1. **Make your changes** to the source code
2. **Format the code**: Run `./format.sh`
3. **Check for warnings**: Run `./lint.sh`
4. **Test the build**: Run `gcc -o hello hello.c && ./hello`
5. **Commit your changes** with a descriptive message

### Continuous Integration

Our CI workflow automatically:
- Builds with both GCC and Clang
- Enforces `-Wall -Wextra` and additional strict warning flags
- Treats warnings as errors (`-Werror`)
- Tests that the program runs correctly

All pull requests must pass these checks before merging.

## Quick Commands

```bash
# Format code
./format.sh

# Check for compiler warnings
./lint.sh

# Build and test
gcc -o hello hello.c && ./hello

# Clean build artifacts
rm -f hello hello_* *.o
```

## Questions?

This is a simple project, but if you have questions about the style guidelines or build process, please open an issue!