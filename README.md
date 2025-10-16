# Hello World

Test repo for JediMaster

## Building

```bash
make            # Build the program
make strict     # Build with strict warnings
make clean      # Remove build artifacts
```

## Testing

```bash
make test       # Run validation tests
make unit-test  # Run unit tests
```

## Code Quality

This project uses automated formatting and static analysis to maintain code quality.

### Formatting Code

To auto-format all C source files according to the project style:

```bash
make format
```

### Checking Code Style

To verify code is properly formatted without making changes:

```bash
make format-check
```

### Running Static Analysis

To run format check and clang-tidy static analysis:

```bash
make lint
```

### Before Submitting a PR

All pull requests must pass the automated lint and format checks. Run this before submitting:

```bash
make lint
make test
```

If format check fails, run `make format` to automatically fix formatting issues.

## CI/CD

The GitHub Actions workflow automatically runs:
- Build tests with GCC and Clang
- Code formatting checks
- Static analysis with clang-tidy
- Validation tests

All checks must pass before a PR can be merged.