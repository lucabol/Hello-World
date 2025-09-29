# Hello World

Test repo for JediMaster

## Quick Start

```bash
# Build the program
make

# Or build with strict compiler warnings
make strict

# Run the program
./hello

# Run validation tests (reproduces CI checks locally)
make test
```

## Validation

This repository includes a comprehensive validation script that ensures the program meets exact specifications.

### Running Validation

```bash
# Run full validation (same as CI)
./test/validate.sh

# Run validation with minimal output (CI-style)
./test/validate.sh --quiet

# Run validation via make target
make test
```

### What the Validation Checks

The `test/validate.sh` script performs the following checks:

1. **Strict Compilation**: Builds with `-Wall -Wextra -Wpedantic -Werror`
2. **Exit Code**: Verifies the program exits with code 0
3. **Exact Output**: Confirms output is exactly `"Hello world!"` (12 bytes)
4. **No Trailing Newline**: Verifies there is no trailing newline character
5. **Binary Comparison**: Optional byte-level comparison with `test/expected_output.bin`

### Interpreting Validation Failures

If validation fails, the script provides detailed output:

```bash
$ ./test/validate.sh
✗ Output mismatch!
Expected: 'Hello world!'
Actual:   'Hello World!'
Expected length: 12
Actual length:   12
Expected (hex): 00000000  48 65 6c 6c 6f 20 77 6f  72 6c 64 21              |Hello world!|
Actual (hex):   00000000  48 65 6c 6c 6f 20 57 6f  72 6c 64 21              |Hello World!|
```

The hex dump shows the exact byte differences to help identify issues.

### Build Targets

Available make targets:

- `make` or `make all` - Standard build
- `make strict` - Build with strict warnings (used by CI)
- `make debug` - Build with debug information
- `make optimized` - Build with optimizations
- `make test` - Run validation tests
- `make test-quiet` - Run validation with minimal output
- `make clean` - Remove build artifacts
- `make help` - Show available targets

## Program Specification

The hello.c program must:

- Output exactly `"Hello world!"` (no trailing newline)
- Exit with code 0
- Compile without warnings using strict flags
- Complete execution in under 1 second