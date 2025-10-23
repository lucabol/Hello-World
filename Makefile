# Makefile for Hello World C program
# Provides standardized build targets for development and CI

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c99
STRICT_FLAGS = -Wformat=2 -Wconversion -Wsign-conversion
BINARY = hello
CLANG_BINARY = hello_clang
DEBUG_BINARY = hello_debug
TEST_RUNNER = test_hello_runner

.PHONY: all clean test unit-test strict debug clang test-quiet

# Default target - build with standard flags
all: $(BINARY)

$(BINARY): hello.c hello.h
	$(CC) $(CFLAGS) -o $(BINARY) hello.c

# Strict compilation (CI-like with extra warnings)
strict: hello.c hello.h
	$(CC) $(CFLAGS) $(STRICT_FLAGS) -o $(BINARY) hello.c

# Debug build with symbols
debug: hello.c hello.h
	$(CC) -g $(CFLAGS) -o $(DEBUG_BINARY) hello.c

# Build with Clang
clang: hello.c hello.h
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
