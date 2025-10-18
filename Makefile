# Makefile for Hello World C program
# Provides standardized build targets for development and CI

# Compiler settings
CC = gcc
CLANG = clang
CFLAGS_BASE = -std=c99
CFLAGS_WARNINGS = -Wall -Wextra -Wpedantic
CFLAGS_EXTRA = -Wformat=2 -Wconversion -Wsign-conversion
CFLAGS_STRICT = $(CFLAGS_BASE) $(CFLAGS_WARNINGS) $(CFLAGS_EXTRA) -Werror
CFLAGS_NORMAL = $(CFLAGS_BASE) -Wall -Wextra -O2
CFLAGS_DEBUG = $(CFLAGS_BASE) -Wall -Wextra -g

# Source files
SRC = hello.c
HEADER = hello.h
TEST_SRC = test/test_hello.c
TEST_RUNNER = test_hello_runner

# Binary names
BIN = hello
BIN_STRICT = hello_strict
BIN_CLANG = hello_clang
BIN_DEBUG = hello_debug
OBJ_LIB = hello_lib.o

# Default target: build optimized binary with warnings
.PHONY: all
all: $(BIN)

$(BIN): $(SRC) $(HEADER)
	@echo "Building $(BIN) with optimization flags..."
	$(CC) $(CFLAGS_NORMAL) -o $(BIN) $(SRC)

# Strict compilation target (used by CI)
# Compiles with strict flags including -Werror to catch all warnings
.PHONY: strict
strict: $(BIN_STRICT)

$(BIN_STRICT): $(SRC) $(HEADER)
	@echo "Building $(BIN_STRICT) with strict compilation flags..."
	$(CC) $(CFLAGS_STRICT) -o $(BIN_STRICT) $(SRC)

# Clang build target
# Builds using Clang compiler for compatibility testing
.PHONY: clang
clang: $(BIN_CLANG)

$(BIN_CLANG): $(SRC) $(HEADER)
	@echo "Building $(BIN_CLANG) with Clang compiler..."
	$(CLANG) -Wall -Wextra -o $(BIN_CLANG) $(SRC)

# Debug build target
# Includes debug symbols for use with debuggers (gdb, lldb)
.PHONY: debug
debug: $(BIN_DEBUG)

$(BIN_DEBUG): $(SRC) $(HEADER)
	@echo "Building $(BIN_DEBUG) with debug symbols..."
	$(CC) $(CFLAGS_DEBUG) -o $(BIN_DEBUG) $(SRC)

# Unit test target
# Builds and runs the unit test suite
# Uses UNIT_TEST define to exclude main() from hello.c
.PHONY: unit-test
unit-test:
	@echo "Running unit tests..."
	@bash test/run_unit_tests.sh || { \
		echo ""; \
		echo "Unit tests failed!"; \
		echo "To reproduce locally: make unit-test"; \
		echo "To run tests directly: bash test/run_unit_tests.sh"; \
		exit 1; \
	}

# Validation test target
# Runs the validation script to ensure correct output format
.PHONY: test
test: unit-test
	@echo "Running validation tests..."
	@bash test/validate.sh --quiet || { \
		echo ""; \
		echo "Validation tests failed!"; \
		echo "To reproduce locally: make test"; \
		echo "To run validation directly: bash test/validate.sh"; \
		exit 1; \
	}
	@echo "All tests passed successfully!"

# Alternative quiet test target for CI environments
.PHONY: test-quiet
test-quiet:
	@bash test/run_unit_tests.sh 2>&1 | grep -E "(PASSED|FAILED|✓|✗)" || true
	@bash test/validate.sh --quiet

# Clean target
# Removes all compiled binaries and temporary files
.PHONY: clean
clean:
	@echo "Cleaning up build artifacts..."
	@rm -f $(BIN) $(BIN_STRICT) $(BIN_CLANG) $(BIN_DEBUG)
	@rm -f $(OBJ_LIB) $(TEST_RUNNER)
	@rm -f *.o *.out *.exe
	@echo "Clean complete."

# Help target
# Displays available make targets and their descriptions
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  make          - Build optimized hello binary (default)"
	@echo "  make all      - Same as make"
	@echo "  make strict   - Build with strict flags (CI quality check)"
	@echo "  make clang    - Build with Clang compiler"
	@echo "  make debug    - Build with debug symbols"
	@echo "  make unit-test - Run unit tests"
	@echo "  make test     - Run all tests (unit + validation)"
	@echo "  make clean    - Remove all build artifacts"
	@echo "  make help     - Show this help message"
