# Makefile for Hello World C Program
# Simple build system with support for unit testing

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99

# Default target: build the main program
.PHONY: all
all: hello

# Build the main hello program
hello: hello.c hello.h
	$(CC) $(CFLAGS) -o hello hello.c

# Build with strict flags (for CI validation)
# Note: Uses -Werror which may fail with warnings on some compiler versions
.PHONY: strict
strict:
	$(CC) $(STRICT_FLAGS) -o hello_strict hello.c

# Build with Clang
.PHONY: clang
clang:
	clang $(CFLAGS) -o hello_clang hello.c

# Build with debug symbols
.PHONY: debug
debug:
	$(CC) $(CFLAGS) -g -o hello_debug hello.c

# Run unit tests
# Note: Unit tests compile hello.c with -DUNIT_TEST, which excludes main()
#       and allows tests to link against get_greeting() without symbol conflicts
.PHONY: unit-test
unit-test:
	@./test/run_unit_tests.sh

# Run validation tests
.PHONY: test
test:
	@./test/validate.sh

# Run validation tests quietly (for CI)
.PHONY: test-quiet
test-quiet:
	@./test/validate.sh --quiet

# Clean build artifacts
.PHONY: clean
clean:
	rm -f hello hello_strict hello_clang hello_debug *.o *.exe *.out test_hello_runner hello_lib.o

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all         - Build the main program (default)"
	@echo "  strict      - Build with strict compilation flags"
	@echo "  clang       - Build with Clang compiler"
	@echo "  debug       - Build with debug symbols"
	@echo "  unit-test   - Run unit tests"
	@echo "  test        - Run validation tests"
	@echo "  test-quiet  - Run validation tests quietly (for CI)"
	@echo "  clean       - Remove build artifacts"
	@echo "  help        - Show this help message"
