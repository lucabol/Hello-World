# Makefile for Hello World C program
# Provides standardized build targets for development and CI

CC = gcc
CFLAGS = -Wall -Wextra
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
BINARY = hello
CLANG_BINARY = hello_clang
DEBUG_BINARY = hello_debug
TEST_RUNNER = test_hello_runner

.PHONY: all clean test unit-test strict debug clang test-quiet

# Default target - build with standard warnings
all: $(BINARY)

$(BINARY): hello.c
	$(CC) $(CFLAGS) -o $(BINARY) hello.c

# Strict compilation (CI-like with -Werror)
strict: hello.c
	$(CC) $(STRICT_FLAGS) -o $(BINARY) hello.c

# Debug build with symbols
debug: hello.c
	$(CC) -g $(CFLAGS) -o $(DEBUG_BINARY) hello.c

# Build with Clang
clang: hello.c
	clang $(CFLAGS) -o $(CLANG_BINARY) hello.c

# Run unit tests
unit-test:
	@bash test/run_unit_tests.sh

# Run validation tests (verbose)
test: strict
	@bash test/validate.sh

# Run validation tests (quiet mode for CI)
test-quiet: strict
	@bash test/validate.sh --quiet

# Clean compiled binaries
clean:
	rm -f $(BINARY) $(DEBUG_BINARY) $(CLANG_BINARY) $(TEST_RUNNER) *.o *.exe *.out
