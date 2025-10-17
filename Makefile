# Makefile for Hello World project
# Supports building the main program and running unit tests

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
INCLUDES = -I.

# Default target
.PHONY: all
all: hello

# Build the main program
hello: hello.c hello.h
	$(CC) -Wall -Wextra -o hello hello.c

# Unit test target for CI
.PHONY: unit-test
unit-test:
	@echo "Building and running unit tests..."
	@bash test/run_unit_tests.sh

# Clean build artifacts
.PHONY: clean
clean:
	rm -f hello hello_lib.o simple_test.o test_hello_runner *.exe *.out *.o
