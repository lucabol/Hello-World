# Makefile for Hello World C program
# 
# Environment requirements:
# - GCC compiler (available at /usr/bin/gcc)
# - Clang compiler (available at /usr/bin/clang) for 'clang' target
# - Bash shell for test execution
#
# Standard targets implemented:
# - default/all: Basic build with GCC
# - strict: Build with strict warning flags and -Werror
# - clang: Build with Clang compiler
# - debug: Build with debug symbols and warnings
# - test: Run validation script to verify program correctness
# - clean: Remove all compiled artifacts

# Default compiler and flags
CC = gcc
CLANG = clang
SOURCE = hello.c
TARGET = hello
TARGET_CLANG = hello_clang
TARGET_DEBUG = hello_debug

# Basic flags
CFLAGS = 
# Strict compilation flags matching validation script requirements
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
# Debug flags
DEBUG_FLAGS = -g -Wall -Wextra
# Clang flags
CLANG_FLAGS = -Wall -Wextra

# Default target (equivalent to 'make all')
.DEFAULT_GOAL := all

# Build basic hello program
all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Build with strict compilation flags (matches CI validation requirements)
strict: 
	$(CC) $(STRICT_FLAGS) -o $(TARGET) $(SOURCE)

# Build with Clang compiler
clang: $(TARGET_CLANG)

$(TARGET_CLANG): $(SOURCE)
	$(CLANG) $(CLANG_FLAGS) -o $(TARGET_CLANG) $(SOURCE)

# Build debug version
debug: $(TARGET_DEBUG)

$(TARGET_DEBUG): $(SOURCE)
	$(CC) $(DEBUG_FLAGS) -o $(TARGET_DEBUG) $(SOURCE)

# Run validation tests (execute test/validate.sh)
test:
	@echo "Running validation tests..."
	./test/validate.sh

# Quiet test mode for CI environments
test-quiet:
	@echo "Running validation tests (quiet mode)..."
	./test/validate.sh --quiet

# Clean all compiled artifacts
clean:
	rm -f $(TARGET) $(TARGET_CLANG) $(TARGET_DEBUG) hello_strict *.exe *.out *.o

# Phony targets (targets that don't create files with these names)
.PHONY: all strict clang debug test test-quiet clean

# Help target to show available commands
help:
	@echo "Available targets:"
	@echo "  all (default) - Build basic hello program with GCC"
	@echo "  strict        - Build with strict warning flags and -Werror"
	@echo "  clang         - Build with Clang compiler"
	@echo "  debug         - Build with debug symbols and warnings"
	@echo "  test          - Run validation script"
	@echo "  test-quiet    - Run validation script in quiet mode"
	@echo "  clean         - Remove all compiled artifacts"
	@echo "  help          - Show this help message"