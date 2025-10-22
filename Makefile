# Makefile for hello.c and plugin system
# Provides convenient build targets for the project

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c99
SANITIZE_FLAGS = -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -g
PLUGIN_SOURCES = plugins/uppercase.c plugins/reverse.c plugins/prefix.c
PLUGIN_OBJECTS = $(PLUGIN_SOURCES:.c=.o)

.PHONY: all clean test test-edge test-all test-sanitizer demo help

# Default target
all: hello

# Build the original hello program
hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

# Build the plugin demo
plugin_demo: plugin_demo.c plugin.c $(PLUGIN_SOURCES)
	$(CC) $(CFLAGS) -o plugin_demo plugin_demo.c plugin.c $(PLUGIN_SOURCES)

# Run the plugin demo
demo: plugin_demo
	./plugin_demo

# Run basic plugin unit tests
test: test/run_plugin_tests.sh
	./test/run_plugin_tests.sh

# Build and run with sanitizers
test-sanitizer:
	@echo "Building tests with Address Sanitizer and Undefined Behavior Sanitizer..."
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) -I. -o test_plugin_runner_asan \
		test/test_plugin.c plugin.c $(PLUGIN_SOURCES)
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) -I. -o test_plugin_edge_runner_asan \
		test/test_plugin_edge_cases.c plugin.c $(PLUGIN_SOURCES)
	@echo "Running tests with sanitizers..."
	./test_plugin_runner_asan
	./test_plugin_edge_runner_asan
	@echo "✓ All sanitizer tests passed!"

# Run all tests (unit + edge + sanitizer)
test-all: test test-sanitizer

# Clean build artifacts
clean:
	rm -f hello plugin_demo test_plugin_runner test_plugin_edge_runner
	rm -f test_plugin_runner_asan test_plugin_edge_runner_asan
	rm -f *_asan *_ubsan
	rm -f *.o plugins/*.o
	rm -f hello_debug hello_optimized hello_strict hello_clang hello_warnings
	rm -f test_hello_runner
	rm -f *.exe *.out

# Help target
help:
	@echo "Available targets:"
	@echo "  make              - Build the original hello program"
	@echo "  make hello        - Build the original hello program"
	@echo "  make plugin_demo  - Build the plugin demonstration"
	@echo "  make demo         - Build and run the plugin demo"
	@echo "  make test         - Run plugin unit tests"
	@echo "  make test-sanitizer - Run tests with ASAN and UBSAN"
	@echo "  make test-all     - Run all tests (unit + edge + sanitizer)"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make help         - Show this help message"
