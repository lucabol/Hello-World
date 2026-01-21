# Hello World
Test repo for JediMaster

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

## Building

Compile the program:
```bash
make
```

Or compile with strict warnings:
```bash
make strict
```

## Testing

Run all tests:
```bash
make test
```