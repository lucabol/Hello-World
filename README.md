# Hello World

Test repo for JediMaster

## Overview

This is a simple C "Hello World" program that demonstrates basic C programming concepts. The program outputs exactly "Hello world!" (without a trailing newline) and exits with code 0.

## Quick Start

**Just want to build and run? Here's the fastest way:**

```bash
make && ./hello
```
**Expected output:** `Hello world!`

### Step-by-step:
```bash
# Compile the program
make

# Run the program
./hello
```

Expected output: `Hello world!`

## Building the Program

### Prerequisites

- **GCC compiler** (version 13.3.0 or compatible)
- **GNU Make** (optional, for using make targets)
- **Clang compiler** (optional alternative)

### Basic Compilation

#### Using GCC (recommended)
```bash
# Simple compilation
gcc -o hello hello.c

# With warnings enabled (recommended for development)
gcc -Wall -Wextra -o hello hello.c

# Optimized build
gcc -O2 -Wall -Wextra -o hello hello.c
```

#### Using Clang (alternative)
```bash
# Basic Clang compilation
clang -o hello hello.c

# With warnings
clang -Wall -Wextra -o hello hello.c
```

### Using Make Targets

This project includes a comprehensive Makefile with multiple build targets:

```bash
# Default build
make

# Development build with warnings
make dev

# Strict build (CI-style with -Werror)
make strict

# Debug build
make debug

# Optimized build  
make optimized

# Build with Clang
make clang

# Show all available targets
make help
```

## Running the Program

After compilation, run the program:

```bash
# Run the compiled program
./hello
```

**Expected Output:** `Hello world!` (exactly 12 characters, no trailing newline)

**Expected Exit Code:** 0

## Testing and Validation

### Running Tests

The project includes a comprehensive validation script that verifies:
- Successful compilation with strict warning flags
- Correct program output format
- Proper exit code (0)
- No trailing newline in output

```bash
# Run full validation tests
make test

# Run tests with minimal output (CI-style)
make test-quiet

# Run validation script directly
./test/validate.sh

# Run validation script with quiet output
./test/validate.sh --quiet
```

### Manual Validation Steps

1. **Compile the program:**
   ```bash
   make strict
   ```

2. **Run the program:**
   ```bash
   ./hello
   ```

3. **Verify output:**
   - Should output exactly: `Hello world!`
   - Should NOT have a trailing newline
   - Should exit with code 0

4. **Check exit code:**
   ```bash
   echo $?
   # Should output: 0
   ```

## Build Configurations

### Development Build
For active development with helpful warnings:
```bash
make dev
# or
gcc -Wall -Wextra -o hello hello.c
```

### Production Build
Optimized for performance:
```bash
make optimized
# or  
gcc -O2 -Wall -Wextra -o hello hello.c
```

### Strict Build (CI-like)
Used for quality assurance and CI/CD:
```bash
make strict
# or
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -o hello hello.c
```

### Debug Build
For debugging with GDB:
```bash
make debug
# or
gcc -g -Wall -Wextra -o hello_debug hello.c
```

## Compiler Compatibility

### GCC (Primary)
- **Version:** 13.3.0 or compatible
- **Compilation time:** Very fast (typically under 1 second)
- **Status:** Fully supported and tested

### Clang (Alternative)
- **Version:** 18.1.3 or compatible  
- **Compilation time:** Fast (may take a few seconds)
- **Status:** Fully supported, produces identical output

Both compilers produce functionally identical executables.

## Cleaning Up

Remove compiled binaries:
```bash
# Using make
make clean

# Manual cleanup
rm -f hello hello_debug hello_optimized hello_clang *.exe *.out *.o
```

## Project Structure

```
.
├── .git/              # Git repository data
├── .gitignore         # Excludes compiled binaries
├── README.md          # This documentation
├── Makefile           # Build configuration
├── hello.c            # Main C source file (63 bytes)
└── test/
    └── validate.sh    # Validation script
```

## Troubleshooting

### Common Issues

#### "Command not found" errors
```bash
# Check if GCC is installed
which gcc
gcc --version

# Check if Make is installed  
which make
make --version
```

#### Permission denied when running `./hello`
```bash
# Make file executable
chmod +x hello

# Or recompile
make clean && make
```

#### "No such file" when running `./hello`
Ensure you've compiled the program first:
```bash
gcc -o hello hello.c
# or
make
```

#### Compilation warnings or errors
Use the strict build to see all potential issues:
```bash
make strict
```

### Build Verification

Verify your environment is set up correctly:
```bash
# Test complete workflow
make clean && make test
```

### Expected Timings

- **GCC compilation:** Very fast (typically under 1 second)
- **Clang compilation:** Fast (may take a few seconds)
- **Program execution:** Instantaneous
- **Full test suite:** Very fast (typically under 2 seconds)

## Development Workflow

### Recommended Steps

1. **Make changes** to `hello.c`
2. **Test compilation:**
   ```bash
   make strict
   ```
3. **Run validation:**
   ```bash
   make test
   ```
4. **Verify output manually:**
   ```bash
   ./hello && echo "Success"
   ```

### Best Practices

- Always use `make strict` to catch potential issues early
- Run `make test` after any changes to verify correctness
- Use `make clean` before final builds to ensure clean state
- Test with both GCC and Clang when making significant changes

## CI/CD Integration

The project is designed for easy CI/CD integration:

```bash
# Typical CI workflow
make test-quiet
```

This command:
1. Builds with strict compilation flags
2. Runs comprehensive validation
3. Provides minimal output suitable for CI logs
4. Exits with non-zero code on any failure

## File Management

- **Source files:** Only modify `hello.c` for functional changes
- **Compiled binaries:** Automatically excluded by `.gitignore`
- **Documentation:** Update this README when changing build processes

## Additional Resources

### Make Targets Reference

| Target | Description | Output Binary |
|--------|-------------|---------------|
| `all` (default) | Standard build | `hello` |
| `dev` | Development build with warnings | `hello` |
| `strict` | CI-style build with -Werror | `hello` |
| `debug` | Debug build with -g flag | `hello_debug` |
| `optimized` | Optimized build with -O2 | `hello_optimized` |
| `clang` | Build using Clang compiler | `hello_clang` |
| `test` | Run validation tests | (builds `hello`) |
| `test-quiet` | Run tests with minimal output | (builds `hello`) |
| `clean` | Remove all compiled binaries | (none) |
| `help` | Show available targets | (none) |

### Compiler Flags Reference

| Build Type | Flags Used |
|------------|------------|
| Basic | (none) |
| Development | `-Wall -Wextra` |
| Strict | `-Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99` |
| Debug | `-g -Wall -Wextra` |
| Optimized | `-O2 -Wall -Wextra` |

---

**Note:** This is a minimal C project with no external dependencies. All operations complete quickly and the build process is straightforward.