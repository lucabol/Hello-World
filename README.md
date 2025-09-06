# Hello World

A simple C program that prints "Hello world!" to demonstrate basic compilation and build options.

## Quick Start

```bash
# Compile the program
gcc -o hello hello.c

# Run the program
./hello
```

**Expected output:** `Hello world!`

## Compilation Instructions

This section provides detailed instructions for compiling the program with different build configurations.

### Basic Compilation

```bash
# Standard compilation
gcc -o hello hello.c

# Run the program
./hello
```

### Development Build (Recommended)

Compile with warnings enabled to catch potential issues:

```bash
# Compilation with warnings
gcc -Wall -Wextra -o hello hello.c

# Run the program
./hello
```

### Debug Build

Compile with debug symbols for debugging with tools like `gdb`:

```bash
# Debug build with symbols and warnings
gcc -g -Wall -Wextra -o hello_debug hello.c

# Run the debug version
./hello_debug
```

### Optimized Build

Compile with optimizations for better performance:

```bash
# Optimized build with warnings
gcc -O2 -Wall -Wextra -o hello_optimized hello.c

# Run the optimized version
./hello_optimized
```

### Strict Compilation

Compile with additional strict warnings for maximum code quality:

```bash
# Strict compilation with comprehensive warnings
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello_strict hello.c

# Run the strict build
./hello_strict
```

### Alternative Compiler (Clang)

You can also use Clang instead of GCC:

```bash
# Basic Clang compilation
clang -o hello_clang hello.c

# Clang with warnings
clang -Wall -Wextra -o hello_clang hello.c

# Run the Clang-compiled version
./hello_clang
```

## Build Verification

To verify your compilation was successful:

```bash
# Compile and run in one command
gcc -Wall -Wextra -o hello hello.c && ./hello

# Check the exit code (should be 0)
echo $?
```

## Cleaning Up

Remove compiled binaries:

```bash
# Remove all compiled files
rm -f hello hello_debug hello_optimized hello_strict hello_clang hello_warnings
```

## Requirements

- **GCC** (recommended) or **Clang** compiler
- Standard C library (stdio.h)
- No additional dependencies required

## Build Times

- **GCC compilation:** < 1 second
- **Clang compilation:** ~4 seconds
- All builds produce identical output

## Troubleshooting

### Common Issues

- **"Command not found" for gcc:** Ensure GCC is installed: `which gcc`
- **Permission denied for ./hello:** Make the file executable: `chmod +x hello`
- **"No such file" when running ./hello:** Compile first: `gcc -o hello hello.c`

### Verify Compiler Installation

```bash
# Check GCC availability and version
gcc --version

# Check Clang availability and version
clang --version
```