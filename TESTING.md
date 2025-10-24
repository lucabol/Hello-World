# Testing Strategy & Documentation

This document explains the testing approach for the visual C code editor, including design decisions, limitations, and maintenance guidelines.

## Overview

The test suite consists of 75 tests across 5 test files:
- **35 unit tests** (test_generator_unit.js) - Core code generation logic
- **22 integration tests** (test_editor_integration.js) - HTML validation & module compatibility
- **10 HTML escape tests** (test_escape_html.js) - Security escaping functions
- **7 XSS security tests** (test_xss_integration.js) - Cross-site scripting protection
- **1 compilation test** (test_editor_output.sh) - GCC smoke test

## Running Tests

### Quick Start
```bash
make test           # Run all tests
make test-quick     # Skip GCC compilation test
make help           # Show all available targets
```

### Individual Test Suites
```bash
make test-unit          # 35 unit tests
make test-integration   # 22 integration tests
make test-escape        # 10 HTML escape tests
make test-xss           # 7 XSS security tests
make test-output        # 1 compilation smoke test
```

### Direct Execution
```bash
node test/test_generator_unit.js
node test/test_editor_integration.js
node test/test_escape_html.js
node test/test_xss_integration.js
bash test/test_editor_output.sh
```

## Requirements

### Essential
- **Node.js** - For running JavaScript tests (version 18.x or later recommended)
- All tests use only Node.js built-in modules (fs, path, child_process)

### Optional
- **GCC** - Only required for `test_editor_output.sh` compilation smoke test
  - **Default behavior**: Unit tests auto-detect GCC and skip compilation if not found
  - **Exit code**: Tests exit with 0 even when GCC compilation is skipped
  - **Use `make test-quick`** to explicitly skip compilation tests
  
#### GCC Behavior Details

**Which tests use GCC:**
- `test/test_editor_output.sh` - Always requires GCC (exits with error if not found)
- `test/test_generator_unit.js` - Auto-detects GCC and skips compilation tests gracefully

**In CI environments:**
- **PR validation**: Use `make test-quick` (no GCC needed, faster feedback)
- **Main branch/nightly**: Use `make test` (requires GCC for full validation)
- **Cross-platform**: Unit/integration tests work on Windows/macOS without compilers

**Installation commands:**
```bash
# Ubuntu/Debian
sudo apt-get update && sudo apt-get install -y gcc

# Alpine Linux
apk add gcc musl-dev

# macOS
xcode-select --install

# Windows (via MinGW)
# Not required - use test-quick on Windows CI
```

### No External Dependencies
This project intentionally avoids external npm packages for:
- Zero installation friction
- CI simplicity
- Educational clarity
- Long-term maintainability

## HTML Validation Approach

### Design Decision: Regex-Based Validation

The integration tests use **regex-based HTML validation** instead of a DOM parser (jsdom/parse5). This is an intentional trade-off:

#### Advantages ✅
- **Zero dependencies** - Works immediately without npm install
- **Fast execution** - No parsing overhead
- **Simple setup** - Easy for students/beginners to understand
- **CI-friendly** - No compatibility issues across Node versions

#### Limitations ⚠️
- **Formatting sensitivity** - Complex whitespace or comment patterns may confuse regex
- **No semantic understanding** - Cannot validate HTML structure beyond pattern matching
- **Maintenance burden** - Regex updates needed when HTML structure changes significantly

### Regex Validation Features

The `parseHtmlForElement` helper uses sophisticated patterns to handle:

1. **Order-insensitive attributes** - Uses lookaheads: `(?=(?:[^>]*\battr\s*=\s*["']value["']))`
2. **Multiple quote styles** - Matches both `"` and `'`
3. **Whitespace tolerance** - Flexible spacing around `=` signs
4. **Boolean attributes** - Handles `checked`, `disabled` with or without values
5. **Regex injection prevention** - Escapes all user-provided strings

### Known Limitations

The regex approach **cannot reliably handle**:

1. **HTML comments** - `<!-- commented attribute="value" -->` may be matched
2. **Nested tags** - Complex nesting may produce false positives
3. **CDATA sections** - Special XML/HTML sections are not recognized
4. **Malformed HTML** - May match invalid HTML that browsers would reject
5. **Attribute value edge cases** - Unusual quoting or escaping may fail

### HTML Variations Intentionally Tolerated

✅ **Will pass validation:**
- Different attribute orders: `<meta http-equiv="CSP" content="...">` vs `<meta content="..." http-equiv="CSP">`
- Quote styles: `attr="value"` vs `attr='value'`
- Whitespace: `attr="value"` vs `attr = "value"` vs `attr= "value"`
- Boolean attrs: `checked` vs `checked=""` vs `checked="checked"`
- Path variations: `src="generator.js"` vs `src="./generator.js"`

❌ **Will fail validation (considered breaking changes):**
- Changing tag names: `<meta>` → `<link>`
- Removing required attributes: Missing `http-equiv` in CSP meta tag
- Changing attribute semantics: `http-equiv="CSP"` → `name="CSP"`
- Structural changes: Moving inline scripts to external files without updating `<script>` tags

## Migration Path to Parser-Based Testing

If the regex approach becomes too brittle, here's how to upgrade:

### Option 1: Add jsdom (Recommended)

```bash
npm install --save-dev jsdom
```

Update `test/test_editor_integration.js`:

```javascript
// At top of file
let useParser = false;
try {
  const jsdom = require('jsdom');
  const { JSDOM } = jsdom;
  useParser = true;
} catch (e) {
  console.log('jsdom not available, using regex validation');
}

// Replace parseHtmlForElement with:
function parseHtmlForElement_DOM(html, tagName, attributes) {
  const dom = new JSDOM(html);
  const elements = dom.window.document.getElementsByTagName(tagName);
  for (let elem of elements) {
    let matches = true;
    for (let [key, value] of Object.entries(attributes)) {
      if (elem.getAttribute(key) !== value) {
        matches = false;
        break;
      }
    }
    if (matches) return true;
  }
  return false;
}
```

