# Makefile for Hello World C Program
# Provides standardized build targets for development and CI

# Compiler settings
CC = gcc
CLANG = clang
SRC = hello.c
TARGET = hello
TARGET_DEBUG = hello_debug
TARGET_CLANG = hello_clang

# Compiler flags
CFLAGS_BASIC = -Wall -Wextra
CFLAGS_STRICT = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
CFLAGS_DEBUG = -g -Wall -Wextra
CFLAGS_OPTIMIZED = -O2 -Wall -Wextra

# Default target: basic build
.PHONY: all
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS_BASIC) -o $(TARGET) $(SRC)

# Strict build (CI-quality with -Werror)
.PHONY: strict
strict:
	$(CC) $(CFLAGS_STRICT) -o $(TARGET) $(SRC)

# Debug build
.PHONY: debug
debug:
	$(CC) $(CFLAGS_DEBUG) -o $(TARGET_DEBUG) $(SRC)

# Clang build (alternative compiler)
.PHONY: clang
clang:
	$(CLANG) $(CFLAGS_BASIC) -o $(TARGET_CLANG) $(SRC)

# Run validation tests
.PHONY: test
test: strict
	bash test/validate.sh

# Quiet test (minimal output for CI)
.PHONY: test-quiet
test-quiet: strict
	bash test/validate.sh --quiet

# Clean build artifacts
.PHONY: clean
clean:
	rm -f $(TARGET) $(TARGET_DEBUG) $(TARGET_CLANG) hello_strict hello_optimized hello_warnings *.exe *.out *.o

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all (default) - Build with basic warnings"
	@echo "  strict        - Build with strict flags (CI-quality)"
	@echo "  debug         - Build with debug symbols"
	@echo "  clang         - Build with Clang compiler"
	@echo "  test          - Run validation tests"
	@echo "  test-quiet    - Run tests with minimal output (CI)"
	@echo "  clean         - Remove build artifacts"
	@echo "  help          - Show this help message"
