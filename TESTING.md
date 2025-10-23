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

**Diagnostic Output:**
On failure, the script provides detailed information:
- GCC compiler version check and clear error if missing
- Exact compilation command and GCC error messages
- Expected vs actual program output
- Exit codes for different failure modes

**CI Requirements:**
- Must run in an environment with GCC installed (ubuntu-latest runners include GCC by default)
- The script includes a compiler check and fails with instructions if GCC is not available

To view failure logs in CI, check the "Test visual editor generated code" step in the GitHub Actions workflow run.

### 3. XSS Integration Tests (`test/test_xss_integration.js`)

**Purpose:** Validates that the editor implementation correctly uses XSS protection mechanisms throughout the codebase.

**Requirements:**
- Node.js v14 or higher (pinned to v18 LTS in CI)
- **No npm dependencies** - uses only built-in Node.js APIs (fs, path)
- Access to editor.html source file

**What it tests:**
- No unsafe innerHTML usage with user data
- escapeHtml function exists and is used
- textContent is used for DOM manipulation
- Safe Blob/download implementation
- No eval or Function constructor usage
- Export filename is hardcoded (not user-controlled)

**How to run:**
```bash
node test/test_xss_integration.js
```

**Integration vs Unit:**
- Unit tests (`test_escape_html.js`) verify the escaping function in isolation
- Integration tests verify the function is used correctly throughout the editor

### 2. XSS Escaping Unit Tests (`test/test_escape_html.js`)

**Purpose:** Validates that the `escapeHtml()` function properly escapes user input to prevent XSS attacks.

**Requirements:**
- Node.js v14 or higher (pinned to v18 LTS in CI via setup-node action)
- **No npm dependencies** - uses only built-in Node.js APIs
- The test is designed to run in Node.js only (not browser-based)

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

### 4. Integration with CI/CD

All tests are integrated into the GitHub Actions CI workflow (`.github/workflows/ci.yml`):

```yaml
- name: Setup Node.js
  uses: actions/setup-node@v4
  with:
    node-version: '18.x'

- name: Print environment versions
  run: |
    echo "Node.js version: $(node --version)"
    echo "GCC version: $(gcc --version | head -1)"
    echo "Make version: $(make --version | head -1)"

- name: Test visual editor generated code
  run: bash test/test_editor_output.sh
  
- name: Test XSS escaping function
  run: node test/test_escape_html.js
  
- name: Test XSS integration
  run: node test/test_xss_integration.js
```

These tests run on every push and pull request to ensure:
- Generated C code remains syntactically valid
- Security escaping functions work correctly
- Escaping is properly integrated throughout the codebase
- No regressions are introduced

**Node.js Version:** CI uses setup-node action to pin Node.js v18 LTS (recommended current LTS version), ensuring consistent test environment across runs. All Node.js tests use only built-in APIs and require no npm dependencies.

**Why Node.js 18 LTS:**
- Node.js 14 reached End-of-Life on April 30, 2023 and no longer receives security updates
- Node.js 18 is the current Active LTS (Long Term Support) version with support until April 2025
- Provides modern JavaScript features and security patches
- CI runners have Node.js 18 readily available
- Upgrade path: Will update to Node.js 20 LTS (next LTS) when it becomes Active LTS

### 5. CodeQL Security Scanning

CodeQL security analysis is configured in `.github/workflows/codeql.yml` and scans:

**Languages Analyzed:**
- **JavaScript**: Scans `editor.html` for security vulnerabilities in the web interface code
- **C/C++ (cpp)**: Scans C code including `hello.c` and validates C code generation patterns
  - Note: CodeQL uses the identifier `'cpp'` for both C and C++ language analysis

**When it runs:**
- On every push to main branch
- On every pull request to main branch (including from forks)
- Weekly on Monday (scheduled scan)

**Why 'cpp' for C code:** CodeQL does not have a separate 'c' language identifier. The 'cpp' analyzer handles both C and C++ code, making it suitable for analyzing the C code in this repository.

**Security Posture:** 
- The workflow runs on all pull requests including those from forks to ensure code quality
- Workflow permissions are restricted (read-only for code, write for security-events)
- Fork PRs can trigger the analysis, helping catch vulnerabilities early
- Scheduled scans run only on the main branch to avoid unnecessary fork runs

**Permissions:**
```yaml
permissions:
  actions: read
  contents: read
  security-events: write
```

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
- **XSS prevention**: ✅ Verified with unit and integration tests
- **Compilation**: ✅ Strict GCC flags ensure quality
- **Runtime behavior**: ✅ Output validation
- **Timeout protection**: ✅ 5-second limit prevents hangs
- **Security**: ✅ Multiple XSS attack vectors tested
- **Export safety**: ✅ Hardcoded filename, safe Blob usage
- **Accessibility**: 🔶 Manual testing (automated tests planned)

## Accessibility Testing

**Current Status:** Accessibility features are tested manually

**Manual Test Checklist:**
- [ ] Keyboard navigation: Tab through all interactive elements
- [ ] Enter key: Activate buttons and add blocks
- [ ] Focus indicators: Visible outline on focused elements
- [ ] ARIA labels: Screen reader announces button purposes
- [ ] Semantic HTML: Proper button and role attributes
- [ ] Color contrast: Sufficient contrast for readability

**Automated Testing (Planned):**
- Consider adding axe-core or similar accessibility audit tool
- Integrate accessibility checks into CI workflow
- Document in future enhancement section below

## Future Test Enhancements

### Planned Automated UI/E2E Tests
**Status:** Not yet implemented (manual testing currently used)

**Proposed approach:**
- Use Playwright or Puppeteer for browser automation
- Test user interactions: add blocks, edit content, export file
- Verify visual regressions
- Test across multiple browsers automatically

**Why deferred:** Current manual testing provides adequate coverage for initial release. Automated UI tests will be added in a follow-up PR to catch regressions as the editor evolves.

**Test scenarios to automate:**
1. Load editor and verify default program loads
2. Add each type of block and verify it appears in workspace
3. Edit block content and verify changes appear in generated code
4. Reorder blocks and verify code generation updates
5. Export file and verify download triggers with correct filename
6. Test XSS payload and verify proper escaping in UI

### Other Potential Enhancements
- Browser-based integration tests for UI interactions
- Additional C code generation patterns (loops, variables, functions)
- File download/export functionality testing in browsers
- Keyboard navigation automated testing
- Cross-browser compatibility automated tests with BrowserStack or similar

**Note on Browser Testing:** Currently, browser-based UI testing is performed manually. The `test/test_escape_html.js` and `test/test_xss_integration.js` run in Node.js only and validate the security implementation without requiring a browser.
