# Makefile for Hello World with Plugin System
# Provides convenient build targets for various configurations

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c99

# Targets
.PHONY: all clean test test-plugins demo help

# Default target
all: hello

# Basic hello (no plugins)
hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

# Plugin-enabled versions
hello-plugin: hello_plugin_enabled.c plugin.c
	$(CC) $(CFLAGS) -o hello-plugin hello_plugin_enabled.c plugin.c

hello-uppercase: hello_plugin_enabled.c plugin.c plugins/example_uppercase.c
	$(CC) $(CFLAGS) -o hello-uppercase hello_plugin_enabled.c plugin.c plugins/example_uppercase.c -DLOAD_UPPERCASE

hello-reverse: hello_plugin_enabled.c plugin.c plugins/example_reverse.c
	$(CC) $(CFLAGS) -o hello-reverse hello_plugin_enabled.c plugin.c plugins/example_reverse.c -DLOAD_REVERSE

hello-all-plugins: hello_plugin_enabled.c plugin.c plugins/example_uppercase.c plugins/example_reverse.c
	$(CC) $(CFLAGS) -o hello-all-plugins hello_plugin_enabled.c plugin.c plugins/example_uppercase.c plugins/example_reverse.c -DLOAD_UPPERCASE -DLOAD_REVERSE

# Plugin demo
demo: plugin_demo.c plugin.c plugins/example_uppercase.c plugins/example_reverse.c
	$(CC) $(CFLAGS) -o plugin_demo plugin_demo.c plugin.c plugins/example_uppercase.c plugins/example_reverse.c

# Test targets
test: hello
	@./test/validate.sh

test-plugins: hello-plugin hello-uppercase hello-reverse hello-all-plugins
	@./test/test_plugin_system.sh

# Clean up
clean:
	rm -f hello hello-plugin hello-uppercase hello-reverse hello-all-plugins plugin_demo
	rm -f hello_strict hello_debug hello_optimized hello_clang
	rm -f test_hello_runner hello_lib.o
	rm -f *.o *.exe *.out

# Help message
help:
	@echo "Available targets:"
	@echo "  make                 - Build default hello (no plugins)"
	@echo "  make hello           - Build default hello (no plugins)"
	@echo "  make hello-plugin    - Build plugin-enabled version (no plugins loaded)"
	@echo "  make hello-uppercase - Build with uppercase plugin"
	@echo "  make hello-reverse   - Build with reverse plugin"
	@echo "  make hello-all-plugins - Build with all plugins"
	@echo "  make demo            - Build plugin demonstration program"
	@echo "  make test            - Run validation tests"
	@echo "  make test-plugins    - Run plugin system tests"
	@echo "  make clean           - Remove all build artifacts"
	@echo "  make help            - Show this help message"
