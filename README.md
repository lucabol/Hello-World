# Hello World

Test repo for JediMaster

## Code Metrics Tool

This repository includes a spreadsheet-like code metrics analyzer that can analyze C source files and display various metrics in a tabular format.

See [CODE_METRICS_README.md](CODE_METRICS_README.md) for details.

Quick start:
```bash
make code_metrics
./code_metrics hello.c
```

## Building and Testing

This repository includes a comprehensive Makefile for building and testing the Hello World C program.

### Quick Start

```bash
make              # Build all binaries (hello and code_metrics)
./hello           # Run the program
make test-all     # Run all tests
```

### Available Make Targets

- **`make`** or **`make all`** - Build all binaries (hello and code_metrics)
- **`make build`** - Build the release binary with optimizations (`-O2`)
- **`make strict`** - Build with strict flags and `-Werror` (used by CI for validation)
- **`make debug`** - Build with debug symbols (`-g`)
- **`make clang`** - Build using the Clang compiler
- **`make code_metrics`** - Build the code metrics analyzer
- **`make code_metrics_strict`** - Build code metrics with strict warnings
- **`make unit-test`** - Compile and run unit tests
- **`make test`** - Run all tests (unit tests + validation script)
- **`make test-metrics`** - Run code metrics tests
- **`make test-all`** - Run all tests including code metrics
- **`make test-quiet`** - Run tests with minimal output (for CI/automated environments)
- **`make clean`** - Remove all build artifacts
- **`make help`** - Display help message with available targets

### CI and Automated Testing

For CI environments, use `make test-quiet` which:
- Minimizes verbose output from build and test processes
- Highlights only test results and errors
- Uses the `--quiet` flag for validation scripts
- Suitable for automated build pipelines

The project includes GitHub Actions workflows that build and test the code with both GCC and Clang, using strict compiler warnings (`-Wall -Wextra -Wpedantic -Werror`).

### Customizing the Build

You can override the compiler and other variables:

```bash
make CC=clang build           # Build with Clang
make CC=gcc-12 strict         # Build with a specific GCC version
make CC=clang code_metrics_strict  # Build code_metrics with Clang
```

### Requirements

- GCC or Clang compiler
- Bash (for test scripts)
- Standard C library

### Output Format

The program outputs exactly two lines:
```
Ciao, Mondo!
Exit code: 0
```

**Important:** This exact format (including the "Exit code: 0" line and trailing newlines) is validated by the test suite to ensure consistency. Do not modify the output format without also updating the validation tests in `test/validate.sh`.

### Unit Testing

The `hello.c` file supports unit testing through conditional compilation using the `UNIT_TEST` macro.

When compiled with `-DUNIT_TEST`, the `main()` function is excluded, allowing the code to be linked with a separate test runner that defines its own `main()`. This enables testing of the `get_greeting()` function in isolation.

To run unit tests:
```bash
make unit-test
```

The unit tests compile `hello.c` as a library (with `-c -DUNIT_TEST`) and link it with `test/test_hello.c`, which contains the test cases and test runner's `main()` function.
