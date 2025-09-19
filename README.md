# Hello World

A simple C "Hello World" program that demonstrates basic C programming concepts.

## Overview

This repository contains a minimal C program that prints a greeting message. It serves as a basic example for C compilation and execution.

## Quick Start

### Building the Program

```bash
gcc -o hello hello.c
```

### Running the Program

```bash
./hello
```

### Expected Output

When you run the program, it will produce the following exact output:

```
Hello world!
```

**Note:** The program outputs "Hello world!" without a trailing newline and exits with code 0.

## Validation

To validate the program works correctly:

1. **Compile the program:**
   ```bash
   gcc -Wall -Wextra -o hello hello.c
   ```

2. **Run the program:**
   ```bash
   ./hello
   ```

3. **Verify the output:** The program should print "Hello world!" exactly as shown above.

4. **Check exit code:**
   ```bash
   echo $?
   ```
   This should return `0`, indicating successful execution.

## Build Options

### Development Build (Recommended)
```bash
gcc -Wall -Wextra -o hello hello.c
```

### Optimized Build
```bash
gcc -O2 -Wall -Wextra -o hello hello.c
```

### Debug Build
```bash
gcc -g -Wall -Wextra -o hello_debug hello.c
```

### Strict Compilation
```bash
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello hello.c
```

### Alternative Compiler (Clang)
```bash
clang -o hello hello.c
```

## Repository Structure

```
.
├── .git/              # Git repository data
├── .gitignore         # Excludes compiled binaries
├── README.md          # This file
├── CONTRIBUTING.md    # Contribution guidelines
└── hello.c            # Main C source file
```

## Requirements

- GCC or Clang compiler
- Standard C library (stdio.h)

No additional dependencies are required.

## Contributing

Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on how to contribute to this project.