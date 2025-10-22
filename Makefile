# Makefile for Hello World and Code Metrics
# Simple build system for C programs

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99

# Targets
all: hello code_metrics

# Build main hello program
hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

# Build code metrics analyzer
code_metrics: code_metrics.c
	$(CC) $(CFLAGS) -o code_metrics code_metrics.c

# Build with strict flags for CI
strict: hello.c
	$(CC) $(STRICT_FLAGS) -o hello_strict hello.c

# Build with clang
clang: hello.c
	clang -o hello_clang hello.c

# Build debug version
debug: hello.c
	$(CC) -g $(CFLAGS) -o hello_debug hello.c

# Run tests
test: strict
	./test/validate.sh

# Run tests quietly (for CI)
test-quiet: strict
	./test/validate.sh --quiet

# Clean up compiled files
clean:
	rm -f hello hello_debug hello_optimized hello_strict hello_clang hello_warnings
	rm -f code_metrics
	rm -f *.o *.exe *.out
	rm -f test_hello_runner

# Phony targets
.PHONY: all strict clang debug test test-quiet clean
