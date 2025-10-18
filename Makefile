# Makefile for Hello World with Plugin System
#
# Targets:
#   make              - Build the original hello program (default)
#   make plugin-demo  - Build the full plugin demonstration
#   make plugins      - Build all plugins
#   make test         - Run validation tests
#   make clean        - Remove built files

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c99
STRICT_FLAGS = $(CFLAGS) -Wformat=2 -Wconversion -Wsign-conversion -Werror

# Source files
PLUGIN_SRC = plugin.c
UPPERCASE_PLUGIN = plugins/uppercase.c
EXCLAIM_PLUGIN = plugins/exclaim.c

# Object files
PLUGIN_OBJ = plugin.o
UPPERCASE_OBJ = plugins/uppercase.o
EXCLAIM_OBJ = plugins/exclaim.o

# Executables
HELLO = hello
HELLO_PLUGINS = hello_with_plugins
PLUGIN_DEMO = hello_plugin_demo
TEST_PLUGIN_RUNNER = test_plugin_runner

.PHONY: all plugin-demo plugins test clean strict help

# Default target: build original hello
all: $(HELLO)

# Build original hello.c
$(HELLO): hello.c
	$(CC) $(CFLAGS) -o $(HELLO) hello.c

# Build plugin system library
$(PLUGIN_OBJ): $(PLUGIN_SRC) plugin.h
	$(CC) $(CFLAGS) -c $(PLUGIN_SRC) -o $(PLUGIN_OBJ)

# Build uppercase plugin
$(UPPERCASE_OBJ): $(UPPERCASE_PLUGIN) plugin.h
	$(CC) $(CFLAGS) -c $(UPPERCASE_PLUGIN) -o $(UPPERCASE_OBJ)

# Build exclaim plugin
$(EXCLAIM_OBJ): $(EXCLAIM_PLUGIN) plugin.h
	$(CC) $(CFLAGS) -c $(EXCLAIM_PLUGIN) -o $(EXCLAIM_OBJ)

# Build all plugins
plugins: $(PLUGIN_OBJ) $(UPPERCASE_OBJ) $(EXCLAIM_OBJ)

# Build plugin demonstration
plugin-demo: $(PLUGIN_DEMO)

$(PLUGIN_DEMO): hello_plugin_demo.c $(PLUGIN_OBJ) $(UPPERCASE_OBJ)
	$(CC) $(CFLAGS) -o $(PLUGIN_DEMO) hello_plugin_demo.c $(PLUGIN_OBJ) $(UPPERCASE_OBJ)

# Build hello with plugins (simple version)
$(HELLO_PLUGINS): hello_plugins.c $(PLUGIN_OBJ) $(UPPERCASE_OBJ)
	$(CC) $(CFLAGS) -o $(HELLO_PLUGINS) hello_plugins.c $(PLUGIN_OBJ) $(UPPERCASE_OBJ)

# Strict compilation (for CI)
strict: hello.c
	$(CC) $(STRICT_FLAGS) -o hello_strict hello.c

# Run validation tests
test:
	@./test/validate.sh

# Run plugin system tests
test-plugins: $(PLUGIN_OBJ)
	@echo "Building and running plugin system tests..."
	$(CC) $(CFLAGS) -I. -o $(TEST_PLUGIN_RUNNER) test/test_plugin.c $(PLUGIN_OBJ)
	@./$(TEST_PLUGIN_RUNNER)

# Run all tests
test-all: test test-plugins

# Run plugin demo
demo: $(PLUGIN_DEMO)
	@echo "Running plugin system demo..."
	@./$(PLUGIN_DEMO)

# Clean build artifacts
clean:
	rm -f $(HELLO) $(HELLO_PLUGINS) $(PLUGIN_DEMO) hello_strict $(TEST_PLUGIN_RUNNER)
	rm -f $(PLUGIN_OBJ) $(UPPERCASE_OBJ) $(EXCLAIM_OBJ)
	rm -f *.o plugins/*.o

# Help
help:
	@echo "Hello World with Plugin System - Build Targets"
	@echo ""
	@echo "  make              - Build original hello program"
	@echo "  make plugin-demo  - Build and run plugin demonstration"
	@echo "  make plugins      - Build all plugin components"
	@echo "  make test         - Run validation tests"
	@echo "  make test-plugins - Run plugin system unit tests"
	@echo "  make test-all     - Run all tests"
	@echo "  make strict       - Build with strict error checking"
	@echo "  make demo         - Build and run the plugin demo"
	@echo "  make clean        - Remove all built files"
	@echo "  make help         - Show this help message"
