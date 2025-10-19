# Makefile for Hello World C program with plugin support

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99

# Default target - builds hello without plugins (original behavior)
all: hello

# Build hello without plugins (original behavior)
hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

# Build with strict flags (for validation)
strict: hello.c
	$(CC) $(STRICT_FLAGS) -o hello_strict hello.c

# Build hello with plugin system enabled (example with uppercase plugin)
hello_with_plugins: hello.c plugin.c plugins/uppercase.c
	$(CC) $(CFLAGS) -DUSE_PLUGINS -o hello_with_plugins hello.c plugin.c plugins/uppercase.c

# Build standalone example that uses plugins
example: hello_with_plugins.c plugin.c plugins/uppercase.c plugins/reverse.c
	$(CC) $(CFLAGS) -o hello_example hello_with_plugins.c plugin.c plugins/uppercase.c plugins/reverse.c

# Build with debug symbols
debug: hello.c
	$(CC) $(CFLAGS) -g -o hello_debug hello.c

# Build with Clang
clang: hello.c
	clang $(CFLAGS) -o hello_clang hello.c

# Run validation tests
test: strict
	./test/validate.sh

# Run validation tests with minimal output
test-quiet: strict
	./test/validate.sh --quiet

# Clean build artifacts
clean:
	rm -f hello hello_debug hello_optimized hello_strict hello_clang hello_warnings
	rm -f hello_with_plugins hello_example
	rm -f *.o *.exe *.out
	rm -f test/*.o test/*.exe test/*.out

.PHONY: all strict debug clang test test-quiet clean example
