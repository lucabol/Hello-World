# Contributing to Hello World

Thank you for your interest in contributing to this project! This document outlines the code quality standards and development workflow.

## Code Quality Standards

### Required Compilation Flags

All code contributions must compile cleanly with strict warning flags enabled. Before submitting a pull request, ensure your code compiles without warnings using:

```bash
make strict
```

This runs the following compilation flags:
- `-Wall` - Enable all common warnings
- `-Wextra` - Enable extra warning flags
- `-Wpedantic` - Issue warnings for code that doesn't conform to ISO C
- `-Wformat=2` - Enable format string security warnings
- `-Wconversion` - Warn for implicit type conversions
- `-Wsign-conversion` - Warn for sign conversions

### Development Workflow

1. **Build with warnings enabled (default):**
   ```bash
   make         # or make dev
   ```

2. **Test your code:**
   ```bash
   make test
   ```

3. **Run static analysis:**
   ```bash
   make lint
   ```

4. **Clean build artifacts:**
   ```bash
   make clean
   ```

### Available Build Targets

- `make` or `make dev` - Development build with `-Wall -Wextra` (default)
- `make standard` - Basic build with minimal flags
- `make optimized` - Optimized build with `-O2`
- `make strict` - Strict compilation with maximum warnings
- `make debug` - Debug build with `-g` flag
- `make test` - Build and run the program
- `make lint` - Check code quality with strict flags
- `make clean` - Remove compiled binaries
- `make help` - Show all available targets

### Before Submitting a PR

1. Ensure your code compiles without warnings: `make strict`
2. Test the program runs correctly: `make test`
3. Run the linter: `make lint`
4. Clean up build artifacts: `make clean`

### Code Style

- Use proper C style formatting
- Include necessary headers (e.g., `#include <stdio.h>`)
- Functions should have explicit return types and return statements
- Use `int main(void)` for the main function
- Add newlines to printf statements where appropriate

Example of good code style:
```c
#include <stdio.h>

int main(void) {
    printf("Hello world!\n");
    return 0;
}
```