# Makefile for Hello World C program with plugin system support
# Provides convenient build targets including unit testing and plugin builds

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99
TARGET = hello
TEST_RUNNER = test_hello_runner
HELLO_LIB_OBJ = hello_lib.o

# Plugin source files
PLUGIN_SRC = plugin.c
PLUGIN_UPPERCASE = plugin_uppercase.c
PLUGIN_DECORATOR = plugin_decorator.c
PLUGIN_LOGGER = plugin_logger.c

# Default target: build the main program
.PHONY: all
all: $(TARGET)

# Build the main hello program
$(TARGET): hello.c hello.h
	$(CC) $(CFLAGS) -o $(TARGET) hello.c

# Unit test target (required by CI)
.PHONY: unit-test
unit-test:
	@echo "Building unit tests..."
	@$(CC) $(CFLAGS) -I. -c -o $(HELLO_LIB_OBJ) hello.c -DUNIT_TEST
	@$(CC) $(CFLAGS) -I. -o $(TEST_RUNNER) test/test_hello.c $(HELLO_LIB_OBJ)
	@echo "Running unit tests..."
	@./$(TEST_RUNNER)
	@rm -f $(HELLO_LIB_OBJ) $(TEST_RUNNER)

# Build with all plugins
.PHONY: with-plugins
with-plugins:
	$(CC) $(CFLAGS) -DUSE_PLUGINS -o hello_with_plugins hello.c $(PLUGIN_SRC) $(PLUGIN_UPPERCASE) $(PLUGIN_DECORATOR) $(PLUGIN_LOGGER)
	@echo "Built hello_with_plugins with all example plugins enabled"
	@echo "Run with: ./hello_with_plugins"

# Build with uppercase plugin only
.PHONY: uppercase
uppercase:
	$(CC) $(CFLAGS) -DUSE_PLUGINS -o hello_uppercase hello.c $(PLUGIN_SRC) $(PLUGIN_UPPERCASE)
	@echo "Built hello_uppercase with uppercase plugin"
	@echo "Run with: ./hello_uppercase"

# Build with decorator plugin only
.PHONY: decorator
decorator:
	$(CC) $(CFLAGS) -DUSE_PLUGINS -o hello_decorator hello.c $(PLUGIN_SRC) $(PLUGIN_DECORATOR)
	@echo "Built hello_decorator with decorator plugin"
	@echo "Run with: ./hello_decorator"

# Build with logger plugin only
.PHONY: logger
logger:
	$(CC) $(CFLAGS) -DUSE_PLUGINS -o hello_logger hello.c $(PLUGIN_SRC) $(PLUGIN_LOGGER)
	@echo "Built hello_logger with logger plugin"
	@echo "Run with: ./hello_logger"

# Build plugins with strict compilation flags (for testing)
.PHONY: strict-plugins
strict-plugins:
	$(CC) $(CFLAGS) -DUSE_PLUGINS -o hello_strict_plugins hello.c $(PLUGIN_SRC) $(PLUGIN_UPPERCASE) $(PLUGIN_DECORATOR) $(PLUGIN_LOGGER)
	@echo "Built with strict flags and all plugins"

# Strict build (matches CI validation)
.PHONY: strict
strict:
	$(CC) $(CFLAGS) -o hello_strict hello.c

# Build with debug symbols
.PHONY: debug
debug:
	$(CC) -g -Wall -Wextra -o hello_debug hello.c

# Run validation tests
.PHONY: test
test: strict
	@bash test/validate.sh

# Run validation tests quietly (for CI)
.PHONY: test-quiet
test-quiet: strict
	@bash test/validate.sh --quiet

# Build with Clang
.PHONY: clang
clang:
	clang -Wall -Wextra -o hello_clang hello.c

# Clean build artifacts
.PHONY: clean
clean:
	rm -f $(TARGET) $(TEST_RUNNER) $(HELLO_LIB_OBJ) hello_strict hello_clang hello_debug
	rm -f hello_strict_plugins hello_with_plugins hello_uppercase hello_decorator hello_logger
	rm -f *.o *.exe *.out
	@echo "Cleaned build artifacts"

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  make              - Build hello without plugins (default)"
	@echo "  make unit-test    - Build and run unit tests"
	@echo "  make with-plugins - Build with all example plugins"
	@echo "  make uppercase    - Build with uppercase plugin only"
	@echo "  make decorator    - Build with decorator plugin only"
	@echo "  make logger       - Build with logger plugin only"
	@echo "  make strict       - Build with strict compilation flags"
	@echo "  make strict-plugins - Build plugins with strict flags"
	@echo "  make test         - Run validation tests"
	@echo "  make clang        - Build with Clang compiler"
	@echo "  make debug        - Build with debug symbols"
	@echo "  make clean        - Remove all build artifacts"
	@echo ""
	@echo "Plugin system usage:"
	@echo "  - hello.c works normally when built without -DUSE_PLUGINS"
	@echo "  - Add -DUSE_PLUGINS to enable plugin support"
	@echo "  - Link plugin.c and your plugin files when building with plugins"
	@echo ""
	@echo "Example custom build:"
	@echo "  gcc -DUSE_PLUGINS -o my_hello hello.c plugin.c my_plugin.c"
