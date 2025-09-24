# Makefile for Hello World C program
# Provides standard build targets as documented in README

# Compiler settings
CC = gcc
CLANG = clang
SOURCE = hello.c

# Default target - optimized build
all: hello

hello: $(SOURCE)
	$(CC) -O2 -Wall -Wextra -o hello $(SOURCE)

# Strict compilation with error on warnings
strict: hello_strict

hello_strict: $(SOURCE)
	$(CC) -Wall -Wextra -Wpedantic -Werror -std=c99 -o hello_strict $(SOURCE)

# Clang build
clang: hello_clang

hello_clang: $(SOURCE)
	$(CLANG) -Wall -Wextra -o hello_clang $(SOURCE)

# Debug build
debug: hello_debug

hello_debug: $(SOURCE)
	$(CC) -g -Wall -Wextra -o hello_debug $(SOURCE)

# Test target - run validation script
test:
	bash test/validate.sh

# Clean build artifacts
clean:
	rm -f hello hello_strict hello_clang hello_debug *.exe *.out *.o

# Mark targets that don't create files
.PHONY: all strict clang debug test clean