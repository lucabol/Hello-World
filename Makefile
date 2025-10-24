# Makefile for Hello-World project with visual C code editor
#
# This Makefile provides convenient targets for running the test suite.
# All tests are designed to run with zero external dependencies (except GCC for compilation tests).
#
# REQUIREMENTS:
#   - GNU Make (required for .SHELLFLAGS support)
#   - Node.js (for JavaScript tests)
#   - Bash (for shell scripts and strict error handling)
#   - GCC (optional, only for compilation smoke test)
#
# Shell configuration: Use bash for better error handling
SHELL := /bin/bash

# Set strict shell flags for GNU Make (fails on undefined variables, errors, and pipe failures)
# NOTE: .SHELLFLAGS is a GNU Make extension. On BSD make this will be ignored.
# If using non-GNU make, these flags won't be set but tests should still work.
ifeq ($(origin .SHELLFLAGS),undefined)
$(warning WARNING: .SHELLFLAGS is not supported by this make implementation.)
$(warning         Some error handling may not work as expected.)
$(warning         Consider using GNU Make for best results.)
else
.SHELLFLAGS := -e -u -o pipefail -c
endif

.PHONY: all test test-unit test-integration test-escape test-xss test-output test-all test-quick help clean

# Default target
all: test

# Run all tests
test: test-all

test-all:
	@echo "========================================="
	@echo "Running complete test suite (75 tests)"
	@echo "========================================="
	@echo ""
	@echo "1/5: Unit tests (generator module)..."
	@node test/test_generator_unit.js
	@echo ""
	@echo "2/5: Integration tests (HTML validation & module compatibility)..."
	@node test/test_editor_integration.js
	@echo ""
	@echo "3/5: HTML escape tests..."
	@node test/test_escape_html.js
	@echo ""
	@echo "4/5: XSS security tests..."
	@node test/test_xss_integration.js
	@echo ""
	@echo "5/5: Output compilation smoke test..."
	@if command -v gcc >/dev/null 2>&1; then \
		bash test/test_editor_output.sh; \
	else \
		echo "⚠️  GCC not found - skipping compilation test"; \
		echo "   Install GCC to run the full test suite"; \
	fi
	@echo ""
	@echo "========================================="
	@echo "✅ All tests passed successfully!"
	@echo "========================================="

# Individual test targets
test-unit:
	@echo "Running unit tests (35 tests)..."
	@node test/test_generator_unit.js

test-integration:
	@echo "Running integration tests (22 tests)..."
	@node test/test_editor_integration.js

test-escape:
	@echo "Running HTML escape tests (10 tests)..."
	@node test/test_escape_html.js

test-xss:
	@echo "Running XSS security tests (7 tests)..."
	@node test/test_xss_integration.js

test-output:
	@echo "Running output compilation smoke test..."
	@if command -v gcc >/dev/null 2>&1; then \
		bash test/test_editor_output.sh; \
	else \
		echo "❌ GCC not found - cannot run compilation test"; \
		echo "   Install GCC to run this test"; \
		exit 1; \
	fi

# Quick test (skip GCC compilation)
test-quick:
	@echo "Running tests without GCC compilation..."
	@node test/test_generator_unit.js
	@node test/test_editor_integration.js
	@node test/test_escape_html.js
	@node test/test_xss_integration.js
	@echo "✅ Quick tests passed (compilation test skipped)"

# Clean generated files
# Note: Only removes files we know we generate, avoiding accidental deletion
clean:
	@echo "Cleaning generated files..."
	@rm -f hello hello_debug hello_clang
	@rm -f *.exe *.out *.o
	@rm -f test/*.o test/hello
	@echo "✅ Clean complete"

# Help target
help:
	@echo "Hello-World Test Suite Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  make test          - Run all tests (default, auto-skips GCC if not found)"
	@echo "  make test-all      - Run all tests with detailed output"
	@echo "  make test-quick    - Run tests without GCC compilation (recommended for CI)"
	@echo "  make test-unit     - Run only unit tests (35 tests)"
	@echo "  make test-integration - Run only integration tests (22 tests)"
	@echo "  make test-escape   - Run only HTML escape tests (10 tests)"
	@echo "  make test-xss      - Run only XSS security tests (7 tests)"
	@echo "  make test-output   - Run only compilation smoke test (requires GCC)"
	@echo "  make clean         - Remove generated files"
	@echo "  make help          - Show this help message"
	@echo ""
	@echo "Requirements:"
	@echo "  - GNU Make (recommended for full error handling)"
	@echo "  - Bash shell"
	@echo "  - Node.js (for running JavaScript tests)"
	@echo "  - GCC (optional, only for compilation smoke test)"
	@echo ""
	@echo "GCC behavior:"
	@echo "  - 'make test' auto-detects and skips if GCC is not found"
	@echo "  - 'make test-quick' never uses GCC (best for CI)"
	@echo "  - 'make test-output' requires GCC and will fail if not found"
	@echo ""
	@echo "For more details, see TESTING.md"
