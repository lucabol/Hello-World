# Testing Documentation

This document describes the automated tests for the Visual C Code Block Editor.

## Test Suite

### 1. Editor Output Smoke Test (`test/test_editor_output.sh`)

**Purpose:** Validates that the default generated C code from the editor compiles and runs correctly.

**What it tests:**
- Generates the default "Hello world!" program (same as editor's initial state)
- Compiles with strict GCC flags: `-Wall -Wextra -Wpedantic -Werror -std=c99`
- Executes the compiled program with a 5-second timeout
- Verifies the output matches expected "Hello world!"

**How to run:**
```bash
bash test/test_editor_output.sh
```

**Exit codes:**
- `0`: All tests passed
- `1`: Compilation or runtime failure
- `124`: Execution timeout

**Features:**
- Deterministic test (no GUI/browser interaction)
- Automatic cleanup using trap
- Fail-fast error handling with `set -euo pipefail`
- Timeout protection to prevent hung CI jobs

### 2. XSS Escaping Unit Tests (`test/test_escape_html.js`)

**Purpose:** Validates that the `escapeHtml()` function properly escapes user input to prevent XSS attacks.

**What it tests:**
- XSS script tags: `<script>alert('XSS')</script>`
- HTML injection: `<img src=x onerror=alert('XSS')>`
- Special characters: `<`, `>`, `&`, `"`, `'`
- Mixed attack vectors
- Normal text (no false positives)

**Test cases covered:**
1. XSS script tag injection
2. HTML img tag with onerror handler
3. Double quotes escaping
4. Single quotes escaping
5. Ampersand escaping
6. Less than and greater than signs
7. Mixed special characters
8. Normal text without special chars
9. Include directive (stdio.h)
10. Empty string

**How to run:**
```bash
node test/test_escape_html.js
```

**Exit codes:**
- `0`: All 10 tests passed
- `1`: One or more tests failed

**Expected escaping behavior:**
- `<` → `&lt;`
- `>` → `&gt;`
- `&` → `&amp;`
- `"` → `&quot;`
- `'` → `&#039;`

### 3. Integration with CI/CD

Both tests are integrated into the GitHub Actions CI workflow (`.github/workflows/ci.yml`):

```yaml
- name: Test visual editor generated code
  run: bash test/test_editor_output.sh
  
- name: Test XSS escaping function
  run: node test/test_escape_html.js
```

These tests run on every push and pull request to ensure:
- Generated C code remains syntactically valid
- Security escaping functions work correctly
- No regressions are introduced

## Running All Tests

To run all tests locally:

```bash
# Run editor output smoke test
bash test/test_editor_output.sh

# Run XSS escaping unit tests
node test/test_escape_html.js

# Or run both in sequence
bash test/test_editor_output.sh && node test/test_escape_html.js
```

## Test Coverage

- **Code generation**: ✅ Verified with smoke test
- **XSS prevention**: ✅ Verified with unit tests
- **Compilation**: ✅ Strict GCC flags ensure quality
- **Runtime behavior**: ✅ Output validation
- **Timeout protection**: ✅ 5-second limit prevents hangs
- **Security**: ✅ Multiple XSS attack vectors tested

## Future Test Enhancements

Potential areas for additional testing:
- Browser-based integration tests (e.g., with Playwright)
- Additional C code generation patterns (loops, variables)
- File download/export functionality
- Keyboard navigation and accessibility
- Cross-browser compatibility automated tests
