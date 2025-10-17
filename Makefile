# Makefile for Hello-World project
# Provides targets for building and testing

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99

# Default target
all: hello

# Standard build
hello: hello.c hello.h
	$(CC) $(CFLAGS) -o hello hello.c

# Strict build (for CI)
strict: hello.c hello.h
	$(CC) $(STRICT_FLAGS) -o hello hello.c

# Unit test target (for CI)
unit-test:
	@./test/run_unit_tests.sh

# Clean build artifacts
clean:
	rm -f hello hello_debug hello_optimized hello_strict hello_clang hello_warnings
	rm -f test_hello_runner *.o test/*.o

.PHONY: all strict unit-test clean
