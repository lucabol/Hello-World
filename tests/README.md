# Code Metrics Analyzer - Test Suite

<!--
SPDX-License-Identifier: MIT
-->

This directory contains the automated test suite for the Code Metrics Analyzer.

## Test Structure

The test suite consists of:
- **`run_tests.sh`**: Main test runner script that executes all tests
- **`fixtures/`**: Directory containing test input files (C source code samples)
- **`fixtures/*.expected`**: Expected output files for corresponding fixtures

## Test Fixtures

Each test fixture is designed to test specific edge cases and parsing scenarios:

### 1. `simple.c`
**Purpose**: Basic functionality test  
**Tests**: Simple C program with standard structure  
**Expected metrics**:
- Total lines: 7
- Code lines: 5
- Blank lines: 1
- Comment lines: 1
- Function count: 1 (main)
- Include count: 1 (stdio.h)

### 2. `multiline_comment.c`
**Purpose**: Multi-line block comment handling  
**Tests**:
- Block comments (`/* ... */`) spanning multiple lines
- State tracking across lines
- Multiple block comment blocks in the same file

**Expected metrics**:
- Total lines: 11
- Code lines: 4
- Blank lines: 1
- Comment lines: 6
- Function count: 1
- Include count: 1

### 3. `inline_comments.c`
**Purpose**: Inline comment detection  
**Tests**:
- Lines with code followed by `//` comments
- Lines with code followed by `/*` inline block comments
- Proper classification (code lines, not comment lines)

**Expected metrics**:
- Total lines: 7
- Code lines: 6
- Blank lines: 1
- Comment lines: 0 (inline comments don't count as comment-only lines)
- Function count: 1
- Include count: 1

### 4. `strings_with_comments.c`
**Purpose**: String literal parsing  
**Tests**:
- Comment markers (`//`, `/*`) inside string literals should be ignored
- Character literals containing `/` should be ignored
- Escape sequences in strings (`\"`, `\'`)

**Coverage**:
- Strings containing "// not a comment"
- Strings containing "/* not a comment */"
- Character literal: `'/'`
- Actual inline comments for comparison

### 5. `function_styles.c`
**Purpose**: Function definition detection  
**Tests**: Various function definition styles:
- Same-line brace: `int foo() {`
- K&R style (brace on next line): `int bar()\n{`
- Allman style with parameters: `int baz(int x, int y)\n{`
- Function prototypes (should be excluded)

**Expected detection**: 4 functions (foo, bar, baz, main)

### 6. `commented_includes.c`
**Purpose**: Include directive robustness  
**Tests**:
- Commented-out includes should be ignored: `// #include <stdlib.h>`
- Block-commented includes: `/* #include <string.h> */`
- Whitespace variations: `#  include   <stddef.h>`
- Both `<>` and `""` include forms

## Running Tests

### Run All Tests
```bash
./tests/run_tests.sh
```

Or using the Makefile:
```bash
make test-metrics
```

### Run from CI
The test script is designed to work in CI environments:
```bash
cd /path/to/repository
make code_metrics
./tests/run_tests.sh
```

### Run Specific Test Manually
You can test individual fixtures manually:
```bash
# Build the tool
make code_metrics

# Run on a specific fixture
./code_metrics tests/fixtures/simple.c

# Compare with expected output
./code_metrics tests/fixtures/simple.c 2>&1 | grep "Total Lines"
```

## Test Output Format

The test script extracts specific metrics from the code_metrics output and compares them with expected values in `.expected` files.

Expected file format (key:value pairs):
```
total_lines:7
code_lines:5
blank_lines:1
comment_lines:1
function_count:1
include_count:1
```

## Adding New Tests

To add a new test:

1. Create a new C source file in `tests/fixtures/`:
   ```bash
   vim tests/fixtures/my_test.c
   ```

2. Run code_metrics on it to determine expected values:
   ```bash
   ./code_metrics tests/fixtures/my_test.c
   ```

3. Create an expected output file:
   ```bash
   cat > tests/fixtures/my_test.expected << EOF
   total_lines:X
   code_lines:Y
   blank_lines:Z
   comment_lines:W
   function_count:A
   include_count:B
   EOF
   ```

4. Run tests to verify:
   ```bash
   ./tests/run_tests.sh
   ```

## Test Coverage

The test suite validates:
- ✅ Basic line counting (code, blank, comments)
- ✅ Multi-line block comment tracking with state machine
- ✅ Inline comment detection (code + comment on same line)
- ✅ Comment markers inside strings and character literals
- ✅ Escape sequence handling in strings/chars
- ✅ Function definition detection (multiple styles)
- ✅ Function prototype exclusion
- ✅ Include directive detection with various formatting
- ✅ Commented-out includes

## Known Limitations

The test suite currently does NOT test:
- Lines longer than MAX_LINE_LENGTH (1024 chars) - would require special fixture
- Files with >100 functions or >50 includes - would require large fixture
- Unusual C++ constructs (tool is C-focused)
- Macro-generated code patterns
- Preprocessor continuations with backslash

These edge cases are documented in CODE_METRICS_README.md as known limitations.

## CI Integration

The test suite is integrated into GitHub Actions CI:
- Runs on every PR
- Tests with both GCC and Clang compilers
- Compiles with strict warnings (`-Werror`)
- All tests must pass before merge

See `.github/workflows/ci.yml` for CI configuration.

## Troubleshooting

If tests fail:

1. Check that code_metrics is built:
   ```bash
   ls -la code_metrics
   ```

2. Run the failing test manually to see detailed output:
   ```bash
   ./code_metrics tests/fixtures/<failing_test>.c
   ```

3. Compare with expected values in the `.expected` file

4. If the test is correct but expected values are wrong, update the `.expected` file

5. If code_metrics behavior changed, update both the code and expected files
