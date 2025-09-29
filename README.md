# Hello World
Test repo for JediMaster

## Build and Test Commands

This repository includes a comprehensive Makefile with the following targets:

### Build Targets
- `make` or `make all` - Standard build with warnings
- `make build` - Alias for standard build 
- `make strict` - Build with strict flags and `-Werror` (for CI validation)
- `make debug` - Build with debug symbols (`-g`) → creates `hello_debug`
- `make clang` - Build using clang compiler → creates `hello_clang`
- `make optimized` - Build with `-O2` optimization → creates `hello_opt`

### Test Targets
- `make test` - Run validation script with full output
- `make test-quiet` - Run validation with minimal output (CI-friendly)

### Utility Targets
- `make clean` - Remove all build artifacts
- `make help` - Show available targets

### Quick Examples
```bash
# Basic workflow
make && ./hello

# Development with debug symbols
make debug && ./hello_debug

# Test everything
make test

# Clean build for release
make clean && make optimized && ./hello_opt

# CI-style validation
make test-quiet
```