# Makefile for Hello World C Program
# Provides build targets for normal compilation and unit testing

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c99
STRICT_FLAGS = -Wformat=2 -Wconversion -Wsign-conversion

# Default target
all: hello

# Build the main program
hello: hello.c hello.h
	$(CC) $(CFLAGS) -o hello hello.c

# Run unit tests
# This target compiles hello.c with -DUNIT_TEST to exclude main()
# and links it with the test runner
unit-test:
	@bash test/run_unit_tests.sh

# Clean build artifacts
clean:
	rm -f hello hello_lib.o test_hello_runner hello_clang hello_debug
	rm -f *.o *.exe *.out

.PHONY: all unit-test clean
