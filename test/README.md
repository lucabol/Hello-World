# Test Suite for Visual C Code Editor

This directory contains comprehensive automated tests for the visual block-based C code editor.

## Test Overview

| Test Suite | Type | Tests | Purpose |
|------------|------|-------|---------|
| `test_escape_html.js` | Unit | 12 | HTML escaping edge cases |
| `test_xss_integration.js` | Security | 7 | XSS attack prevention |
| `test_innerHTML_scan.js` | Security | 5 | Dangerous API detection |
| `test_c_generation.js` | Unit | 7 | C code safety & correctness |
| `test_header_validation.js` | Unit | 57 | Input validation (path traversal, injection) |
| `test_e2e_editor.js` | E2E | 33 | End-to-end workflow validation |
| `test_accessibility.js` | A11y | 31 | Accessibility compliance |
| **TOTAL** | | **152** | |

## Running Tests

### All Tests (CI)
```bash
# Run all test suites
cd test
./run_unit_tests.sh
```

### Individual Test Suites
```bash
# HTML escaping tests
node test/test_escape_html.js

# XSS security tests
node test/test_xss_integration.js

# innerHTML security scanner
node test/test_innerHTML_scan.js

# C code generation tests
node test/test_c_generation.js

# Header validation tests (57 edge cases)
node test/test_header_validation.js

# End-to-end workflow tests
node test/test_e2e_editor.js

# Accessibility tests
node test/test_accessibility.js
```

## Test Types Explained

### Unit Tests
Standard unit tests that verify individual functions and components work correctly with edge cases.

### Security Tests
Validate that the editor is protected against:
- XSS attacks (escaping, innerHTML usage)
- Format string vulnerabilities
- Path traversal attacks
- Command injection
- Protocol scheme injection

### E2E Tests (Static Analysis)
**Note:** These are **NOT browser automation tests**. They are lightweight static analysis tests that:
- Parse `editor.html` and verify its structure
- Check for presence of required functions and security features
- Validate ARIA attributes and accessibility markup
- Verify code generation patterns
- Test keyboard navigation support

**No browser or headless automation framework required** (no Puppeteer, Playwright, Selenium).

### Interactive Browser Testing
For full interactive testing (drag-and-drop, visual validation):

```bash
# Option 1: Open in browser manually
open editor.html

# Option 2: Use Playwright (future enhancement)
npx playwright test editor.spec.js

# Option 3: Use Puppeteer (future enhancement)
node test/puppeteer-interactive-tests.js
```

## Test Requirements

### Node.js Version
- Node.js 14.x or higher
- No additional npm packages required for current tests
- Standard library only (fs, path)

### CI Environment
Tests run in GitHub Actions on:
- Ubuntu latest
- Node.js 20.x

No browser installation needed for current test suite.

## Writing New Tests

### Test Template
```javascript
#!/usr/bin/env node

console.log('🧪 Running My Tests...\n');

let passedTests = 0;
let failedTests = 0;

function test(description, fn) {
    try {
        fn();
        console.log(`  ✓ ${description}`);
        passedTests++;
    } catch (error) {
        console.log(`  ✗ ${description}`);
        console.log(`    ${error.message}`);
        failedTests++;
    }
}

function assert(condition, message) {
    if (!condition) {
        throw new Error(message || 'Assertion failed');
    }
}

// Your tests here
test('Example test', () => {
    assert(true, 'Should pass');
});

// Summary
console.log(`\n${passedTests} passed, ${failedTests} failed`);
process.exit(failedTests > 0 ? 1 : 0);
```

## Test Coverage

### Security Coverage
- ✅ XSS protection (HTML escaping)
- ✅ Format string vulnerabilities (printf safety)
- ✅ Path traversal (directory navigation)
- ✅ Command injection (shell metacharacters)
- ✅ Protocol injection (http://, file://)
- ✅ Backslash paths (Windows-style)
- ✅ UNC paths (network shares)
- ✅ Dangerous APIs (eval, innerHTML, document.write)

### Accessibility Coverage
- ✅ ARIA roles and labels
- ✅ Keyboard navigation (Tab, Enter, Space)
- ✅ Focus management and indicators
- ✅ Screen reader support (aria-live)
- ✅ Semantic HTML (header, main, h1, h2)
- ✅ Color contrast (WCAG AA)

### Functionality Coverage
- ✅ Block creation (include, printf, return)
- ✅ Code generation with escaping
- ✅ Include deduplication (case-insensitive)
- ✅ Export functionality
- ✅ Input validation
- ✅ Touch/mobile support

## Continuous Integration

Tests run automatically on:
- Every push to the repository
- Every pull request
- Before merge to main

See `.github/workflows/ci.yml` for full CI configuration.

## Troubleshooting

### Tests Failing Locally
```bash
# Ensure Node.js version is compatible
node --version  # Should be 14.x or higher

# Check file permissions
chmod +x test/*.js

# Run with verbose output
node test/test_name.js 2>&1 | tee test-output.log
```

### CI Failures
1. Check GitHub Actions logs for specific error messages
2. Verify all test files are committed and pushed
3. Ensure no OS-specific path issues (use path.join)
4. Confirm file encodings are UTF-8

## Future Enhancements

Planned additions:
- [ ] Playwright-based interactive browser tests
- [ ] Visual regression testing
- [ ] Performance benchmarks
- [ ] Code coverage reports
- [ ] Mutation testing

## Contributing

When adding new tests:
1. Follow the existing test patterns
2. Use descriptive test names
3. Add assertions with clear messages
4. Update this README
5. Ensure tests are deterministic (no flaky tests)
6. Run locally before committing

## License

Tests are part of the Hello-World project and follow the same license.
