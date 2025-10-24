# Makefile for Hello-World project with visual C code editor
#
# This Makefile provides convenient targets for running the test suite.
# All tests are designed to run with zero external dependencies (except GCC for compilation tests).

.PHONY: all test test-unit test-integration test-escape test-xss test-output test-all help clean

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
	@bash test/test_editor_output.sh
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
	@bash test/test_editor_output.sh

# Quick test (skip GCC compilation)
test-quick:
	@echo "Running tests without GCC compilation..."
	@node test/test_generator_unit.js
	@node test/test_editor_integration.js
	@node test/test_escape_html.js
	@node test/test_xss_integration.js
	@echo "✅ Quick tests passed (compilation test skipped)"

# Clean generated files
clean:
	@echo "Cleaning generated files..."
	@rm -f hello hello_debug hello_clang *.exe *.out *.o
	@rm -f test/*.o test/hello test/hello.c
	@echo "✅ Clean complete"

# Help target
help:
	@echo "Hello-World Test Suite Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  make test          - Run all tests (default)"
	@echo "  make test-all      - Run all tests with detailed output"
	@echo "  make test-quick    - Run tests without GCC compilation"
	@echo "  make test-unit     - Run only unit tests (35 tests)"
	@echo "  make test-integration - Run only integration tests (22 tests)"
	@echo "  make test-escape   - Run only HTML escape tests (10 tests)"
	@echo "  make test-xss      - Run only XSS security tests (7 tests)"
	@echo "  make test-output   - Run only compilation smoke test"
	@echo "  make clean         - Remove generated files"
	@echo "  make help          - Show this help message"
	@echo ""
	@echo "Requirements:"
	@echo "  - Node.js (for running JavaScript tests)"
	@echo "  - GCC (optional, only for compilation smoke test)"
	@echo ""
	@echo "For more details, see TESTING.md"
