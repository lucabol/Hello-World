# Hello-World

A simple C "Hello World" program demonstrating basic compilation and execution.

## Quick Start

### Compile and Run
```bash
gcc -o hello hello.c
./hello
```

This will output: `Hello world!`

## Detailed Compilation Instructions

### Prerequisites
- GCC compiler (recommended) or Clang
- Standard C library (stdio.h)

### Basic Compilation

#### Using GCC (Recommended)
```bash
gcc -o hello hello.c
```
- **Purpose**: Compiles `hello.c` into an executable named `hello`
- **Time**: < 1 second
- **Output**: Creates `hello` binary file

#### Using Clang (Alternative)
```bash
clang -o hello hello.c
```
- **Purpose**: Same as GCC but using Clang compiler
- **Time**: ~4 seconds
- **Output**: Identical functionality to GCC version

### Development Builds

#### With Warnings (Recommended for Development)
```bash
gcc -Wall -Wextra -o hello hello.c
```
- **-Wall**: Enables common warning messages
- **-Wextra**: Enables extra warning messages
- **Purpose**: Helps catch potential issues during development

#### Debug Build
```bash
gcc -g -Wall -Wextra -o hello_debug hello.c
```
- **-g**: Includes debugging information
- **Purpose**: Enables debugging with tools like GDB
- **Usage**: `./hello_debug` (same output, but debuggable)

#### Optimized Build
```bash
gcc -O2 -Wall -Wextra -o hello hello.c
```
- **-O2**: Enables level 2 optimization
- **Purpose**: Creates faster, more efficient executable
- **Trade-off**: Longer compile time, optimized runtime performance

#### Strict Compilation (Maximum Warnings)
```bash
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello hello.c
```
- **-Wpedantic**: Enforces strict ISO C compliance
- **-Wformat=2**: Enhanced format string checking
- **-Wconversion**: Warns about type conversions
- **-Wsign-conversion**: Warns about signed/unsigned conversions
- **Purpose**: Maximum code quality checking

### Compiler Flag Reference

| Flag | Purpose | When to Use |
|------|---------|-------------|
| `-o <name>` | Specify output file name | Always (otherwise creates `a.out`) |
| `-Wall` | Enable common warnings | Development and production |
| `-Wextra` | Enable additional warnings | Development and production |
| `-g` | Include debug information | Debugging |
| `-O2` | Optimize for speed | Production builds |
| `-Wpedantic` | Strict standard compliance | Code quality checking |

### Execution

After compilation, run the program:
```bash
./hello
```

**Expected Output**: `Hello world!`
**Exit Code**: 0 (success)

### Verification

Check that compilation was successful:
```bash
gcc -Wall -Wextra -o hello hello.c && ./hello && echo "Exit code: $?"
```

This command chain will:
1. Compile with warnings
2. Run the program
3. Display the exit code (should be 0)

## Common Tasks

### Clean Build
```bash
# Remove old binaries and recompile
rm -f hello && gcc -o hello hello.c
```

### Test Multiple Compilers
```bash
# Compare GCC and Clang output
gcc -o hello_gcc hello.c
clang -o hello_clang hello.c
./hello_gcc && ./hello_clang
```

### Check for Warnings
```bash
# Compile with maximum warning level
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello hello.c
```

## Troubleshooting

### Common Issues

#### "command not found: gcc"
**Solution**: Install GCC compiler
```bash
# Ubuntu/Debian
sudo apt update && sudo apt install gcc

# Check installation
which gcc && gcc --version
```

#### "Permission denied" when running ./hello
**Solution**: Ensure the file is executable
```bash
chmod +x hello
```

#### "No such file or directory" when running ./hello
**Solution**: Make sure you compiled first
```bash
gcc -o hello hello.c
ls -la hello  # Verify file exists
```

### Build Verification
```bash
# Complete verification workflow
which gcc && \
gcc -Wall -Wextra -o hello hello.c && \
./hello && \
echo "Build and execution successful!"
```

## File Structure

```
.
├── README.md          # This documentation
├── hello.c            # C source code
├── .gitignore         # Excludes compiled binaries
└── .git/              # Git repository data
```

**Note**: Compiled binaries (`hello`, `*.exe`, `*.out`) are excluded from version control via `.gitignore`.

## Technical Details

- **Language**: C (ISO C standard)
- **Dependencies**: Standard C library only
- **Output**: Single executable binary
- **Platform**: Cross-platform (Linux, macOS, Windows with appropriate compiler)

---

*Test repo for JediMaster*
