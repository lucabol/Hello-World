# Hello World

A simple C program that demonstrates build validation and continuous integration workflows.

## Overview

This repository contains a minimal "Hello World" C program with comprehensive testing and validation infrastructure. The project showcases:
- Strict compilation with multiple warning flags
- Automated validation scripts
- CI/CD integration with GitHub Actions
- Multiple compiler support (GCC and Clang)

## Building the Program

### Basic Build

```bash
gcc -o hello hello.c
```

### Build with Warnings (Recommended)

```bash
gcc -Wall -Wextra -o hello hello.c
```

### Strict Build (CI-Quality)

```bash
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -o hello hello.c
```

### Alternative Compiler (Clang)

```bash
clang -Wall -Wextra -o hello_clang hello.c
```

## Running the Program

After building, execute the program:

```bash
./hello
```

**Expected Output:** `Hello world!` (without trailing newline)

**Expected Exit Code:** 0

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

The validation script (`test/validate.sh`) performs the following checks:

1. **Strict Compilation Test**
   - Compiles the program with strict warning flags
   - Flags used: `-Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99`
   - Ensures code quality and standard compliance
   - Build must succeed without warnings or errors

2. **Binary Creation Verification**
   - Confirms that the compilation produces an executable binary
   - Checks file existence after compilation

3. **Program Execution Test**
   - Runs the compiled program
   - Captures both standard output and standard error
   - Records the exit code for verification

4. **Exit Code Validation**
   - Verifies the program exits with code 0 (success)
   - Any non-zero exit code indicates failure

5. **Output Format Verification**
   - Checks that the program output exactly matches the expected string
   - Currently expects: `Ciao, Mondo!`
   - Performs byte-level comparison
   - Shows hexadecimal dumps on mismatch for debugging

6. **Trailing Newline Check**
   - Verifies that the output does NOT end with a newline character
   - The program should output text without adding `\n` at the end
   - This ensures precise output control

### Understanding Validation Output

**Success Output (Normal Mode):**
```
ℹ Starting validation of Hello World program...
ℹ Building with strict compilation flags...
✓ Strict compilation passed with GCC
ℹ Running program and capturing output...
✓ Program exited with correct exit code (0)
✓ Output format is correct
✓ No trailing newline confirmed (as expected)
✓ All validation checks passed!

ℹ Summary:
  - Strict compilation: PASSED
  - Exit code (0): PASSED
  - Output format: PASSED
  - No trailing newline: PASSED
```

**Success Output (Quiet Mode):**
```
Validation: All tests PASSED
```

**Failure Output Example:**
```
✗ Output mismatch!
Expected: 'Ciao, Mondo!'
Actual:   'Hello world!'
Expected length: 12
Actual length:   12
Expected (hex): 00000000  43 69 61 6f 2c 20 4d 6f  6e 64 6f 21
Actual (hex):   00000000  48 65 6c 6c 6f 20 77 6f  72 6c 64 21
```

## Continuous Integration (CI) Workflow

This repository uses GitHub Actions for automated testing. The CI workflow is defined in `.github/workflows/ci.yml`.

### CI Jobs

The CI pipeline includes multiple jobs that run in parallel:

#### 1. Build and Test with GCC (`build-and-test-gcc`)

This is the primary validation job:

```yaml
- Checkout code
- Build with GCC: gcc -Wall -Wextra -o hello hello.c
- Run validation tests: bash test/validate.sh --quiet
```

**Purpose:** Ensures the code compiles cleanly with warnings enabled and passes all validation checks.

#### 2. Build and Test with Clang (`build-and-test-clang`)

Validates compiler compatibility:

```yaml
- Checkout code
- Build with Clang: clang -Wall -Wextra -o hello_clang hello.c
- Test Clang build: ./hello_clang
```

**Purpose:** Confirms the code works with an alternative compiler (Clang).

#### 3. Collaborative Editor Tests (`collaborative-editor-tests`)

Tests advanced features if present:

```yaml
- Checkout code
- Setup Node.js
- Install dependencies: npm ci
- Run collaborative editor tests
- Run OT correctness tests
- Test backward compatibility: make test
- Test server startup (smoke test)
```

**Purpose:** Validates additional functionality related to collaborative editing features.

#### 4. Security Tests (`security-tests`)

Optional security validation:

```yaml
- Checkout code
- Setup Node.js
- Install dependencies: npm ci
- Run comprehensive security tests: node test-security.js
```

**Purpose:** Performs security-focused testing (continues on error for faster CI).

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

## Troubleshooting

### Common Issues

#### Compilation Fails with Warnings

**Symptom:** Build fails with `-Werror` flag
```
error: unused variable 'x' [-Werror=unused-variable]
```

**Solution:** Fix all warnings. The strict build treats warnings as errors to ensure code quality.

#### Output Mismatch

**Symptom:** Validation fails with "Output mismatch!"
```
✗ Output mismatch!
Expected: 'Ciao, Mondo!'
Actual:   'Hello world!'
```

**Solution:** 
- Check the `EXPECTED_OUTPUT` variable in `test/validate.sh` (line 33)
- Ensure `hello.c` outputs the exact expected string
- Note: The output should NOT include a trailing newline

#### Wrong Exit Code

**Symptom:** Program exits with non-zero code
```
✗ Program exited with code 1, expected 0
```

**Solution:** Ensure `main()` returns 0 or implicitly returns success.

#### Hexdump Debugging

Use hexdump to see exact byte-level output:

```bash
./hello | hexdump -C
```

This shows:
- Exact characters in hexadecimal
- ASCII representation
- Presence/absence of newlines and special characters

### Validation Script Details

The validation script uses several techniques to ensure accurate testing:

1. **Sentinel Pattern:** Adds 'x' after output to preserve trailing whitespace
2. **Color-Coded Output:** Green (✓) for success, Red (✗) for errors, Yellow (ℹ) for info
3. **Quiet Mode:** Minimal output for CI environments (use `--quiet` flag)
4. **Cleanup Trap:** Automatically removes temporary `hello_strict` binary on exit
5. **Strict Error Handling:** 
   - `set -e`: Exit on any error
   - `set -u`: Exit on unset variables
   - `set -o pipefail`: Exit on pipeline failures

### Checking Program Output Format

To verify output format manually:

```bash
# Check output string
./hello && echo ""

# Check with hexdump for exact bytes
./hello | hexdump -C

# Check exit code
./hello
echo $?
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

- **GCC:** Version 4.9 or higher (for C99 support)
- **Bash:** Version 4.0 or higher (for validation script)
- **Optional:** Clang compiler for alternative builds

## Additional Resources

- **C99 Standard:** The code follows the C99 standard (`-std=c99`)
- **GCC Warning Options:** See [GCC documentation](https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html)
- **GitHub Actions:** See [GitHub Actions documentation](https://docs.github.com/en/actions)