# Makefile for hello.c with plugin system support
# 
# Targets:
#   make                    - Build hello without plugins (default)
#   make hello              - Build hello without plugins
#   make with-plugins       - Build hello with all example plugins
#   make uppercase          - Build hello with uppercase plugin
#   make decorator          - Build hello with decorator plugin
#   make clean              - Remove all build artifacts
#   make test               - Run validation tests
#
# Examples:
#   make                    - Builds standard hello
#   make with-plugins       - Builds with all plugins enabled
#   make uppercase          - Builds with uppercase transformation

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99

# Source files
CORE = hello.c
PLUGIN_SRC = plugin.c
PLUGIN_UPPERCASE = plugin_uppercase.c
PLUGIN_DECORATOR = plugin_decorator.c

# Default target - build without plugins
.PHONY: all
all: hello

# Build standard hello (no plugins)
hello: $(CORE)
	$(CC) $(CFLAGS) -o $@ $^

# Build with all plugins
.PHONY: with-plugins
with-plugins:
	$(CC) $(CFLAGS) -DUSE_PLUGINS -o hello_with_plugins $(CORE) $(PLUGIN_SRC) $(PLUGIN_UPPERCASE) $(PLUGIN_DECORATOR)
	@echo "Built hello_with_plugins with all plugins enabled"
	@echo "Run with: ./hello_with_plugins"

# Build with uppercase plugin only
.PHONY: uppercase
uppercase:
	$(CC) $(CFLAGS) -DUSE_PLUGINS -o hello_uppercase $(CORE) $(PLUGIN_SRC) $(PLUGIN_UPPERCASE)
	@echo "Built hello_uppercase with uppercase plugin"
	@echo "Run with: ./hello_uppercase"

# Build with decorator plugin only
.PHONY: decorator
decorator:
	$(CC) $(CFLAGS) -DUSE_PLUGINS -o hello_decorator $(CORE) $(PLUGIN_SRC) $(PLUGIN_DECORATOR)
	@echo "Built hello_decorator with decorator plugin"
	@echo "Run with: ./hello_decorator"

# Build with strict compilation flags (for CI)
.PHONY: strict
strict:
	$(CC) $(STRICT_FLAGS) -o hello_strict $(CORE)

# Run validation tests
.PHONY: test
test: strict
	@echo "Running validation tests..."
	@bash test/validate.sh

# Run validation tests quietly (for CI)
.PHONY: test-quiet
test-quiet: strict
	@bash test/validate.sh --quiet

# Clean build artifacts
.PHONY: clean
clean:
	rm -f hello hello_strict hello_with_plugins hello_uppercase hello_decorator
	rm -f *.o *.out *.exe
	@echo "Cleaned build artifacts"

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  make              - Build hello without plugins (default)"
	@echo "  make with-plugins - Build with all example plugins"
	@echo "  make uppercase    - Build with uppercase plugin only"
	@echo "  make decorator    - Build with decorator plugin only"
	@echo "  make strict       - Build with strict compilation flags"
	@echo "  make test         - Run validation tests"
	@echo "  make clean        - Remove all build artifacts"
	@echo ""
	@echo "Plugin system usage:"
	@echo "  - hello.c works normally when built without -DUSE_PLUGINS"
	@echo "  - Add -DUSE_PLUGINS to enable plugin support"
	@echo "  - Link plugin.c and your plugin files when building with plugins"
	@echo ""
	@echo "Example custom build:"
	@echo "  gcc -DUSE_PLUGINS -o my_hello hello.c plugin.c my_plugin.c"
