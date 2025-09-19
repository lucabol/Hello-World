# Hello World
Test repo for JediMaster

## Building

This project includes a Makefile for convenient building with different configurations:

### Available targets:

- `make` or `make all` - Build optimized binary (default target)
- `make dev` - Build with development flags (-Wall -Wextra)
- `make debug` - Build debug version with symbols (-g -Wall -Wextra)
- `make strict` - Build with strict warning flags
- `make clean` - Remove all compiled binaries and temporary files

### Example usage:

```bash
# Build optimized version (default)
make

# Build for development
make dev

# Build debug version
make debug

# Build with strict warnings
make strict

# Clean all generated files
make clean
```

### Manual compilation:

You can also compile manually using gcc:

```bash
# Basic compilation
gcc -o hello hello.c

# Development build
gcc -Wall -Wextra -o hello hello.c

# Debug build
gcc -g -Wall -Wextra -o hello_debug hello.c

# Optimized build
gcc -O2 -Wall -Wextra -o hello hello.c

# Strict compilation
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello hello.c
```