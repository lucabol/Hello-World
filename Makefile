# Makefile for Hello World with Plugin System
# 
# Targets:
#   all           - Build hello (default)
#   hello         - Build basic hello program with plugin support
#   demo          - Build and run plugin demonstration
#   hello-plugins - Build hello with example plugins
#   clean         - Remove all built files
#   test          - Run validation tests

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99

# Source files
PLUGIN_SOURCES = plugin.c
PLUGIN_EXAMPLES = plugins_examples.c
HELLO_SOURCE = hello.c

# Object files
PLUGIN_OBJS = plugin.o
PLUGIN_EXAMPLE_OBJS = plugins_examples.o

# Default target
all: hello

# Basic hello program with plugin system support
hello: $(HELLO_SOURCE) $(PLUGIN_SOURCES)
	$(CC) $(CFLAGS) -o hello $(HELLO_SOURCE) $(PLUGIN_SOURCES)

# Hello with example plugins pre-registered
hello-plugins: hello_with_plugins.c $(PLUGIN_SOURCES) $(PLUGIN_EXAMPLES)
	$(CC) $(CFLAGS) -o hello_with_plugins hello_with_plugins.c $(PLUGIN_SOURCES) $(PLUGIN_EXAMPLES)

# Plugin demonstration program
plugin_demo: plugin_demo.c $(PLUGIN_SOURCES) $(PLUGIN_EXAMPLES)
	$(CC) $(CFLAGS) -o plugin_demo plugin_demo.c $(PLUGIN_SOURCES) $(PLUGIN_EXAMPLES)

# Build and run demo
demo: plugin_demo hello-plugins
	@echo "===================================="
	@echo "Running Plugin System Demo"
	@echo "===================================="
	@./plugin_demo
	@echo ""
	@echo "===================================="
	@echo "Running Hello with Plugins"
	@echo "===================================="
	@./hello_with_plugins

# Strict build for CI/validation
strict: $(HELLO_SOURCE) $(PLUGIN_SOURCES)
	$(CC) $(STRICT_FLAGS) -o hello_strict $(HELLO_SOURCE) $(PLUGIN_SOURCES)

# Clean build artifacts
clean:
	rm -f hello hello_strict hello_with_plugins plugin_demo
	rm -f *.o *.exe *.out
	rm -f test_hello_runner hello_lib.o

# Run validation tests
test: strict
	@echo "Running validation tests..."
	@if [ -f test/validate.sh ]; then \
		./test/validate.sh; \
	else \
		echo "No validation script found"; \
		exit 1; \
	fi

# Help target
help:
	@echo "Available targets:"
	@echo "  all           - Build hello (default)"
	@echo "  hello         - Build basic hello program with plugin support"
	@echo "  demo          - Build and run plugin demonstration"
	@echo "  hello-plugins - Build hello with example plugins"
	@echo "  plugin_demo   - Build plugin demo program"
	@echo "  strict        - Build with strict compilation flags"
	@echo "  clean         - Remove all built files"
	@echo "  test          - Run validation tests"
	@echo "  help          - Show this help message"

.PHONY: all demo clean test strict help
