# Hello World

A simple C program that demonstrates build validation and continuous integration workflows.

## Overview

This repository contains a minimal "Hello World" program in C, along with comprehensive testing and validation infrastructure. It serves as a template for understanding CI/CD workflows, automated testing, and code quality validation.

## Building the Program

### Basic Build

```bash
gcc -o hello hello.c
```

### Build with Warnings

```bash
gcc -Wall -Wextra -o hello hello.c
```

### Strict Build (CI-style)

```bash
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -o hello hello.c
```

## Running the Program

```bash
./hello
```

Expected output: `Hello world!` (without trailing newline, 12 bytes total)

## Testing and Validation

### Running the Validation Script

The repository includes a comprehensive validation script that performs multiple checks:

```bash
bash test/validate.sh
```

For CI-style minimal output:

```bash
bash test/validate.sh --quiet
```

### What the Validation Script Tests

The validation script ([`test/validate.sh`](test/validate.sh)) performs the following checks:

1. **Strict Compilation**: Compiles with strict GCC flags including:
   - `-Wall -Wextra -Wpedantic`: Enable all warnings
   - `-Wformat=2 -Wconversion -Wsign-conversion`: Additional safety checks
   - `-Werror`: Treat warnings as errors
   - `-std=c99`: Enforce C99 standard

2. **Binary Creation**: Verifies that the compilation produces an executable

3. **Program Execution**: Runs the program and captures its output and exit code

4. **Exit Code Validation**: Ensures the program exits with code 0 (success)

5. **Output Format Validation**: Verifies the output matches expected format exactly using byte-level comparison

6. **Trailing Newline Check**: Confirms there is no trailing newline in the output

### Understanding Validation Output

In normal mode, the script provides detailed colored output with:
- ✓ Green checkmarks for passed tests
- ✗ Red X marks for failed tests
- ℹ Yellow info messages for progress updates

In quiet mode (`--quiet`), output is minimal for CI environments.

## Continuous Integration (CI) Workflow

This repository uses GitHub Actions for automated testing. The CI workflow is defined in `.github/workflows/ci.yml`.

### CI Jobs

The CI pipeline includes multiple jobs that run in parallel:

#### 1. Build and Test with GCC (`build-and-test-gcc`)

This is the primary validation job:

```yaml
steps:
  - name: Checkout code
    uses: actions/checkout@v4
  - name: Build with GCC (with warnings)
    run: gcc -Wall -Wextra -o hello hello.c
  - name: Run validation tests
    run: bash test/validate.sh --quiet
```

**Purpose:** Ensures the code compiles cleanly with warnings enabled and passes all validation checks.

**Key Features:**
- Uses GCC with warning flags to catch potential issues
- Runs the complete validation script in quiet mode
- Fails the build if any validation check fails

#### 2. Build and Test with Clang (`build-and-test-clang`)

Validates compiler compatibility:

```yaml
steps:
  - name: Checkout code
    uses: actions/checkout@v4
  - name: Build with Clang
    run: clang -Wall -Wextra -o hello_clang hello.c
  - name: Test Clang build
    run: ./hello_clang
```

**Purpose:** Confirms the code works with an alternative compiler (Clang).

**Key Features:**
- Ensures code is portable across different compilers
- Uses same warning flags as GCC build
- Verifies output is consistent

#### 3. Collaborative Editor Tests (`collaborative-editor-tests`)

Tests advanced features if present:

```yaml
steps:
  - name: Checkout code
    uses: actions/checkout@v4
  - name: Setup Node.js
    uses: actions/setup-node@v4
  - name: Install dependencies
    run: npm ci
  - name: Run collaborative editor tests
    run: ./test_collab.sh
  - name: Run OT correctness tests
    run: node test-ot.js
  - name: Test backward compatibility
    run: make test
  - name: Test server startup (smoke test)
    run: |
      node collab_server.js &
      # Health check with timeout
```

**Purpose:** Validates additional functionality related to collaborative editing features.

**Key Features:**
- Runs only if additional features are present in the repository
- Includes operational transformation (OT) tests
- Verifies backward compatibility with original functionality
- Tests server startup and health endpoints

#### 4. Security Tests (`security-tests`)

Optional security validation:

```yaml
steps:
  - name: Checkout code
    uses: actions/checkout@v4
  - name: Setup Node.js
    uses: actions/setup-node@v4
  - name: Install dependencies
    run: npm ci
  - name: Run comprehensive security tests
    run: node test-security.js
```

