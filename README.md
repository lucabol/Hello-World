# Hello World

Test repo for JediMaster

## Building and Testing

### Dependencies

- **C Compiler**: gcc or clang
- **Build Tool**: make
- **Shell**: bash (for test scripts)
- **Utilities**: hexdump or od (for byte-level output validation)

### Normal Build

Build the program with developer-friendly flags (warnings but no -Werror):
```bash
make
./hello
```

Expected output:
```
Ciao, Mondo!
Exit code: 0
```

### Unit Testing

The `hello.c` file supports unit testing through conditional compilation using the `UNIT_TEST` macro.

When compiled with `-DUNIT_TEST`, the `main()` function is excluded, allowing the code to be linked with a separate test runner that defines its own `main()`. This enables testing of the `get_greeting()` function in isolation.

To run unit tests:
```bash
make unit-test
```

The Makefile handles the complete build process:
1. Compiles `hello.c` as a library with `-c -DUNIT_TEST` to create `hello_lib.o`
2. Links `hello_lib.o` with `test/test_hello.c` to create the test runner
3. Executes the test runner

You can also run tests manually:
```bash
# Build the test library
gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -I. -c -o hello_lib.o hello.c -DUNIT_TEST

# Build and link the test runner
gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -I. -o test_hello_runner test/test_hello.c hello_lib.o

# Run the tests
./test_hello_runner
```

### Validation Testing

The validation script performs byte-level verification of the program output:

```bash
# Verbose mode (shows detailed results)
make test

# Quiet mode (minimal output for CI)
make test-quiet

# Direct script invocation
./test/validate.sh
./test/validate.sh --quiet
```

The validation checks:
- Strict compilation with all warnings as errors
- Program exits with code 0
- Exact output format (including trailing newlines)
- Byte-level output verification using hexdump

### Available Make Targets

- `make` or `make all` - Build with developer-friendly flags (no -Werror)
- `make dev` - Same as above (explicit developer build)
- `make strict` - Build with strict flags (-Werror) for CI
- `make debug` - Build with debug symbols (-g)
- `make clang` - Build using Clang compiler
- `make unit-test` - Build and run unit tests
- `make test` - Run validation tests (verbose)
- `make test-quiet` - Run validation tests (quiet mode for CI)
- `make clean` - Remove all build artifacts

### For CI/CD

The strict compilation flags include:
- `-Wall -Wextra -Wpedantic -Werror -std=c99`
- `-Wformat=2 -Wconversion -Wsign-conversion`

CI should use:
```bash
make unit-test  # For unit testing
make test-quiet # For validation testing
```