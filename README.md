# Hello World

Test repo for JediMaster

## Building

This repository includes a Makefile with standardized build targets:

```bash
# Build the program (default target)
make

# Build with strict compilation flags (matches CI validation)
make strict

# Build using clang compiler
make clang

# Build with debug symbols
make debug
```

## Testing

```bash
# Run unit tests
make unit-test

# Run validation tests
make test

# Run validation tests in quiet mode (CI)
make test-quiet
```

## Cleaning

```bash
# Remove all compiled binaries and artifacts
make clean
```

## Help

```bash
# Display available make targets
make help
```