# Makefile for Hello World C program
# Provides targets for building, testing, and cleaning

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
DEBUG_FLAGS = -g -Wall -Wextra -std=c99
OPTIMIZE_FLAGS = -O2 -Wall -Wextra -std=c99

# Binaries
HELLO_BIN = hello
HELLO_DEBUG = hello_debug
HELLO_CLANG = hello_clang
TEST_BIN = test/test_hello

# Source files
HELLO_SRC = hello.c
TEST_SRC = test/test_hello.c

# Default target
.PHONY: all
all: $(HELLO_BIN)

# Build hello program
$(HELLO_BIN): $(HELLO_SRC)
	$(CC) $(CFLAGS) -o $@ $<

# Build with debug symbols
.PHONY: debug
debug: $(HELLO_SRC)
	$(CC) $(DEBUG_FLAGS) -o $(HELLO_DEBUG) $<

# Build with optimizations
.PHONY: optimize
optimize: $(HELLO_SRC)
	$(CC) $(OPTIMIZE_FLAGS) -o $(HELLO_BIN) $<

# Build with strict warnings (matches CI)
.PHONY: strict
strict: $(HELLO_SRC)
	$(CC) $(STRICT_FLAGS) -o $(HELLO_BIN) $<

# Build with Clang compiler
.PHONY: clang
clang: $(HELLO_SRC)
	clang $(CFLAGS) -o $(HELLO_CLANG) $<

# Build unit tests
$(TEST_BIN): $(TEST_SRC) test/minunit.h $(HELLO_SRC)
	$(CC) $(CFLAGS) -o $@ $(TEST_SRC)

# Run unit tests (C testing framework)
.PHONY: test-unit
test-unit: $(TEST_BIN)
	./$(TEST_BIN)

# Run validation script (shell-based tests)
.PHONY: test-validate
test-validate: $(HELLO_BIN)
	bash test/validate.sh

# Run validation script in quiet mode (CI-style)
.PHONY: test-quiet
test-quiet: $(HELLO_BIN)
	bash test/validate.sh --quiet

# Run all tests
.PHONY: test
test: test-unit test-validate
	@echo ""
	@echo "All tests completed successfully!"

# Clean build artifacts
.PHONY: clean
clean:
	rm -f $(HELLO_BIN) $(HELLO_DEBUG) $(HELLO_CLANG) $(TEST_BIN)
	rm -f *.exe *.out *.o hello_test hello_strict hello_strict_test

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all          - Build hello program (default)"
	@echo "  debug        - Build with debug symbols"
	@echo "  optimize     - Build with optimizations"
	@echo "  strict       - Build with strict warnings (CI-like)"
	@echo "  clang        - Build with Clang compiler"
	@echo "  test-unit    - Run C unit tests"
	@echo "  test-validate - Run shell validation tests"
	@echo "  test-quiet   - Run validation in quiet mode"
	@echo "  test         - Run all tests (unit + validation)"
	@echo "  clean        - Remove build artifacts"
	@echo "  help         - Show this help message"
