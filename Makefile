# Makefile for Hello World C program

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99

.PHONY: all clean unit-test strict test test-quiet debug clang

# Default target
all: hello

# Build the main program
hello: hello.c
	$(CC) -Wall -Wextra -o hello hello.c

# Strict build (matches CI validation)
strict: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

# Debug build
debug: hello.c
	$(CC) -g -Wall -Wextra -o hello_debug hello.c

# Clang build
clang: hello.c
	clang -o hello_clang hello.c

# Build and run unit tests
unit-test:
	@echo "Building unit tests..."
	@$(CC) $(CFLAGS) -I. -c -o hello_lib.o hello.c -DUNIT_TEST
	@$(CC) $(CFLAGS) -I. -o test_hello_runner test/test_hello.c hello_lib.o
	@echo "Running unit tests..."
	@./test_hello_runner
	@rm -f hello_lib.o test_hello_runner

# Run validation tests (same as CI)
test: strict
	@bash test/validate.sh

# Run validation tests in quiet mode (CI-style)
test-quiet: strict
	@bash test/validate.sh --quiet

# Clean build artifacts
clean:
	rm -f hello hello_debug hello_clang hello_strict *.exe *.out *.o test_hello_runner hello_lib.o
