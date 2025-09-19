# Hello World

![Build Status](https://img.shields.io/badge/build-passing-brightgreen) ![Language](https://img.shields.io/badge/language-C-blue)

A simple "Hello World" program written in C. This repository contains a minimal C source file that demonstrates basic program structure and output.

## Quick Start

### Building and Running

```bash
# Compile the program
gcc -o hello hello.c

# Run the program
./hello
```

### Expected Output
```
Hello world!

Exit code: 0
```

## Supported Compilers

- **GCC** (recommended): `gcc -o hello hello.c`
- **Clang**: `clang -o hello hello.c`

Both compilers produce identical output. Compilation typically takes under 1 second with GCC.

## Development

### Build Options

```bash
# Basic build
gcc -o hello hello.c

# Development build with warnings (recommended)
gcc -Wall -Wextra -o hello hello.c

# Debug build
gcc -g -Wall -Wextra -o hello_debug hello.c

# Optimized build
gcc -O2 -Wall -Wextra -o hello hello.c

# Strict compilation
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello hello.c
```

### Quick Test

```bash
# Build and test in one command
gcc -Wall -Wextra -o hello hello.c && ./hello
```

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for development guidelines and contribution process.

## Repository Structure

```
.
├── README.md          # This file
├── hello.c            # Main C source file
├── CONTRIBUTING.md    # Contribution guidelines
└── .gitignore         # Git ignore rules for compiled binaries
```