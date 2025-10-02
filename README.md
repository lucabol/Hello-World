# Hello World

Test repo for JediMaster

## Building

Build the program using:
```bash
make              # Standard build
make strict       # Build with strict warnings (CI-like)
make debug        # Build with debug symbols
make optimize     # Build with optimizations
```

Or directly with GCC:
```bash
gcc -o hello hello.c
```

## Running

```bash
./hello
```

Expected output: `Hello world!` (without trailing newline)

## Testing

The project includes automated unit tests using a lightweight C testing framework (minunit).

### Run all tests:
```bash
make test         # Run all tests (unit + validation)
```

### Run specific test suites:
```bash
make test-unit    # Run C unit tests only
make test-validate # Run shell validation script
make test-quiet   # Run validation in CI mode
```

### Manual test execution:
```bash
# Compile and run unit tests
gcc -o test/test_hello test/test_hello.c
./test/test_hello

# Run validation script
bash test/validate.sh
```

See [test/README.md](test/README.md) for detailed testing documentation.

## Development

```bash
make help         # Show all available targets
make clean        # Remove build artifacts
```

## CI/CD

Tests are automatically run in CI using `make test` to ensure code quality.