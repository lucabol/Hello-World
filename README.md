# Hello World
Test repo for JediMaster

## Building and Testing

This repository includes a comprehensive Makefile for building and testing the Hello World C program.

### Quick Start

```bash
make              # Build the release binary
./hello           # Run the program
make test         # Run all tests
```

### Available Make Targets

- **`make`** or **`make build`** - Build the release binary with optimizations (`-O2`)
- **`make strict`** - Build with strict flags and `-Werror` (used by CI for validation)
- **`make debug`** - Build with debug symbols (`-g`)
- **`make clang`** - Build using the Clang compiler
- **`make unit-test`** - Compile and run unit tests
- **`make test`** - Run all tests (unit tests + validation script)
- **`make test-quiet`** - Run tests with minimal output (CI-friendly)
- **`make clean`** - Remove all build artifacts
- **`make help`** - Display help message with available targets

### Customizing the Build

You can override the compiler and other variables:

```bash
make CC=clang build           # Build with Clang
make CC=gcc-12 strict         # Build with a specific GCC version
```

### Requirements

- GCC or Clang compiler
- Bash (for test scripts)
- Standard C library

### Output Format

The program outputs:
```
Ciao, Mondo!
Exit code: 0
```

This format is validated by the test suite to ensure consistency.