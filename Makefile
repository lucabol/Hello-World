# Makefile for Hello World C Program with Code Coverage Support

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic
CFLAGS_STRICT = $(CFLAGS) -Werror
CFLAGS_COVERAGE = $(CFLAGS) --coverage -O0 -g

# Output binaries
TARGET = hello
TARGET_COVERAGE = hello_coverage

# Source files
SOURCES = hello.c

# Coverage tools
GCOV = gcov
LCOV = lcov
GENHTML = genhtml

# Coverage directories and files
COVERAGE_DIR = coverage
COVERAGE_INFO = $(COVERAGE_DIR)/coverage.info

# Default target
.PHONY: all
all: $(TARGET)

# Build standard binary
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

# Build with strict flags (for CI)
.PHONY: strict
strict:
	$(CC) $(CFLAGS_STRICT) -o $(TARGET) $(SOURCES)

# Build with Clang
.PHONY: clang
clang:
	clang $(CFLAGS) -o hello_clang $(SOURCES)

# Build debug version
.PHONY: debug
debug:
	$(CC) -g $(CFLAGS) -o hello_debug $(SOURCES)

# Run tests
.PHONY: test
test: strict
	@echo "Running validation tests..."
	@bash test/validate.sh

# Run tests quietly (for CI)
.PHONY: test-quiet
test-quiet: strict
	@bash test/validate.sh --quiet

# Unit test target (placeholder for CI compatibility)
.PHONY: unit-test
unit-test: test

# Code coverage target
.PHONY: coverage
coverage: clean-coverage
	@echo "Building with coverage instrumentation..."
	$(CC) $(CFLAGS_COVERAGE) -o $(TARGET_COVERAGE) $(SOURCES)
	@echo ""
	@echo "Running tests to generate coverage data..."
	@./$(TARGET_COVERAGE) > /dev/null
	@echo ""
	@echo "Generating coverage reports..."
	@mkdir -p $(COVERAGE_DIR)
	@echo ""
	@echo "Coverage summary:"
	@$(GCOV) $(TARGET_COVERAGE)-hello.gcda 2>&1 | grep -E "(File '|Lines executed|Creating)"
	@echo ""
	@if command -v $(LCOV) >/dev/null 2>&1 && command -v $(GENHTML) >/dev/null 2>&1; then \
		echo "Generating HTML coverage report..."; \
		$(LCOV) --capture --directory . --output-file $(COVERAGE_INFO) --rc lcov_branch_coverage=1 2>/dev/null || \
		$(LCOV) --capture --directory . --output-file $(COVERAGE_INFO) 2>/dev/null; \
		$(LCOV) --remove $(COVERAGE_INFO) '/usr/*' --output-file $(COVERAGE_INFO) 2>/dev/null; \
		$(GENHTML) $(COVERAGE_INFO) --output-directory $(COVERAGE_DIR) --title "Hello World Coverage Report" --legend 2>/dev/null; \
		echo ""; \
		echo "✓ HTML coverage report generated: $(COVERAGE_DIR)/index.html"; \
		echo ""; \
		echo "Coverage Statistics:"; \
		$(LCOV) --summary $(COVERAGE_INFO) 2>&1 | grep -E "(lines\.\.\.|functions\.\.\.)"; \
	else \
		echo "lcov/genhtml not found. Skipping HTML report generation."; \
		echo "Install with: sudo apt-get install lcov"; \
	fi
	@echo ""
	@find . -maxdepth 1 -name '*.gcov' -exec mv {} $(COVERAGE_DIR)/ \; 2>/dev/null || true

# Clean coverage artifacts
.PHONY: clean-coverage
clean-coverage:
	@rm -rf $(COVERAGE_DIR)
	@rm -f *.gcda *.gcno *.gcov
	@rm -f $(TARGET_COVERAGE)

# Clean all build artifacts
.PHONY: clean
clean: clean-coverage
	@rm -f $(TARGET) hello_debug hello_clang hello_strict hello_warnings hello_optimized
	@rm -f *.o *.exe *.out
	@echo "Clean complete"

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all            - Build the hello program (default)"
	@echo "  strict         - Build with strict warnings (CI mode)"
	@echo "  clang          - Build with Clang compiler"
	@echo "  debug          - Build debug version"
	@echo "  test           - Run validation tests"
	@echo "  test-quiet     - Run validation tests (minimal output)"
	@echo "  unit-test      - Alias for test (CI compatibility)"
	@echo "  coverage       - Generate code coverage reports"
	@echo "  clean          - Remove all build artifacts"
	@echo "  clean-coverage - Remove only coverage artifacts"
	@echo "  help           - Show this help message"
