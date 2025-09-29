# Makefile for Hello World C program
# Provides standardized build targets for development and CI

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra
STRICT_CFLAGS = -Wall -Wextra -Wpedantic -Werror
DEBUG_CFLAGS = -g -Wall -Wextra
OPT_CFLAGS = -O2 -Wall -Wextra

# Target binary name
TARGET = hello
SOURCE = hello.c

# Default target
all: $(TARGET)

# Standard build
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Strict build (for CI and quality assurance)
strict: $(SOURCE)
	$(CC) $(STRICT_CFLAGS) -o $(TARGET) $(SOURCE)

# Debug build
debug: $(SOURCE)
	$(CC) $(DEBUG_CFLAGS) -o hello_debug $(SOURCE)

# Optimized build
optimized: $(SOURCE)
	$(CC) $(OPT_CFLAGS) -o $(TARGET) $(SOURCE)

# Alternative compiler (Clang)
clang: $(SOURCE)
	clang -o hello_clang $(SOURCE)

# Test target - runs validation
test: strict
	./test/validate.sh

# Quiet test for CI
test-quiet: strict
	./test/validate.sh --quiet

# Clean build artifacts
clean:
	rm -f $(TARGET) hello_debug hello_clang hello_strict *.exe *.out *.o

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Standard build (default)"
	@echo "  strict    - Build with strict warning flags (-Werror)"
	@echo "  debug     - Build with debug information"
	@echo "  optimized - Build with optimization (-O2)"
	@echo "  clang     - Build with Clang compiler"
	@echo "  test      - Run validation tests"
	@echo "  test-quiet- Run validation tests with minimal output"
	@echo "  clean     - Remove build artifacts"
	@echo "  help      - Show this help message"

.PHONY: all strict debug optimized clang test test-quiet clean help