**Purpose:** Performs security-focused testing (continues on error for faster CI).

**Key Features:**
- Runs security-specific test suite
- Does not block other tests if it fails (continue-on-error)
- Has a 10-minute timeout to prevent hanging

### How CI is Triggered

- **On Push:** Runs when code is pushed to the `main` branch
- **On Pull Request:** Runs when a PR targets the `main` branch

### Reproducing CI Failures Locally

If CI fails, reproduce the failure locally:

1. **Run the exact validation command:**
   ```bash
   bash test/validate.sh --quiet
   ```

2. **Check strict compilation:**
   ```bash
   gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -o hello_strict hello.c
   ```

3. **Test with Clang:**
   ```bash
   clang -Wall -Wextra -o hello_clang hello.c
   ./hello_clang
   ```

4. **Check output manually:**
   ```bash
   ./hello | hexdump -C
   ```

   Expected hex output (12 bytes total):
   ```
   00000000  48 65 6c 6c 6f 20 77 6f  72 6c 64 21              |Hello world!|
   0000000c
   ```

5. **Verify exit code:**
   ```bash
   ./hello
   echo $?  # Should print 0
   ```

## Troubleshooting

### Common Issues

#### Compilation Fails with Warnings

If you see compilation warnings:
- Review the specific warning message
- Fix the code issue (unused variables, type mismatches, etc.)
- Test again with strict flags: `gcc -Wall -Wextra -Werror -o hello hello.c`

#### Output Format Mismatch

If validation fails due to output mismatch:
- Check that your program outputs exactly `Hello world!` (or expected text)
- Ensure there is NO trailing newline: use `printf` without `\n`
- Verify with hex dump: `./hello | hexdump -C`

#### Validation Script Fails

If the validation script fails:
- Run without `--quiet` flag for detailed output: `bash test/validate.sh`
- Check which specific test failed (compilation, exit code, output format)
- Review the error messages and hex dumps for exact differences

### Validation Script Details

The validation script uses several techniques to ensure accuracy:

- **Exit on Error**: Uses `set -e` to exit immediately on any command failure
- **Pipeline Failures**: Uses `set -o pipefail` to catch errors in command pipelines
- **Unset Variables**: Uses `set -u` to catch undefined variable usage
- **Output Preservation**: Uses sentinel technique to preserve exact output including/excluding newlines
- **Hex Comparison**: Uses `hexdump` to show exact byte-level differences in output

### Checking Program Output Format

To manually verify program output format:

```bash
# Check visible output
./hello

# Check with explicit newline indicator
./hello; echo ""

# Check byte-level output
./hello | hexdump -C

# Check output length
./hello | wc -c  # Should be 12 for "Hello world!"
```

## Development Workflow

### Making Changes

1. **Modify code:** Edit `hello.c`
2. **Test locally:** Run `bash test/validate.sh`
3. **Fix issues:** Address any validation failures
4. **Commit:** Push changes to trigger CI

### Before Committing

Always run the validation script to catch issues early:

```bash
bash test/validate.sh
```

Ensure all checks pass before pushing.

## Repository Structure

```
.
├── .github/
│   ├── workflows/
│   │   └── ci.yml              # CI/CD configuration
│   └── copilot-instructions.md # Development guidelines
├── test/
│   └── validate.sh             # Validation script
├── hello.c                     # Main C source file
├── README.md                   # This file
└── .gitignore                  # Git ignore rules
```

## Requirements

- **GCC**: For compilation (tested with GCC 13.3.0 on Ubuntu 24.04)
- **Clang**: Optional, for compiler compatibility testing (tested with Clang 18.1.3)
- **Bash**: For running validation scripts
- **OS**: Tested on Ubuntu 24.04 LTS (Linux x86_64)

### Verification Environment

The commands and examples in this README were verified in the following environment:
- **OS**: Ubuntu 24.04 LTS (6.11.0-1018-azure kernel)
- **GCC**: 13.3.0 (Ubuntu 13.3.0-6ubuntu2~24.04)
- **Clang**: 18.1.3 (Ubuntu clang version 18.1.3)
- **Bash**: 5.2.21

## Additional Resources

- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [GCC Warning Options](https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html)
- [C99 Standard Reference](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf)