# Hello World

A simple C "Hello World" program with comprehensive testing and CI validation.

## Overview

This repository contains a minimal C program (`hello.c`) that prints "Hello world!" to standard output. The project includes validation scripts to ensure code quality and output correctness.

## Quick Start

### Prerequisites

- GCC compiler (or Clang)
- Bash shell (for running test scripts)

### Building

Build the program using GCC:

```bash
gcc -Wall -Wextra -o hello hello.c
```

Or with Clang:

```bash
clang -Wall -Wextra -o hello_clang hello.c
```

### Running

Execute the compiled program:

```bash
./hello
```

Expected output: `Hello world!` (without trailing newline)

## Testing

> **Note**: A Makefile with convenient test targets is planned (see issue #1). Until then, use the commands below.

### Validation Tests

Run the validation script to verify the program builds and outputs correctly:

```bash
./test/validate.sh
```

For CI-style quiet output:

```bash
./test/validate.sh --quiet
```

The validation script performs:
- Strict compilation with `-Wall -Wextra -Wpedantic -Werror`
- Output verification (must be exactly "Hello world!" with trailing newline)
- Exit code validation (must return 0)

### Unit Tests

Unit tests verify the internal functionality. To run them:

```bash
# Compile unit tests
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -I. -c -o hello_lib.o hello.c -DUNIT_TEST
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -I. -o test_hello_runner test/test_hello.c hello_lib.o

# Run tests
./test_hello_runner

# Clean up
rm -f hello_lib.o test_hello_runner
```

Or use the test runner script:

```bash
./test/run_unit_tests.sh
```

> **Future**: When the Makefile is added, you'll be able to use `make test` or `make unit-test` as convenient shortcuts.

## Reproducing CI Locally

The CI workflow performs these steps. You can reproduce failures locally:

### GCC Build and Test

```bash
# Build with warnings
gcc -Wall -Wextra -o hello hello.c

# Run validation (CI uses --quiet flag)
./test/validate.sh --quiet
```

### Clang Build and Test

```bash
# Build with Clang
clang -Wall -Wextra -o hello_clang hello.c

# Test the build
./hello_clang

# Run unit tests with Clang
clang -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -I. -c -o hello_lib.o hello.c -DUNIT_TEST
clang -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -I. -o test_hello_runner test/test_hello.c hello_lib.o
./test_hello_runner
rm -f hello_lib.o test_hello_runner
```

## Troubleshooting

### Common Issues

**Issue: CI fails with "make: *** No rule to make target 'unit-test'"**

**Solution**: The current CI configuration references `make unit-test`, but the Makefile is not yet in the repository (see issue #1). This is a known issue. To reproduce the unit tests locally, use:
```bash
./test/run_unit_tests.sh
```

**Issue: "No such file or directory" when running `make test`**

**Solution**: This repository does not currently have a Makefile. Use the direct GCC commands or test scripts shown in the Testing section above. A Makefile is planned for a future update.

**Issue: Validation script fails with output mismatch**

**Solution**: The validation script expects the program to output exactly `Hello world!` followed by a newline character. Ensure your `hello.c` uses `printf("Hello world!\n");` or equivalent. Note: The current `hello.c` in the repository does NOT include the trailing newline, which will cause validation to fail.

**Issue: Unit tests fail with "hello.h: No such file or directory"**

**Solution**: The unit tests expect a `hello.h` header file that defines the `get_greeting()` function interface. This file is not currently in the repository. The unit test infrastructure is prepared for a refactored version of the code.

**Issue: Colorized output in test logs**

**Solution**: To disable colors in test output:
- For validation script: Use `./test/validate.sh --quiet`
- For unit tests: Set environment variable `SIMPLE_TEST_NO_COLOR=1`
  ```bash
  SIMPLE_TEST_NO_COLOR=1 ./test/run_unit_tests.sh
  ```

### Reproducing CI Failures

The CI workflow is currently incomplete due to missing files. Here's how to reproduce what SHOULD work:

#### Current State (some steps will fail)

1. **Build with GCC**: `gcc -Wall -Wextra -o hello hello.c` ✅ Works
2. **Run unit tests**: `make unit-test` ❌ Fails (no Makefile)
3. **Run validation**: `./test/validate.sh --quiet` ❌ Fails (output mismatch)

#### Workarounds to Test Locally

```bash
# Build the program
gcc -Wall -Wextra -o hello hello.c

# Run the program
./hello

# Manually run validation (will fail until output is fixed)
./test/validate.sh

# Manually run unit tests (will fail until hello.h exists)
./test/run_unit_tests.sh
```

#### Expected After Issue #1 is Resolved

Once the Makefile and necessary refactoring are complete:

```bash
make test          # Run all tests
make unit-test     # Run unit tests only
make validate      # Run validation tests only
```

## Repository Structure

```
.
├── hello.c                 # Main C source file
├── test/
│   ├── validate.sh         # Validation test script
│   ├── run_unit_tests.sh   # Unit test runner script
│   ├── test_hello.c        # Unit test source
│   └── simple_test.h       # Lightweight testing framework
├── .github/
│   └── workflows/
│       └── ci.yml          # CI configuration
├── README.md               # This file
├── CONTRIBUTING.md         # Contribution guidelines
└── LICENSE                 # MIT License
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process for submitting pull requests.