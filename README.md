# Hello World

Test repo for JediMaster

## Projects

This repository contains two C programs:

1. **hello.c** - A simple "Hello World" program
2. **code_metrics.c** - A robust file analysis tool with comprehensive error handling

## Code Formatting

This project uses **clang-format** for automated code formatting to ensure consistent code style across all C source files.

### Formatting Rules

The project follows these formatting conventions (defined in `.clang-format`):
- **Indentation**: 4 spaces (no tabs)
- **Brace Style**: Linux style (opening brace on new line for functions)
- **Line Length**: 80 characters maximum
- **Pointer Alignment**: Right-aligned (e.g., `int *ptr`)
- **Include Sorting**: Alphabetically sorted

### Using Code Formatting

#### Format All C Files
To automatically format all C source files:
```bash
make format
```

#### Check Formatting Compliance
To check if code is properly formatted without modifying files:
```bash
make format-check
```

#### Run Formatting Tests
To run the formatting compliance test suite:
```bash
bash test/test_formatting.sh
```

### CI/CD Integration

The repository includes automated formatting checks in CI:
- **Format Check Workflow**: Validates that all code is properly formatted
- **Pull Requests**: Must pass formatting checks before merging
- **Local Testing**: Run `make format-check` before committing

### Best Practices

1. **Before Committing**: Always run `make format` to format your code
2. **IDE Integration**: Configure your IDE to use the `.clang-format` file
3. **Pre-commit Hook**: Consider setting up a git pre-commit hook to auto-format
4. **Code Reviews**: Formatting issues are automatically detected in CI

## Quick Start

### Building
```bash
make          # Build both programs
make hello    # Build hello only
make code_metrics  # Build code_metrics only
```

Or compile with strict warnings:
```bash
make strict
```

### Testing
```bash
make test            # Run all tests
make test-hello      # Test hello.c only
make test-metrics    # Test code_metrics.c only
```

## Documentation

- [Code Metrics Tool Documentation](docs/CODE_METRICS.md) - Comprehensive guide to the code metrics analyzer

## Architecture Decision Records

For detailed documentation on key architectural decisions, see our [Architecture Decision Records (ADRs)](docs/adr/README.md):

- [ADR-001: No Trailing Newline in Output](docs/adr/ADR-001-no-trailing-newline.md) - Why the program outputs without a trailing newline
- [ADR-002: Byte-Level Validation](docs/adr/ADR-002-byte-level-validation.md) - Our validation methodology using hex dumps
- [ADR-003: Dual Compiler Support](docs/adr/ADR-003-dual-compiler-support.md) - Why we support both GCC and Clang
