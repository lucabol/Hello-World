# Makefile for hello.c with plugin system
# Provides targets for building with and without plugins

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99

# Default target - build without plugins (original behavior)
all: hello

# Build hello without any plugins (original behavior)
# hello.c now includes plugin system inline
hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

# Build with strict flags (for CI validation)
strict: hello.c
	$(CC) $(STRICT_FLAGS) -o hello hello.c

# Build with example plugins - demonstrates plugin usage
hello-with-uppercase: hello_with_plugins.c plugin_loader.c example_plugins.c plugin.h example_plugins.h
	$(CC) $(CFLAGS) -o hello-with-uppercase hello_with_plugins.c plugin_loader.c example_plugins.c

# Build with reverse plugin example
hello-with-reverse: hello_with_reverse.c plugin_loader.c example_plugins.c plugin.h example_plugins.h
	$(CC) $(CFLAGS) -o hello-with-reverse hello_with_reverse.c plugin_loader.c example_plugins.c

# Build with chained plugins example
hello-with-chain: hello_with_chain.c plugin_loader.c example_plugins.c plugin.h example_plugins.h
	$(CC) $(CFLAGS) -o hello-with-chain hello_with_chain.c plugin_loader.c example_plugins.c

# Build all plugin examples
examples: hello-with-uppercase hello-with-reverse hello-with-chain

# Run validation tests
test: hello
	./test/validate.sh

# Run validation tests in quiet mode (for CI)
test-quiet: hello
	./test/validate.sh --quiet

# Run unit tests (if available)
unit-test:
	@echo "Unit tests not yet implemented for plugin system"
	@exit 0

# Clean build artifacts
clean:
	rm -f hello hello-with-uppercase hello-with-reverse hello-with-chain
	rm -f hello_strict hello_debug hello_clang
	rm -f *.o *.exe *.out

# Debug build
debug: hello.c
	$(CC) -g $(CFLAGS) -o hello_debug hello.c

# Clang build
clang: hello.c
	clang $(CFLAGS) -o hello_clang hello.c

.PHONY: all strict test test-quiet unit-test clean debug clang hello-with-uppercase hello-with-reverse hello-with-chain examples
