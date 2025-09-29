# Simple Makefile for Hello World C program
# Provides targets for building, testing, and validation

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
STRICT_CFLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99

# Default target
all: hello

# Build the main program
hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

# Build with strict warnings (CI-like)
strict: hello.c
	$(CC) $(STRICT_CFLAGS) -o hello hello.c

# Build debug version
debug: hello.c
	$(CC) $(CFLAGS) -g -o hello_debug hello.c

# Build with clang
clang: hello.c
	clang $(CFLAGS) -o hello_clang hello.c

# Build the C unit test
test_hello: test/test_hello.c hello
	$(CC) $(CFLAGS) -o test_hello test/test_hello.c

# Run C unit tests
test-c: test_hello
	./test_hello

# Run shell validation (existing)
test-shell: hello
	./test/validate.sh

# Run both test suites
test: test-c test-shell

# Run tests with minimal output (CI-friendly)
test-quiet: hello test_hello
	@./test_hello
	@./test/validate.sh --quiet

# Clean compiled files
clean:
	rm -f hello hello_debug hello_strict hello_clang hello_warnings hello_test* test_hello voice_demo voice_demo_test *.exe *.out *.o

# Phony targets
.PHONY: all strict debug clang test-c test-shell test test-quiet clean