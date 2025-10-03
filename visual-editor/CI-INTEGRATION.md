# CI Integration Guide

This document provides guidance for integrating the visual editor tests into CI/CD pipelines.

## Quick Start

Add to your GitHub Actions workflow:

```yaml
visual-editor-tests:
  runs-on: ubuntu-latest
  steps:
    - uses: actions/checkout@v4
    
    - name: Test Visual Editor Code Generation
      run: bash visual-editor/test-integration.sh
```

## What Gets Tested

The integration test suite (`test-integration.sh`) validates:

1. **Default Hello World**: Verifies basic code generation
2. **Multiple Prints**: Tests concatenation of statements
3. **Print with Newline**: Validates escape sequence handling
4. **Comments**: Tests comment generation and safety
5. **Exit Codes**: Ensures programs exit correctly

All tests compile with strict GCC flags:
```bash
-Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
```

## No External Dependencies

The tests run completely offline:
- No CDN downloads required
- No npm/node dependencies
- No browser automation needed (for integration tests)
- Pure bash + GCC

## Full Example Workflow

```yaml
name: Visual Editor Tests

on: [push, pull_request]

jobs:
  test-visual-editor:
    runs-on: ubuntu-latest
    
    steps:
      - name: Checkout code
        uses: actions/checkout@v4
      
      - name: Verify GCC installation
        run: gcc --version
      
      - name: Run integration tests
        run: |
          cd visual-editor
          bash test-integration.sh
      
      - name: Verify no external dependencies
        run: |
          cd visual-editor
          # Should return no results
          if grep -i "cdn\|https://" index.html; then
            echo "Error: External dependencies found"
            exit 1
          fi
          echo "✓ No external dependencies confirmed"
```

## Optional: Browser UI Tests

For testing the UI itself (not required for code generation):

```yaml
  ui-tests:
    runs-on: ubuntu-latest
    
    steps:
      - uses: actions/checkout@v4
      
      - name: Setup Node.js (for Playwright)
        uses: actions/setup-node@v4
        with:
          node-version: '18'
      
      - name: Install Playwright
        run: |
          npm init -y
          npm install @playwright/test
          npx playwright install chromium
      
      - name: Run UI tests
        run: npx playwright test
```

Note: UI tests are optional. The integration tests validate code generation without browser automation.

## Testing Offline

To test without network access:

```bash
# Disable network
sudo ifconfig eth0 down

# Run tests
bash visual-editor/test-integration.sh

# Re-enable network
sudo ifconfig eth0 up
```

This confirms the editor works in airgapped environments.

## Expected Output

Successful test run:
```
Visual Editor Integration Test
================================

Test 1: Default Hello World output
✓ Test 1 PASSED: Output matches expected 'Hello world!'

Test 2: Multiple print statements
✓ Test 2 PASSED: Multiple prints work correctly

Test 3: Print with newline
✓ Test 3 PASSED: Print with newline works correctly

Test 4: Code with comments
✓ Test 4 PASSED: Comments work correctly

Test 5: Program exit code
✓ Test 5 PASSED: Exit code is 0

================================
All tests PASSED! ✓
```

## Troubleshooting

### GCC not found
```yaml
- name: Install GCC
  run: sudo apt-get update && sudo apt-get install -y gcc
```

### Tests fail in CI but pass locally
- Check GCC version: Tests require GCC 4.9+
- Verify bash version: Tests require Bash 4.0+
- Check temp directory permissions

### Timeout issues
Increase timeout if compiling is slow:
```yaml
- name: Run tests
  run: bash visual-editor/test-integration.sh
  timeout-minutes: 5
```

## Performance

Expected CI runtime:
- Test execution: ~2-5 seconds
- Total job time: ~10-30 seconds (including checkout)

The tests are fast and suitable for every commit.
