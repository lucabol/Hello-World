# Hello World
Test repo for JediMaster

## Building and Running

### Standard Build
```bash
make              # Build with standard flags
./hello          # Run the program
```

### Development Builds
```bash
make debug       # Build with debug symbols
make strict      # Build with strict warnings (CI mode)
make clang       # Build with Clang compiler
```

### Sanitizer Builds (Runtime Error Detection)
This project supports building with various sanitizers to detect runtime errors:

```bash
make asan        # Build with AddressSanitizer (memory errors)
make ubsan       # Build with UndefinedBehaviorSanitizer (undefined behavior)
make msan        # Build with MemorySanitizer (uninitialized memory)
make test-sanitizers  # Build and test all sanitizers
```

**What do sanitizers detect?**
- **AddressSanitizer (ASan)**: Use-after-free, buffer overflows, memory leaks
- **UndefinedBehaviorSanitizer (UBSan)**: Integer overflow, null pointer dereference, signed integer overflow
- **MemorySanitizer (MSan)**: Uninitialized memory reads (requires Clang and instrumented libraries)

### Testing
```bash
make test        # Run validation tests
./test/validate.sh       # Direct validation script
./test/test_sanitizers.sh # Test all sanitizer builds
```

### Cleaning
```bash
make clean       # Remove all build artifacts
```

## Requirements
- GCC or Clang compiler
- Make (optional, but recommended)
- Standard C library

## CI/CD
The project includes GitHub Actions workflows that:
- Build with GCC and Clang
- Run validation tests
- Build and test with all sanitizers (ASan, UBSan, MSan)
- Verify output correctness