### Option 2: Add parse5

```bash
npm install --save-dev parse5
```

Similar approach but using parse5's AST traversal.

### Option 3: Keep Regex + Add Optional Parser Test

Create `test/test_editor_integration_dom.js` that runs only when jsdom is available:

```bash
npm install --save-dev jsdom  # Optional
make test-integration         # Uses regex (always works)
make test-integration-dom     # Uses jsdom (if installed)
```

## Test Maintenance Guidelines

### When HTML Changes

1. **Minor formatting changes** - Existing regex tests should pass automatically
2. **New attributes** - Add new assertions to `test_editor_integration.js`
3. **Structural changes** - Update both regex patterns and expected values
4. **New features** - Add new test cases to appropriate test file

### When Tests Fail

1. **Verify the HTML actually changed** - View `tools/editor/editor.html`
2. **Check if change is intentional** - Security/accessibility changes should be intentional
3. **Update test expectations** - Modify regex patterns or add new test cases
4. **Document the reason** - Add comments explaining why the test was updated

### Adding New Tests

1. **Unit tests** - Add to `test/test_generator_unit.js` for code generation logic
2. **Integration tests** - Add to `test/test_editor_integration.js` for HTML/UI validation
3. **Security tests** - Add to `test/test_xss_integration.js` or `test/test_escape_html.js`

## CI Integration

### GitHub Actions Workflow

This repository includes `.github/workflows/test-visual-editor.yml` which provides:

**Pull Request Validation (Fast):**
- Runs `make test-quick` on every PR
- No GCC required (unit, integration, security tests only)
- Tests on ubuntu-latest with Node.js 20.x
- Typical runtime: < 30 seconds

**Main Branch & Nightly (Comprehensive):**
- Runs `make test` on main branch and nightly schedule
- Includes GCC compilation smoke test
- Validates full end-to-end workflow

**Cross-Platform Testing:**
- Tests on Ubuntu, macOS, and Windows
- Tests with Node.js 18.x and 20.x
- Skips GCC compilation (not available on all platforms)

### Custom CI Setup

#### Fast PR validation (recommended)
```yaml
name: Quick Tests
on: [pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-node@v4
        with:
          node-version: '20.x'
      
      - name: Run tests without GCC
        run: make test-quick
```

#### Full validation with GCC
```yaml
name: Full Tests
on: [push]

jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-node@v4
        with:
          node-version: '20.x'
      
      - name: Install GCC
        run: |
          sudo apt-get update
          sudo apt-get install -y gcc
      
      - name: Run all tests
        run: make test
```

### Environment Requirements

- **Node.js**: v18 or later (uses ES6+ features)
- **GCC**: Optional (only for `test-output` target)
- **OS**: Linux (primary), macOS (supported), Windows (unit tests only)
- **Shell**: bash (required for Makefile, automatically configured)

## Test Output

### Success
```
=========================================
Running complete test suite (74 tests)
=========================================

1/5: Unit tests (generator module)...
✅ All 35 unit tests passed

2/5: Integration tests (HTML validation & module compatibility)...
✅ All 22 integration tests passed

... (remaining test output)

=========================================
✅ All tests passed successfully!
=========================================
```

### Failure
Tests exit with non-zero status and show detailed error messages:

```
❌ Test failed: CSP meta tag validation
Expected: CSP meta tag with script-src directive
Found: <meta content="default-src 'self'">
Missing: script-src directive
```

## Security Testing

### XSS Protection Tests

The test suite validates that all user inputs are properly escaped:

1. **HTML entities**: `<`, `>`, `&`, `"`, `'`
2. **Printf format specifiers**: `%` → `%%`
3. **DOM manipulation**: Using `textContent` instead of `innerHTML`
4. **No eval()**: Static analysis confirms no dynamic code execution

### What We Test

✅ **Covered:**
- HTML escaping of user input
- Printf format string safety
- CSP meta tag presence and basic validation
- Module export integrity
- ARIA accessibility structure
- Keyboard navigation hooks

❌ **Not covered (manual review needed):**
- Runtime browser behavior
- Actual screen reader announcements
- Visual rendering accuracy
- Drag-and-drop UX
- File download functionality

## Performance

Test suite execution time (approximate):
- Unit tests: ~2 seconds (includes GCC compilation of test cases)
- Integration tests: <1 second (HTML parsing only)
- All tests: ~3 seconds total

## Troubleshooting

### "GCC not found" Warning
- **Solution**: Install GCC or use `make test-quick`
- **Impact**: Only affects compilation smoke test (1 test)

### "Module not found" Error
- **Solution**: Ensure you're running from repository root
- **Impact**: Cannot find test files or generator module

### Regex Pattern Too Complex
- **Solution**: Consider migrating to jsdom (see Migration Path above)
- **Impact**: Test maintenance becomes difficult

## Future Improvements

Potential enhancements (not currently planned):

1. **Headless browser tests** - Use Playwright/Puppeteer for true E2E testing
2. **Visual regression tests** - Screenshot comparison for UI changes
3. **Coverage reports** - Track code coverage metrics
4. **Performance benchmarks** - Measure code generation speed
5. **Fuzzing** - Random input testing for robustness

## Questions?

For questions about testing strategy or contributing tests, see:
- **EDITOR_GUIDE.md** - User-facing documentation
- **README.md** - Project overview
- **tools/editor/generator.js** - Inline code documentation
