# Makefile for hello.c and plugin system
# Provides convenient build targets for the project

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c99
PLUGIN_SOURCES = plugins/uppercase.c plugins/reverse.c plugins/prefix.c
PLUGIN_OBJECTS = $(PLUGIN_SOURCES:.c=.o)

.PHONY: all clean test demo help

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

# Run plugin unit tests
test: test/run_plugin_tests.sh
	./test/run_plugin_tests.sh

# Clean build artifacts
clean:
	rm -f hello plugin_demo test_plugin_runner
	rm -f *.o plugins/*.o
	rm -f hello_debug hello_optimized hello_strict hello_clang hello_warnings
	rm -f test_hello_runner
	rm -f *.exe *.out

# Help target
help:
	@echo "Available targets:"
	@echo "  make           - Build the original hello program"
	@echo "  make hello     - Build the original hello program"
	@echo "  make plugin_demo - Build the plugin demonstration"
	@echo "  make demo      - Build and run the plugin demo"
	@echo "  make test      - Run plugin unit tests"
	@echo "  make clean     - Remove build artifacts"
	@echo "  make help      - Show this help message"
