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

### Unit Testing

The `hello.c` file supports unit testing through conditional compilation using the `UNIT_TEST` macro.

When compiled with `-DUNIT_TEST`, the `main()` function is excluded, allowing the code to be linked with a separate test runner that defines its own `main()`. This enables testing of the `get_greeting()` function in isolation.

To run unit tests:
```bash
make unit-test
```

The unit tests compile `hello.c` as a library (with `-c -DUNIT_TEST`) and link it with `test/test_hello.c`, which contains the test cases and test runner's `main()` function.
