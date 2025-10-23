#!/bin/bash
# Test script for the code metrics analyzer

set -e

echo "Testing Code Metrics Analyzer..."

# Clean up any existing files
rm -f metrics.json metrics-report.html

# Run the analyzer
node metrics-analyzer.js hello.c

# Verify JSON output exists and has expected content
if [ ! -f metrics.json ]; then
    echo "✗ metrics.json was not created"
    exit 1
fi

# Verify HTML output exists
if [ ! -f metrics-report.html ]; then
    echo "✗ metrics-report.html was not created"
    exit 1
fi

# Check JSON content
if ! grep -q '"fileName": "hello.c"' metrics.json; then
    echo "✗ metrics.json does not contain expected content"
    exit 1
fi

if ! grep -q '"mainFunction": true' metrics.json; then
    echo "✗ metrics.json does not detect main function"
    exit 1
fi

# Check HTML content
if ! grep -q "Code Metrics Dashboard" metrics-report.html; then
    echo "✗ metrics-report.html does not contain dashboard"
    exit 1
fi

if ! grep -q "hello.c" metrics-report.html; then
    echo "✗ metrics-report.html does not contain file name"
    exit 1
fi

echo "✓ All metrics analyzer tests passed!"

# Clean up
rm -f metrics.json metrics-report.html

exit 0
