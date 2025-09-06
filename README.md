# Hello World
Test repo for JediMaster

## Building and Testing

### Quick Build
```bash
gcc -o hello hello.c
./hello
```

### Using Make
```bash
make          # Build the program
make test     # Run automated tests
make clean    # Clean build artifacts
make help     # Show all available targets
```

### Automated Testing
Run the comprehensive test suite:
```bash
./test_hello.sh
```

This tests the program with multiple compiler configurations (GCC and Clang) and various build flags to ensure consistent behavior across different builds.