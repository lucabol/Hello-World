# Hello World

[![CI](https://github.com/lucabol/Hello-World/actions/workflows/ci.yml/badge.svg)](https://github.com/lucabol/Hello-World/actions/workflows/ci.yml)

A simple C "Hello World" program.

## Quick Start

### Build and Run

**Using Make (recommended):**
```bash
# Build and run
make && ./hello
```

**Or using GCC directly:**
```bash
# Compile the program
gcc -o hello hello.c

# Run the program
./hello
```

**Output:**
```
Hello world!

Exit code: 0
```

The program exits with code 0 (success).

## Build Instructions

### Using Make

```bash
# Standard build
make

# Debug build
make debug

# Optimized build  
make optimized

# Build and test
make test

# Clean compiled files
make clean
```

### Using GCC Directly

### Standard Build
```bash
gcc -o hello hello.c
```

### Development Build (with warnings)
```bash
gcc -Wall -Wextra -o hello hello.c
```

### Debug Build
```bash
gcc -g -Wall -Wextra -o hello_debug hello.c
```

### Optimized Build
```bash
gcc -O2 -Wall -Wextra -o hello hello.c
```

## Validation

After making any changes, verify the program works correctly:

1. **Compile the program:**
   ```bash
   gcc -Wall -Wextra -o hello hello.c
   ```

2. **Run the program:**
   ```bash
   ./hello
   ```

3. **Expected output:**
   ```
   Hello world!

   Exit code: 0
   ```

4. **Check exit code:**
   ```bash
   echo $?
   # Should output: 0
   ```

## Alternative Compilers

The program also works with Clang:
```bash
clang -o hello hello.c
./hello
```

## Repository Structure

```
.
├── .github/
│   ├── workflows/
│   │   └── ci.yml     # GitHub Actions CI workflow
│   └── copilot-instructions.md
├── .gitignore         # Git ignore rules for compiled binaries
├── Makefile           # Build configuration
├── README.md          # This file
└── hello.c            # Main C source file
```

## Requirements

- GCC or Clang compiler
- Standard C library (stdio.h)

## Contributing

1. Fork the repository
2. Make your changes
3. Test that the program compiles and runs correctly
4. Submit a pull request

---

*Test repo for JediMaster*