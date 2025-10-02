# Makefile for Hello World C program
# Automates build targets and testing workflow
#
# Available targets:
#   all (default) - Build the program with standard flags
#   strict        - Build with strict compilation flags (CI-compatible)
#   debug         - Build with debug symbols
#   clang         - Build using Clang compiler
#   optimized     - Build with optimization flags
#   clean         - Remove all compiled binaries
#   test          - Run validation tests
#   test-quiet    - Run validation tests with minimal output (CI-style)
#   help          - Show detailed help message

# Set default goal explicitly for portability
.DEFAULT_GOAL := all

# Compiler settings
CC = gcc
CLANG = clang
CFLAGS = -Wall -Wextra

# C standard version (can be overridden, e.g., make STD=-std=c11)
# Using c99 for maximum compatibility with the validation script
STD ?= -std=c99

# Build flag configurations
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror $(STD)
DEBUG_FLAGS = -g -Wall -Wextra
OPTIMIZE_FLAGS = -O2 -Wall -Wextra

# Source and target files
SRC = hello.c
TARGET = hello
TARGET_DEBUG = hello_debug
TARGET_STRICT = hello_strict
TARGET_CLANG = hello_clang
TARGET_OPTIMIZE = hello_optimized

# Declare all phony targets
.PHONY: all strict debug clang optimized clean test test-quiet help

# Default target - standard build
all: $(TARGET)

# Standard build
$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC)

# Strict build (CI-compatible with strict warnings and -Werror)
strict: $(SRC)
	$(CC) $(STRICT_FLAGS) -o $(TARGET_STRICT) $(SRC)

# Debug build
debug: $(SRC)
	$(CC) $(DEBUG_FLAGS) -o $(TARGET_DEBUG) $(SRC)

# Clang build (using same flags as standard build for consistency)
clang: $(SRC)
	$(CLANG) $(CFLAGS) -o $(TARGET_CLANG) $(SRC)

# Optimized build
optimized: $(SRC)
	$(CC) $(OPTIMIZE_FLAGS) -o $(TARGET_OPTIMIZE) $(SRC)

# Clean build artifacts
clean:
	rm -f $(TARGET) $(TARGET_DEBUG) $(TARGET_STRICT) $(TARGET_CLANG) $(TARGET_OPTIMIZE)
	rm -f *.exe *.out *.o

# Run validation tests (checks for script existence first)
test: strict
	@if [ -x ./test/validate.sh ]; then \
		./test/validate.sh; \
	else \
		echo "ERROR: ./test/validate.sh missing or not executable"; \
		exit 1; \
	fi

# Run tests with quiet output (CI-style)
test-quiet: strict
	@if [ -x ./test/validate.sh ]; then \
		./test/validate.sh --quiet; \
	else \
		echo "ERROR: ./test/validate.sh missing or not executable"; \
		exit 1; \
	fi

# Help target - shows available targets and descriptions
help:
	@echo "Available targets:"
	@echo "  all        - Build the program (default)"
	@echo "  strict     - Build with strict compilation flags (CI-compatible)"
	@echo "  debug      - Build with debug symbols"
	@echo "  clang      - Build using Clang compiler"
	@echo "  optimized  - Build with optimization flags"
	@echo "  clean      - Remove all compiled binaries"
	@echo "  test       - Run validation tests"
	@echo "  test-quiet - Run validation tests with minimal output"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Build artifacts created:"
	@echo "  hello, hello_debug, hello_strict, hello_clang, hello_optimized"
	@echo ""
	@echo "Override options:"
	@echo "  make STD=-std=c11  - Use a different C standard"

