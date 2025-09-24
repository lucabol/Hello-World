# Hello World

Test repo for JediMaster

A simple C "Hello World" program that demonstrates basic C programming concepts.

## Quick Start

### Using the Makefile (Recommended)

This repository includes a standardized Makefile for consistent builds and testing:

```bash
# Build optimized version (default)
make

# Run the program
make run

# Run all tests (strict compilation + execution)
make test

# Build debug version
make debug

# Build with Clang
make clang

# Clean build artifacts
make clean

# Show all available targets
make help
```

### Using Validation Scripts

Run the validation script to ensure the program builds and works correctly:

```bash
# Run validation (builds with strict flags and tests output)
./scripts/validate_output.sh ./hello test-build

# Run comprehensive validation tests
./scripts/test_validation.sh

# Alternative validation paths
./validate.sh
./test/validate.sh
```

### Manual Build Commands

You can also build manually if needed:

```bash
# Basic build
gcc -o hello hello.c

# Development build with warnings
gcc -Wall -Wextra -o hello hello.c

# Run the program
./hello
```

## Expected Output

When you run the program, it will produce the following exact output:

```
Hello world!
```

**Important:** The program intentionally outputs "Hello world!" **without a trailing newline** and exits with code 0. This behavior is validated by the test scripts to ensure exact byte-level output compliance.

## CI Verification

Our Continuous Integration (CI) system performs strict validation to ensure code quality and output consistency:

### Static Analysis
- **Syntax check:** `gcc -fsyntax-only -Wall -Wextra hello.c` validates code syntax before compilation
- **Compilation warnings:** All builds use `-Wall -Wextra` flags to catch potential issues

### Output Format Validation
- **Exact output verification:** CI uses `scripts/validate_output.sh` to capture program output and compare it byte-for-byte with the expected format
- **Cross-compiler consistency:** Validates that GCC, Clang, and strict builds all produce identical output
- **Exit code verification:** Ensures the program exits with code 0
- **Robust comparison:** Uses `printf` for precise expected output generation and `cmp` for reliable comparison

### Validation Script Testing
- **Comprehensive test suite:** CI runs `./scripts/test_validation.sh` to verify the validation infrastructure itself
- **Edge case coverage:** Tests handle wrong output, non-zero exit codes, trailing newlines, and stderr output

### Compiler Matrix
- **GCC build:** Standard compilation with warnings enabled
- **Clang build:** Alternative compiler validation for consistency
- **Strict build:** Uses `-Werror` with additional warning flags (`-Wpedantic`, `-Wformat=2`, `-Wconversion`, `-Wsign-conversion`)

The CI fails if:
- Static analysis detects syntax errors
- Program output differs from "Hello world!" (exact 12-byte match required, no trailing newline)
- Program exits with non-zero code
- Any compiler warnings are generated with strict flags
- Validation script tests fail

### Local Testing
You can run the same validation locally:
```bash
gcc -o hello hello.c
./scripts/validate_output.sh ./hello local-test

# Run all validation script tests
./scripts/test_validation.sh

# Or use the Makefile
make test
make validate-all
```

### Debugging Output Mismatches
When validation fails, the script provides detailed diagnostics including `xxd` hexadecimal output. In the byte-level comparison:
- Each line shows: `offset: hex_bytes ascii_representation`
- Look for differences in hex values (e.g., `0a` indicates a newline character)
- Byte count differences help identify extra/missing characters

**Why precise formatting matters:** The exact output format is part of the program specification. Any deviation could indicate code changes that break compatibility or introduce unintended behavior changes.
