# Makefile for Hello World with Plugin System
# 
# Targets:
#   all           - Build hello (default)
#   hello         - Build basic hello program with plugin support
#   demo          - Build and run plugin demonstration
#   hello-plugins - Build hello with example plugins
#   unit-test     - Build and run unit tests
#   clean         - Remove all built files
#   test          - Run all tests (unit + validation)
#   ci            - Run CI build (strict compilation + all tests)

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
ASAN_FLAGS = -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer

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

# Custom plugin example
custom_plugin_example: custom_plugin_example.c $(PLUGIN_SOURCES)
	$(CC) $(CFLAGS) -o custom_plugin_example custom_plugin_example.c $(PLUGIN_SOURCES)

# Unit tests
test_plugin_unit: test/test_plugin_unit.c $(PLUGIN_SOURCES) $(PLUGIN_EXAMPLES)
	$(CC) $(STRICT_FLAGS) -I. -o test_plugin_unit test/test_plugin_unit.c $(PLUGIN_SOURCES) $(PLUGIN_EXAMPLES)

# Build and run unit tests
unit-test: test_plugin_unit
	@echo "===================================="
	@echo "Running Unit Tests"
	@echo "===================================="
	@./test_plugin_unit

# Build and run demo
demo: plugin_demo hello-plugins custom_plugin_example
	@echo "===================================="
	@echo "Running Plugin System Demo"
	@echo "===================================="
	@./plugin_demo
	@echo ""
	@echo "===================================="
	@echo "Running Hello with Plugins"
	@echo "===================================="
	@./hello_with_plugins
	@echo ""
	@echo "===================================="
	@echo "Running Custom Plugin Example"
	@echo "===================================="
	@./custom_plugin_example

# Strict build for CI/validation
strict: $(HELLO_SOURCE) $(PLUGIN_SOURCES)
	$(CC) $(STRICT_FLAGS) -o hello_strict $(HELLO_SOURCE) $(PLUGIN_SOURCES)

# ASAN build for memory safety testing
asan: test/test_plugin_unit.c $(PLUGIN_SOURCES) $(PLUGIN_EXAMPLES)
	$(CC) $(CFLAGS) $(ASAN_FLAGS) -I. -o test_plugin_unit_asan test/test_plugin_unit.c $(PLUGIN_SOURCES) $(PLUGIN_EXAMPLES)
	@echo "Running tests with ASAN/UBSAN..."
	@./test_plugin_unit_asan

# Clean build artifacts
clean:
	rm -f hello hello_strict hello_with_plugins plugin_demo custom_plugin_example
	rm -f test_plugin_unit test_plugin_unit_asan
	rm -f *.o *.exe *.out
	rm -f test_hello_runner hello_lib.o

# Run all tests
test: unit-test strict
	@echo ""
	@echo "Running validation tests..."
	@if [ -f test/validate.sh ]; then \
		./test/validate.sh; \
	else \
		echo "No validation script found"; \
		exit 1; \
	fi

# CI build target - strict compilation + all tests
ci: clean
	@echo "===================================="
	@echo "CI Build: Strict Compilation"
	@echo "===================================="
	$(MAKE) strict
	@echo ""
	@echo "===================================="
	@echo "CI Build: Unit Tests"
	@echo "===================================="
	$(MAKE) unit-test
	@echo ""
	@echo "===================================="
	@echo "CI Build: Validation Tests"
	@echo "===================================="
	@if [ -f test/validate.sh ]; then \
		./test/validate.sh; \
	else \
		echo "No validation script found"; \
		exit 1; \
	fi
	@echo ""
	@echo "===================================="
	@echo "CI Build: Complete ✓"
	@echo "===================================="

# Help target
help:
	@echo "Available targets:"
	@echo "  all                  - Build hello (default)"
	@echo "  hello                - Build basic hello program with plugin support"
	@echo "  demo                 - Build and run plugin demonstration"
	@echo "  hello-plugins        - Build hello with example plugins"
	@echo "  plugin_demo          - Build plugin demo program"
	@echo "  custom_plugin_example - Build custom plugin example"
	@echo "  test_plugin_unit     - Build unit tests"
	@echo "  unit-test            - Build and run unit tests"
	@echo "  strict               - Build with strict compilation flags"
	@echo "  asan                 - Build and run with AddressSanitizer"
	@echo "  clean                - Remove all built files"
	@echo "  test                 - Run all tests (unit + validation)"
	@echo "  ci                   - Run CI build (strict + all tests)"
	@echo "  help                 - Show this help message"

.PHONY: all demo clean test strict help unit-test ci asan
