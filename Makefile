# Makefile for Hello World C program with plugin system
# Supports building the main program and example plugins

# Compiler and flags configuration
CC ?= gcc
CFLAGS ?= -Wall -Wextra -std=c99
OPTFLAGS = -O2
DEBUGFLAGS = -g
# STRICT_FLAGS: includes -Werror to fail on any warnings, ensuring code quality
STRICT_FLAGS = -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror
PLUGIN_FLAGS = -fPIC -shared

# Combined flag sets for cleaner target definitions
STRICT_CFLAGS = $(CFLAGS) $(STRICT_FLAGS)
DEBUG_CFLAGS = $(CFLAGS) $(DEBUGFLAGS)
OPT_CFLAGS = $(CFLAGS) $(OPTFLAGS)

# Source files and targets
SOURCE = hello.c
PLUGIN_SOURCE = plugin.c
TARGET = hello
DEBUG_TARGET = hello_debug
STRICT_TARGET = hello_strict
CLANG_TARGET = hello_clang

# Plugin directory
PLUGINS_DIR = plugins

# Default target (optimized build)
all: $(TARGET) $(PLUGINS_DIR)

# Optimized build target
$(TARGET): $(SOURCE) $(PLUGIN_SOURCE)
	$(CC) $(OPT_CFLAGS) -ldl -rdynamic -o $(TARGET) $(SOURCE) $(PLUGIN_SOURCE)

# Debug build target (debug info without optimization)
debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(SOURCE) $(PLUGIN_SOURCE)
	$(CC) $(DEBUG_CFLAGS) -ldl -rdynamic -o $(DEBUG_TARGET) $(SOURCE) $(PLUGIN_SOURCE)

# Strict compilation target (with -Werror for CI/quality assurance)
strict: $(STRICT_TARGET)

$(STRICT_TARGET): $(SOURCE) $(PLUGIN_SOURCE)
	$(CC) $(STRICT_CFLAGS) -ldl -rdynamic -o $(STRICT_TARGET) $(SOURCE) $(PLUGIN_SOURCE)

# Clang build target (with optimization)
clang: $(CLANG_TARGET)

$(CLANG_TARGET): $(SOURCE) $(PLUGIN_SOURCE)
	clang $(OPT_CFLAGS) -ldl -rdynamic -o $(CLANG_TARGET) $(SOURCE) $(PLUGIN_SOURCE)

# Create plugins directory
$(PLUGINS_DIR):
	mkdir -p $(PLUGINS_DIR)

# Example plugins
plugins: $(PLUGINS_DIR)/uppercase.so $(PLUGINS_DIR)/exclamation.so $(PLUGINS_DIR)/reverse.so

$(PLUGINS_DIR)/uppercase.so: plugins/uppercase.c plugin.h
	@mkdir -p $(PLUGINS_DIR)
	$(CC) $(PLUGIN_CFLAGS) $(PLUGIN_FLAGS) -o $(PLUGINS_DIR)/uppercase.so plugins/uppercase.c

$(PLUGINS_DIR)/exclamation.so: plugins/exclamation.c plugin.h
	@mkdir -p $(PLUGINS_DIR) 
	$(CC) $(PLUGIN_CFLAGS) $(PLUGIN_FLAGS) -o $(PLUGINS_DIR)/exclamation.so plugins/exclamation.c

$(PLUGINS_DIR)/reverse.so: plugins/reverse.c plugin.h
	@mkdir -p $(PLUGINS_DIR) 
	$(CC) $(PLUGIN_CFLAGS) $(PLUGIN_FLAGS) -o $(PLUGINS_DIR)/reverse.so plugins/reverse.c

# Run the default binary
run: $(TARGET)
	./$(TARGET)

# Test target: run validation script
# Note: Uses default build, not strict, to avoid -Werror issues in CI
test:
	bash test/validate.sh

# Test plugin system comprehensively  
test-plugins: all
	bash test/plugin_tests.sh

# Test target with quiet output for CI environments
test-quiet:
	bash test/validate.sh --quiet

# Strict test: run validation with strict compilation (includes -Werror)
test-strict: strict
	bash test/validate.sh

# Validate target: run validation script on built binaries
validate: strict
	@chmod +x test/validate.sh
	@echo "=== Validating strict build ==="
	@bash test/validate.sh --strict ./$(STRICT_TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET) $(DEBUG_TARGET) $(CLANG_TARGET) $(STRICT_TARGET) *.exe *.out *.o
	rm -f $(PLUGINS_DIR)/*.so

# Clean everything including plugins directory
distclean: clean
	rm -rf $(PLUGINS_DIR)

# Help target to show available commands
help:
	@echo "Available targets:"
	@echo "  all      - Build optimized hello (default)"
	@echo "  debug    - Build with debug flags (-g)"
	@echo "  strict   - Build with strict warnings and -Werror"
	@echo "  clang    - Build with clang compiler"
	@echo "  plugins  - Build example plugins"
	@echo "  run      - Run the default binary"
	@echo "  test     - Build and run basic validation (no -Werror)"
	@echo "  test-strict - Build strict and run validation (with -Werror)"
	@echo "  test-plugins - Run comprehensive plugin tests"
	@echo "  test-quiet - Same as test but with minimal output"
	@echo "  validate - Validate strict build output and exit code"
	@echo "  clean    - Remove all build artifacts"
	@echo "  distclean - Remove build artifacts and plugins directory"
	@echo "  help     - Show this help message"

# Declare phony targets
.PHONY: all debug strict clang plugins run test test-strict test-plugins test-quiet validate validate-all clean distclean help
