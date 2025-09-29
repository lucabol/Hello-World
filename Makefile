# Makefile for Hello World C program
# Standard build automation for the hello.c program

# Compiler settings
CC = gcc
CLANG = clang
CFLAGS = -Wall -Wextra
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
DEBUG_FLAGS = -g -Wall -Wextra
OPTIMIZE_FLAGS = -O2 -Wall -Wextra

# Source files
SRC = hello.c
TARGET = hello
TARGET_DEBUG = hello_debug
TARGET_CLANG = hello_clang
TARGET_STRICT = hello_strict

# Default target
all: $(TARGET)

# Standard build
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Build alias (same as default)
build: $(TARGET)

# Strict build with all warnings as errors (for CI)
strict: $(SRC)
	$(CC) $(STRICT_FLAGS) -o $(TARGET_STRICT) $(SRC)

# Debug build
debug: $(SRC)
	$(CC) $(DEBUG_FLAGS) -o $(TARGET_DEBUG) $(SRC)

# Clang build
clang: $(SRC)
	$(CLANG) $(CFLAGS) -o $(TARGET_CLANG) $(SRC)

# Optimized build
optimized: $(SRC)
	$(CC) $(OPTIMIZE_FLAGS) -o $(TARGET) $(SRC)

# Test target - runs validation script
test: strict
	./test/validate.sh

# Quiet test for CI environments
test-quiet: strict
	./test/validate.sh --quiet

# Clean build artifacts
clean:
	rm -f $(TARGET) $(TARGET_DEBUG) $(TARGET_CLANG) $(TARGET_STRICT)
	rm -f *.exe *.out *.o

# Show help
help:
	@echo "Available targets:"
	@echo "  all        - Build the program (default)"
	@echo "  build      - Build the program"
	@echo "  strict     - Build with strict flags (warnings as errors)"
	@echo "  debug      - Build with debug symbols"
	@echo "  clang      - Build using clang compiler"
	@echo "  optimized  - Build with optimization"
	@echo "  test       - Run validation tests"
	@echo "  test-quiet - Run validation tests (minimal output)"
	@echo "  clean      - Remove build artifacts"
	@echo "  help       - Show this help message"

# Declare phony targets
.PHONY: all build strict debug clang optimized test test-quiet clean help