# Hello World

A simple C "Hello World" program demonstrating basic compilation, testing, and validation workflows.

## Quick Start

```bash
# Compile the program
gcc -o hello hello.c

# Run the program
./hello
```

## Table of Contents

- [Prerequisites](#prerequisites)
- [Building the Program](#building-the-program)
- [Testing and Validation](#testing-and-validation)
- [Development Workflow](#development-workflow)
- [CI/CD Pipeline](#cicd-pipeline)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)

## Prerequisites

- **GCC Compiler**: Version 13.3.0 or compatible
- **Optional**: Clang compiler for alternative builds
- **Bash**: For running validation scripts

### Checking Prerequisites

```bash
# Check GCC availability and version
gcc --version

# Check Clang availability (optional)
clang --version

# Verify bash
bash --version
```

## Building the Program

### Basic Compilation

The simplest way to build the program:

```bash
gcc -o hello hello.c
```

### Recommended Development Build

For development, use warning flags to catch potential issues:

```bash
gcc -Wall -Wextra -o hello hello.c
```

### Production Build

For optimized production builds:

```bash
gcc -O2 -Wall -Wextra -o hello hello.c
```

### Strict Compilation (CI-Style)

This matches the CI validation and treats warnings as errors:

```bash
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -o hello hello.c
```

### Alternative Compiler (Clang)

You can also build with Clang:

```bash
clang -Wall -Wextra -o hello_clang hello.c
```

### Debug Build

For debugging with GDB:

```bash
gcc -g -Wall -Wextra -o hello_debug hello.c
```

## Testing and Validation

### Running the Program

After compilation, execute the program:

```bash
./hello
```

**Expected Output**: `Hello world!` (no trailing newline)
**Expected Exit Code**: 0

> **Note**: There is currently a known issue with the validation script (`test/validate.sh`) that expects additional output beyond what the program actually produces. The program correctly outputs only "Hello world!" but the validation script expects additional text. This is a validation script issue, not a program issue.

### Validation Script

The repository includes a comprehensive validation script that tests compilation and output:

```bash
# Run full validation (with colored output)
./test/validate.sh

# Run in quiet mode (CI-style)
./test/validate.sh --quiet
```

> **Known Issue**: The validation script currently has a bug where it expects additional output ("Exit code: 0") that the program doesn't produce. The program correctly outputs only "Hello world!" but the validation expects more. This is being tracked as a validation script issue.

### Manual Validation Steps

To manually verify the program works correctly:

1. **Compile with strict flags**:
   ```bash
   gcc -Wall -Wextra -Wpedantic -Werror -o hello hello.c
   ```

2. **Run the program**:
   ```bash
   ./hello
   ```

3. **Verify output**: Should output exactly `Hello world!` with no trailing newline

4. **Check exit code**:
   ```bash
   echo $?
   # Should output: 0
   ```

### What the Validation Script Tests

The validation script (`test/validate.sh`) performs the following checks:

- **Strict Compilation**: Compiles with `-Wall -Wextra -Wpedantic -Werror`
- **Output Verification**: Ensures exact output matches `Hello world!`
- **Exit Code Check**: Verifies the program exits with code 0
- **Trailing Newline Verification**: Confirms proper output format
- **Hex Dump Analysis**: Provides byte-level output verification

## Development Workflow

### Recommended Development Process

1. **Make Changes**: Edit `hello.c` as needed
2. **Compile with Warnings**: `gcc -Wall -Wextra -o hello hello.c`
3. **Test Execution**: `./hello`
4. **Run Validation**: `./test/validate.sh`
5. **Commit Changes**: If validation passes

### Build Targets

Although there's no visible Makefile, the following make commands work:

```bash
# Run tests (equivalent to ./test/validate.sh --quiet)
make test

# Default build
make
```

### File Structure

```
.
├── hello.c           # Main C source file
├── README.md         # This documentation
├── .gitignore        # Git ignore rules for binaries
├── test/
│   └── validate.sh   # Validation script
└── .github/
    └── workflows/
        └── ci.yml    # CI/CD configuration
```

### Ignored Files

The following compiled files are automatically ignored by Git:
- `hello`, `hello_debug`, `hello_optimized`, `hello_strict`, `hello_clang`
- `*.exe`, `*.out`, `*.o`
- Debug files and temporary artifacts

## CI/CD Pipeline

The repository uses GitHub Actions for continuous integration:

### CI Jobs

1. **GCC Build and Test**:
   - Compiles with `gcc -Wall -Wextra`
   - Runs validation script in quiet mode

2. **Clang Build and Test**:
   - Compiles with `clang -Wall -Wextra`
   - Tests execution

3. **Additional Tests**:
   - Node.js-based collaborative editor tests
   - Security tests
   - Server startup smoke tests

### Reproducing CI Locally

To reproduce the exact CI validation locally:

```bash
# Build as CI does
gcc -Wall -Wextra -o hello hello.c

# Run validation as CI does
bash test/validate.sh --quiet
```

## Troubleshooting

### Common Issues

#### "Command not found: gcc"
Ensure GCC is installed:
```bash
# Ubuntu/Debian
sudo apt update && sudo apt install gcc

# Check installation
which gcc
```

#### "Permission denied: ./hello"
Make the binary executable:
```bash
chmod +x hello
```

#### "No such file or directory: ./hello"
Ensure you've compiled the program first:
```bash
gcc -o hello hello.c
```

#### Compilation Warnings
Use strict compilation to catch all warnings:
```bash
gcc -Wall -Wextra -Wpedantic -Werror -o hello hello.c
```

#### Validation Script Fails
Common causes:
- Output doesn't match exactly `Hello world!`
- Program exits with non-zero code
- Trailing newline issues

**Current Known Issue**: The validation script (`test/validate.sh`) currently expects output that includes "Exit code: 0" but the program only outputs "Hello world!". This is a bug in the validation script, not the program. The program behavior is correct.

**Manual Validation Alternative**: Instead of relying on the validation script, manually verify:
```bash
# Compile
gcc -Wall -Wextra -Wpedantic -Werror -o hello hello.c

# Run and check output manually
./hello
# Should output: Hello world!

# Check exit code
echo $?
# Should output: 0
```

### Build Verification

Verify your environment is set up correctly:

```bash
# Quick environment check
gcc --version && echo "GCC: OK" || echo "GCC: NOT FOUND"

# Test compilation and execution
gcc -o hello hello.c && ./hello && echo "Build successful: OK" || echo "Build failed"

# Clean up test
rm -f hello
```

### Expected Timings

- **Compilation**: Under 1 second with GCC, ~4 seconds with Clang
- **Execution**: Instantaneous
- **Validation**: Under 2 seconds total

## Contributing

### Before Making Changes

1. **Test Current State**: Run `./test/validate.sh` to ensure baseline works
2. **Make Minimal Changes**: Focus on specific improvements
3. **Test Frequently**: Run validation after each change
4. **Follow Standards**: Use consistent C coding style

### Submission Process

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Ensure all tests pass: `./test/validate.sh`
5. Submit a pull request

### Code Style

- Follow standard C conventions
- Keep the program simple and focused
- Maintain exact output format: `Hello world!`
- Ensure program exits with code 0

---

**Note**: This is a minimal C project designed for demonstration and testing purposes. All operations should complete quickly (under 5 seconds).