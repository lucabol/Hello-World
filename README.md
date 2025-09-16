# Hello World

A simple C "Hello World" program demonstrating basic compilation and execution.

## Overview

This repository contains a minimal C program that prints "Hello world!" to the console. It's designed to be a simple example for learning C compilation and execution basics.

## Prerequisites

- C compiler (GCC or Clang)
- Linux/Unix environment (or compatible system)

## Quick Start

```bash
# Compile the program
gcc -o hello hello.c

# Run the program
./hello
```

## Detailed Compilation Instructions

### Basic Compilation

The simplest way to compile the program:

```bash
gcc -o hello hello.c
```

This creates an executable named `hello` from the source file `hello.c`.

### Compilation with Warnings (Recommended)

For development, it's recommended to enable compiler warnings:

```bash
gcc -Wall -Wextra -o hello hello.c
```

- `-Wall`: Enables most warning messages
- `-Wextra`: Enables extra warning flags not covered by `-Wall`

### Alternative Compiler (Clang)

You can also use Clang instead of GCC:

```bash
clang -o hello hello.c
```

### Build Configurations

#### Debug Build
For debugging with tools like GDB:

```bash
gcc -g -Wall -Wextra -o hello_debug hello.c
```

#### Optimized Build
For production/performance:

```bash
gcc -O2 -Wall -Wextra -o hello hello.c
```

#### Strict Compilation
With maximum warnings and pedantic checks:

```bash
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello hello.c
```

## Running the Program

After compilation, execute the program:

```bash
./hello
```

### Expected Output

The program should output:
```
Hello world!
```

And exit with code 0 (success).

## Validation Steps

To verify everything works correctly:

1. **Compile the program:**
   ```bash
   gcc -Wall -Wextra -o hello hello.c
   ```

2. **Run the program:**
   ```bash
   ./hello
   ```

3. **Check the output:**
   - Should print: `Hello world!`
   - Should exit with code 0

4. **Verify exit code:**
   ```bash
   echo $?
   ```
   Should return `0`.

## Testing Different Configurations

### Test All Build Types
```bash
# Basic build
gcc -o hello hello.c && ./hello

# With warnings
gcc -Wall -Wextra -o hello hello.c && ./hello

# Debug build
gcc -g -Wall -Wextra -o hello_debug hello.c && ./hello_debug

# Optimized build
gcc -O2 -Wall -Wextra -o hello hello.c && ./hello

# Strict build
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello hello.c && ./hello

# Clang build
clang -o hello_clang hello.c && ./hello_clang
```

All builds should produce identical output.

## Troubleshooting

### Common Issues

**"Command not found: gcc"**
- Ensure GCC is installed: `sudo apt install gcc` (Ubuntu/Debian)
- Check if GCC is available: `which gcc`

**"Permission denied" when running ./hello**
- Make the file executable: `chmod +x hello`
- Or run with explicit path: `/path/to/hello`

**"No such file or directory" when running ./hello**
- Make sure you compiled the program first
- Check the current directory contains the executable: `ls -la hello`

### Verification Commands

```bash
# Check compiler availability
which gcc && gcc --version
which clang && clang --version

# Complete build and test
gcc -Wall -Wextra -o hello hello.c && ./hello && echo "Success: Exit code $?"
```

## Cleaning Up

To remove compiled binaries:

```bash
rm -f hello hello_debug hello_clang *.exe *.out
```

## File Structure

```
.
├── README.md          # This documentation
├── hello.c            # Main C source file
├── .gitignore         # Git ignore file (excludes compiled binaries)
└── .git/              # Git repository data
```

## Development Notes

- The program uses standard C library (`stdio.h`)
- No external dependencies required
- Compiled binaries are automatically excluded by `.gitignore`
- Both GCC and Clang compilers are supported and produce identical results

## License

Test repo for JediMaster