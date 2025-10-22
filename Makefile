# Makefile for Hello World C program
# Provides standard build targets for development and CI workflows

# Compiler settings
CC = gcc
CLANG = clang
CFLAGS_BASE = -Wall -Wextra
CFLAGS_STRICT = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
CFLAGS_DEBUG = -g -Wall -Wextra
CFLAGS_RELEASE = -O2 -Wall -Wextra

# Source files
SRC = hello.c
TEST_SRC = test/test_hello.c

# Binary targets
BIN = hello
BIN_STRICT = hello_strict
BIN_DEBUG = hello_debug
BIN_CLANG = hello_clang
TEST_RUNNER = test_hello_runner
LIB_OBJ = hello_lib.o

# Phony targets (not actual files)
.PHONY: all build strict debug clang unit-test test test-quiet clean help

# Default target - build release binary
all: build

build:
	@echo "Building release binary with optimizations..."
	$(CC) $(CFLAGS_RELEASE) -o $(BIN) $(SRC)
	@echo "✓ Build complete: $(BIN)"

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

# Compile and run unit tests
unit-test:
	@echo "Building and running unit tests..."
	@if [ -f test/run_unit_tests.sh ]; then \
		bash test/run_unit_tests.sh; \
	else \
		echo "Compiling unit tests..."; \
		$(CC) $(CFLAGS_STRICT) -I. -c -o $(LIB_OBJ) $(SRC) -DUNIT_TEST; \
		$(CC) $(CFLAGS_STRICT) -I. -o $(TEST_RUNNER) $(TEST_SRC) $(LIB_OBJ); \
		echo "Running unit tests..."; \
		./$(TEST_RUNNER); \
		rm -f $(LIB_OBJ) $(TEST_RUNNER); \
	fi

# Run all tests (validation script and unit tests)
test: unit-test
	@echo "Running validation tests..."
	@bash test/validate.sh

# Run tests in quiet mode (CI-friendly output)
test-quiet: unit-test
	@bash test/validate.sh --quiet

# Clean all build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@rm -f $(BIN) $(BIN_STRICT) $(BIN_DEBUG) $(BIN_CLANG)
	@rm -f $(TEST_RUNNER) $(LIB_OBJ)
	@rm -f *.exe *.out *.o *.obj
	@rm -f test/*.o test/*.exe test/*.out
	@rm -f hello_warnings hello_optimized hello_test* voice_demo voice_demo_test
	@echo "✓ Clean complete"

# Help target - display available targets
help:
	@echo "Available targets:"
	@echo "  make          - Build release binary (default)"
	@echo "  make build    - Build release binary with optimizations"
	@echo "  make strict   - Build with strict flags and -Werror (CI validation)"
	@echo "  make debug    - Build with debug symbols"
	@echo "  make clang    - Build using Clang compiler"
	@echo "  make unit-test - Compile and run unit tests"
	@echo "  make test     - Run all tests (unit tests + validation)"
	@echo "  make test-quiet - Run tests with minimal output (CI mode)"
	@echo "  make clean    - Remove all build artifacts"
	@echo "  make help     - Show this help message"
