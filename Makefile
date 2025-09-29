# Makefile for Hello World C program
# Simple build configuration with multiple targets

CC = gcc
CLANG = clang
CFLAGS = -Wall -Wextra
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
DEBUG_FLAGS = -g -Wall -Wextra
OPTIMIZED_FLAGS = -O2 -Wall -Wextra

SRCFILE = hello.c
TARGET = hello

# Default target
all: $(TARGET)

# Standard build
$(TARGET): $(SRCFILE)
	$(CC) -o $@ $<

# Development build with warnings
dev: $(SRCFILE)
	$(CC) $(CFLAGS) -o $(TARGET) $<

# Strict build (CI-like) with warnings as errors
strict: $(SRCFILE)
	$(CC) $(STRICT_FLAGS) -o $(TARGET) $<

# Debug build
debug: $(SRCFILE)
	$(CC) $(DEBUG_FLAGS) -o hello_debug $<

# Optimized build
optimized: $(SRCFILE)
	$(CC) $(OPTIMIZED_FLAGS) -o hello_optimized $<

# Build with Clang
clang: $(SRCFILE)
	$(CLANG) -o hello_clang $<

# Test target - runs validation script
test: strict
	./test/validate.sh

# Test with quiet output (CI-style)
test-quiet: strict
	./test/validate.sh --quiet

# Clean compiled binaries
clean:
	rm -f $(TARGET) hello_debug hello_optimized hello_clang hello_strict *.exe *.out *.o

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build the program (default)"
	@echo "  dev        - Build with development warnings"
	@echo "  strict     - Build with strict flags (CI-like)"
	@echo "  debug      - Build debug version"
	@echo "  optimized  - Build optimized version"
	@echo "  clang      - Build with Clang compiler"
	@echo "  test       - Run validation tests"
	@echo "  test-quiet - Run tests with minimal output"
	@echo "  clean      - Remove compiled binaries"
	@echo "  help       - Show this help message"

.PHONY: all dev strict debug optimized clang test test-quiet clean help