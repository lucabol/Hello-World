# Makefile for Hello World C program
# Provides standardized build targets with different levels of warnings

CC = gcc
CFLAGS = -std=c99
TARGET = hello
SOURCE = hello.c

# Default target - basic build
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Standard build with warnings (recommended for development)
.PHONY: build
build: $(SOURCE)
	$(CC) $(CFLAGS) -Wall -Wextra -o $(TARGET) $(SOURCE)

# Strict build with pedantic warnings and error enforcement (matches CI)
.PHONY: strict
strict: $(SOURCE)
	$(CC) $(CFLAGS) -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -o $(TARGET) $(SOURCE)

# Debug build with debugging symbols
.PHONY: debug
debug: $(SOURCE)
	$(CC) $(CFLAGS) -g -Wall -Wextra -o hello_debug $(SOURCE)

# Clang build (alternative compiler)
.PHONY: clang
clang: $(SOURCE)
	clang $(CFLAGS) -Wall -Wextra -o hello_clang $(SOURCE)

# Run validation tests (matches CI validation)
.PHONY: test
test: strict
	./test/validate.sh

# Quiet test output (CI-style)
.PHONY: test-quiet
test-quiet: strict
	./test/validate.sh --quiet

# Clean compiled binaries
.PHONY: clean
clean:
	rm -f $(TARGET) hello_debug hello_clang hello_strict *.exe *.out *.o

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  $(TARGET)     - Basic build (default)"
	@echo "  build      - Build with standard warnings (-Wall -Wextra)"
	@echo "  strict     - Strict build with pedantic warnings and -Werror"
	@echo "  debug      - Debug build with symbols"
	@echo "  clang      - Build with Clang compiler"
	@echo "  test       - Build strict and run validation tests"
	@echo "  test-quiet - Run tests with minimal output (CI-style)"
	@echo "  clean      - Remove compiled binaries"
	@echo "  help       - Show this help message"