# Hello World
Test repo for JediMaster

## Quick Start

Build and run the program using Make:

```bash
# Build optimized version (default)
make

# Run the program
./hello

# Other build targets
make dev      # Development build with warnings
make debug    # Debug build (creates hello_debug)
make strict   # Strict compilation with maximum warnings
make clang    # Build with clang compiler (creates hello_clang)
make clean    # Remove all compiled binaries
```

## Manual Compilation

You can also compile manually using gcc:

```bash
# Basic compilation
gcc -o hello hello.c

# Development build with warnings
gcc -Wall -Wextra -o hello hello.c

# Optimized build
gcc -O2 -Wall -Wextra -o hello hello.c
```