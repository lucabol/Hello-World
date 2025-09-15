# Hello World
Test repo for JediMaster

## Quick Start

Build and run the program using the consolidated Makefile:

```bash
# Build and test (recommended for development)
make test

# Or build manually then run
make dev
./hello
```

## Build Options

All compilation commands have been consolidated into a single Makefile with multiple targets:

- `make` or `make all` - Basic build
- `make dev` - Development build with warnings (recommended)
- `make debug` - Debug build with symbols
- `make optimized` - Optimized build for production
- `make strict` - Strict compilation with all warnings
- `make clang` - Build using Clang compiler
- `make test` - Build and run the program
- `make clean` - Remove compiled files
- `make help` - Show all available targets

## Manual Compilation

If you prefer manual compilation:
```bash
gcc -Wall -Wextra -o hello hello.c
./hello
```