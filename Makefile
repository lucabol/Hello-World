# Makefile for Hello World C program

.PHONY: all clean unit-test strict test

# Default target - build the hello program
all: hello

# Build the hello program
hello: hello.c
	gcc -o hello hello.c

# Build with strict warnings (for CI quality checks)
strict: hello.c
	gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -o hello hello.c

# Run unit tests
unit-test:
	@bash test/run_unit_tests.sh

# Run all tests (validation script)
test:
	@bash test/validate.sh

# Clean up compiled binaries
clean:
	rm -f hello hello_strict hello_debug hello_clang *.o test_hello_runner
