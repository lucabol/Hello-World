# Makefile for Hello World and Code Metrics
# Provides standard build targets for development and CI workflows
#
# Usage examples:
#   make                    # Build all targets
#   make CC=clang          # Build with Clang
#   make CFLAGS="-O2"      # Override compiler flags
#   make code_metrics_strict  # Build with strict warnings

# Set shell explicitly for consistent behavior across systems
SHELL := /bin/bash

# Compiler settings (can be overridden via environment or command line)
CC ?= gcc
CLANG ?= clang
RM ?= rm -f
CFLAGS_BASE = -Wall -Wextra -std=c99
CFLAGS_STRICT = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
CFLAGS_DEBUG = -g -Wall -Wextra
CFLAGS_RELEASE = -O2 -Wall -Wextra

# Source files
SRC = hello.c
TEST_SRC = test/test_hello.c
CODE_METRICS_SRC = code_metrics.c

# Binary targets
BIN = hello
BIN_STRICT = hello_strict
BIN_DEBUG = hello_debug
BIN_CLANG = hello_clang
CODE_METRICS_BIN = code_metrics
TEST_RUNNER = test_hello_runner
LIB_OBJ = hello_lib.o

# Phony targets (not actual files)
.PHONY: all build strict debug clang code_metrics code_metrics_strict unit-test unit-test-quiet test test-quiet test-metrics test-all clean help

# Default target - build all binaries
all: build code_metrics

build:
	@echo "Building release binary with optimizations..."
	$(CC) $(CFLAGS_RELEASE) -o $(BIN) $(SRC)
	@echo "✓ Build complete: $(BIN)"

# Build main hello program for tests
hello: $(SRC)
	$(CC) $(CFLAGS_BASE) -o $(BIN) $(SRC)

# Strict build with all warnings as errors (for CI validation)
strict:
	@echo "Building with strict compilation flags..."
	$(CC) $(CFLAGS_STRICT) -o $(BIN_STRICT) $(SRC)
	@echo "✓ Strict build complete: $(BIN_STRICT)"

# Debug build with debug symbols
debug:
	@echo "Building debug binary with debug symbols..."
	$(CC) $(CFLAGS_DEBUG) -o $(BIN_DEBUG) $(SRC)
	@echo "✓ Debug build complete: $(BIN_DEBUG)"

# Build with Clang compiler
clang:
	@echo "Building with Clang compiler..."
	$(CLANG) $(CFLAGS_BASE) -o $(BIN_CLANG) $(SRC)
	@echo "✓ Clang build complete: $(BIN_CLANG)"

# Build code metrics analyzer
code_metrics: $(CODE_METRICS_SRC)
	@echo "Building code metrics analyzer..."
	$(CC) $(CFLAGS_BASE) -o $(CODE_METRICS_BIN) $(CODE_METRICS_SRC)
	@echo "✓ Code metrics build complete: $(CODE_METRICS_BIN)"

# Build code metrics with strict flags
code_metrics_strict: $(CODE_METRICS_SRC)
	@echo "Building code metrics analyzer with strict flags..."
	$(CC) $(CFLAGS_STRICT) -o $(CODE_METRICS_BIN) $(CODE_METRICS_SRC)
	@echo "✓ Code metrics strict build complete: $(CODE_METRICS_BIN)"

# Compile and run unit tests
unit-test: hello.h
	@echo "Building and running unit tests..."
	@if [ -f test/run_unit_tests.sh ]; then \
		bash test/run_unit_tests.sh; \
	else \
		echo "Compiling unit tests..."; \
		$(CC) $(CFLAGS_STRICT) -I. -DUNIT_TEST -c -o $(LIB_OBJ) $(SRC) || exit 1; \
		$(CC) $(CFLAGS_STRICT) -I. -o $(TEST_RUNNER) $(TEST_SRC) $(LIB_OBJ) || exit 1; \
		echo "Running unit tests..."; \
		./$(TEST_RUNNER) && $(RM) $(LIB_OBJ) $(TEST_RUNNER) || { echo "Tests failed - artifacts preserved for debugging"; exit 1; }; \
	fi

# Compile and run unit tests in quiet mode (minimal output)
unit-test-quiet: hello.h
	@if [ -f test/run_unit_tests.sh ]; then \
		bash test/run_unit_tests.sh 2>&1 | grep -E "(PASSED|FAILED|✓|✗|Error|error)" || true; \
	else \
		$(CC) $(CFLAGS_STRICT) -I. -DUNIT_TEST -c -o $(LIB_OBJ) $(SRC) 2>&1 || exit 1; \
		$(CC) $(CFLAGS_STRICT) -I. -o $(TEST_RUNNER) $(TEST_SRC) $(LIB_OBJ) 2>&1 || exit 1; \
		./$(TEST_RUNNER) 2>&1 | grep -E "(PASSED|FAILED|✓|✗)" && $(RM) $(LIB_OBJ) $(TEST_RUNNER) || { echo "Tests failed - artifacts preserved for debugging"; exit 1; }; \
	fi

# Run code_metrics tests
test-metrics: code_metrics
	@echo "Running code metrics tests..."
	@./tests/run_tests.sh

# Run all tests (validation script, unit tests, and code metrics tests)
test: build unit-test
	@echo "Running validation tests..."
	@bash test/validate.sh || exit 1

# Run tests in quiet mode (CI-friendly output)
test-quiet: build unit-test-quiet
	@bash test/validate.sh --quiet || exit 1

# Run all tests including code metrics
test-all: test test-metrics

# Clean all build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@$(RM) $(BIN) $(BIN_STRICT) $(BIN_DEBUG) $(BIN_CLANG)
	@$(RM) $(CODE_METRICS_BIN)
	@$(RM) $(TEST_RUNNER) $(LIB_OBJ)
	@$(RM) *.exe *.out *.o *.obj
	@$(RM) test/*.o test/*.exe test/*.out
	@echo "✓ Clean complete"

# Help target - display available targets
help:
	@echo "Available targets:"
	@echo "  make          - Build all binaries (default)"
	@echo "  make build    - Build hello binary with optimizations"
	@echo "  make strict   - Build with strict flags and -Werror (CI validation)"
	@echo "  make debug    - Build with debug symbols"
	@echo "  make clang    - Build using Clang compiler"
	@echo "  make code_metrics - Build code metrics analyzer"
	@echo "  make code_metrics_strict - Build code metrics with strict flags"
	@echo "  make unit-test - Compile and run unit tests"
	@echo "  make test     - Run all tests (unit tests + validation)"
	@echo "  make test-metrics - Run code metrics tests"
	@echo "  make test-all - Run all tests including code metrics"
	@echo "  make test-quiet - Run tests with minimal output (CI mode)"
	@echo "  make clean    - Remove all build artifacts"
	@echo "  make help     - Show this help message"
